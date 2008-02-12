#!/bin/tcsh

set OTHERDIR = $1

echo Comparing against $OTHERDIR

foreach file (`find . -name "*.[ch]" | grep -v moc_`)
	if (`diff $file $OTHERDIR/$file | wc | awk '{print $1}'` != 0) then
		echo $file differs from $OTHERDIR/$file
	endif
end

foreach file (`find . -name "*.cpp" | grep -v moc_`)
	if (`diff $file $OTHERDIR/$file | wc | awk '{print $1}'` != 0) then
		echo $file differs from $OTHERDIR/$file
	endif
end

foreach file (`find . -name "*.pro" | grep -v moc_`)
	if (`diff $file $OTHERDIR/$file | wc | awk '{print $1}'` != 0) then
		echo $file differs from $OTHERDIR/$file
	endif
end
