#!/bin/sh

if [ $# != 3 ]; then
	echo "Usage: $0 <dirpath or dirpath/regex> <pattern> <newtext>"
	echo "Ex:"
	echo "\t$0 dirpath xyz \"ab cd\" 	: From all files inside dirpath replace 'xyz' with 'ab cd'"
	echo "\t$0 dirpath/*.c raj jar 	: From dirpath select all 'c' files and replace 'raj' strings with 'jar'"
	echo "\n** If pattern or newtext argument has space specify them in quotes & escape special chars"
	exit 1
fi

output=`find $1 2>&1`
if [ $? = 0 ]; then
	for file in `find $1 | tr '\n' ' '`; do
		./replace.sh $file $2 $3
		if [ $? = 1 ]; then
			exit 1
		fi
	done
else
	echo "Dir path [$name] is not correct"
	exit 1
fi



