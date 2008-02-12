#!/bin/tcsh

set CPP_FILES = `find . -name "*.cpp"`
set HEADER_FILES = `find . -name "*.h"`
set C_FILES = `find . -name "*.c"`

foreach file (`echo $CPP_FILES $HEADER_FILES $C_FILES`)
	if (`grep -c $1 $file` != 0) then
		set tempfile = $file.fixed
		echo $file matches pattern $1
		./replace.perl $file $tempfile $1 $2
		#echo mv $tempfile $file
		/bin/mv $tempfile $file
	endif
end
	
	
