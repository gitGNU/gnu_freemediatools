#!/bin/tcsh

set CURRENT = $PWD

foreach file (`find . -name CVS`)
	#echo $file
	set ONE_DOWN = `echo ${file}/../..`
	#echo $ONE_DOWN
	set ONE_DOWN_LIST = `ls $file/..`
	#echo $ONE_DOWN_LIST
	#wc `echo $ONE_DOWN_LIST`
	if (`echo ${ONE_DOWN_LIST}` == "CVS") then
		echo $file
		set BAD_DIR = `echo $ONE_DOWN | awk -F"/" '{ print $(NF - 3) }'`
		echo Removing $BAD_DIR
		cd $ONE_DOWN
		rm -f -r $BAD_DIR
		cd $CURRENT
	endif
end
