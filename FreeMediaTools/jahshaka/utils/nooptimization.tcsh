#!/bin/tcsh

foreach makefile (`find . -name Makefile`)
	echo $makefile
	sed '1,$s/-O.//g' $makefile > tmp
	mv -f tmp $makefile
end
