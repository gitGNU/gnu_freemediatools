#!/bin/tcsh

set OTHERDIR = $1

echo "Comparing against $OTHERDIR"

echo "Comparing .[ch] files"

foreach file (`find . -name "*.[ch]" | grep -v moc_`)
	if (`diff $file $OTHERDIR/$file | wc | awk '{print $1}'` != 0) then
		echo Differences between $file $OTHERDIR/$file
		diff $file $OTHERDIR/$file
	endif
end

echo "Comparing .cpp files"

foreach file (`find . -name "*.cpp" | grep -v moc_`)
	if (`diff $file $OTHERDIR/$file | wc | awk '{print $1}'` != 0) then
		echo Differences between $file $OTHERDIR/$file
		diff $file $OTHERDIR/$file
	endif
end

echo "Comparing .pro files"

foreach file (`find . -name "*.pro" | grep -v moc_`)
	if (`diff $file $OTHERDIR/$file | wc | awk '{print $1}'` != 0) then
		echo Differences between $file $OTHERDIR/$file
		diff $file $OTHERDIR/$file
	endif
end

echo "Comparing .ds[pw] files"

foreach file (`find . -name "*.ds[pw]" | grep -v moc_`)
	if (`diff $file $OTHERDIR/$file | wc | awk '{print $1}'` != 0) then
		echo Differences between $file $OTHERDIR/$file
		diff $file $OTHERDIR/$file
	endif
end

echo "Comparing .vcproj files"

foreach file (`find . -name "*.vcproj" | grep -v moc_`)
	if (`diff $file $OTHERDIR/$file | wc | awk '{print $1}'` != 0) then
		echo Differences between $file $OTHERDIR/$file
		diff $file $OTHERDIR/$file
	endif
end

echo "Comparing .sln files"

foreach file (`find . -name "*.sln" | grep -v moc_`)
	if (`diff $file $OTHERDIR/$file | wc | awk '{print $1}'` != 0) then
		echo Differences between $file $OTHERDIR/$file
		diff $file $OTHERDIR/$file
	endif
end

echo "Comparing .tcsh files"

foreach file (`find . -name "*.tcsh" | grep -v moc_`)
	if (`diff $file $OTHERDIR/$file | wc | awk '{print $1}'` != 0) then
		echo Differences between $file $OTHERDIR/$file
		diff $file $OTHERDIR/$file
	endif
end
