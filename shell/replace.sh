#!/bin/sh

if [ $# != 3 ]; then
	echo "Usage: $0 <filename> <linenum or pattern> <newtext>"
	echo "Ex:"
	echo "\t$0 file1 80 \"ab cd\" 	: From file1 line 80 will be replaced with 'ab cd'"
	echo "\t$0 file2 raj jar 	: From file2 all 'raj' strings will be replaced with 'jar'"
	echo "\n** If pattern or newtext argument has space specify them in quotes & escape special chars"
	exit 1
fi

newtext=$3
case $newtext in
	'')
		echo "Error: 3rd argument text to be replaced is empty"
		exit 1
		;;
esac

filename=$1
if [ ! -f $filename ]; then
	echo "Error: File name $filename does not exist"
	exit 1
fi

pattern=$2
case $pattern in
	'')
		echo "Error: 2nd argument line number or pattern is empty"
		exit 1
		;;
	^[0-9]$)
		linenum=$pattern
		replace_cmd="s/^.*$/$newtext/"
		;;
	*)
		lines=`grep -n "$pattern" "$filename" 2>&1`
		if [ $? != 0 ]; then
			echo "Pattern [$pattern] not found"
			exit 1
		fi
		linenums=`grep -n "$pattern" "$filename" | sed s/:.*// | tr '\n' ' ' 2>&1`
		replace_cmd="s/$pattern/$newtext/"
		;;
esac

echo -n "\nFILE [$filename] pattern [$pattern] will be REPLACED from below lines\n\n$lines\n\nWITH \n\t$newtext\n"

replace() {
/usr/bin/ex $filename <<EOF
	$1
	$2
	w
	q
EOF
}

echo -n "Are you sure to replace all occurrences (y/n/i): "
read choice

if [ $choice = "y" ]; then
	for linenum in $linenums; do
		replace $linenum $replace_cmd
	done
elif [ $choice = "i" ]; then
	echo "Interactive mode: "
	for linenum in $linenums; do
		echo -n "Are you sure to replace line number $linenum (y/n): "
		read choice
		if [ $choice = "y" ]; then
			replace $linenum $replace_cmd
		else
			echo "Skipping $linenum"
		fi
	done
fi
