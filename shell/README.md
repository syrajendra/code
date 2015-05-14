TOOLS

    1. Replace string or complete line with specified text
    ./replace.sh 
    Usage: ./replace.sh <filename> <linenum or pattern> <newtext>
    Ex:
        ./replace.sh file1 80 "ab cd"   : From file1 line 80 will be replaced with 'ab cd'
        ./replace.sh file2 raj jar      : From file2 all 'raj' strings will be replaced with 'jar'
    ** If pattern or newtext argument has space specify them in quotes & escape special chars
    

    2. Replace string with specified text recursively in folder
     ./replace-recursive.sh
    Usage: ./replace-recursive.sh <dirpath or dirpath/regex> <pattern> <newtext>
    Ex:
        ./replace-recursive.sh dirpath xyz "ab cd"      : From all files inside dirpath replace 'xyz' with 'ab cd'
        ./replace-recursive.sh dirpath/*.c raj jar      : From dirpath select all 'c' files and replace 'raj' strings with 'jar'
    ** If pattern or newtext argument has space specify them in quotes & escape special chars

