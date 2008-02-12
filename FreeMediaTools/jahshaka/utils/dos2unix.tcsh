#!/bin/tcsh

set OTHERDIR = $1

echo Cleaning up $OTHERDIR

foreach file (`find . -name "*.[ch]" | grep -v moc_`)
    dos2unix $file -o $file
    echo cleaning up $file
end

foreach file (`find . -name "*.cpp" | grep -v moc_`)
    dos2unix $file -o $file
    echo cleaning up $file
end

foreach file (`find . -name "*.pro" | grep -v moc_`)
    dos2unix $file -o $file
    echo cleaning up $file
end
