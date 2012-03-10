# /bin/bash

DIRS=".
plugins
DcmType"

TOTAL=0

for dir in $DIRS ; do
    echo git add $dir/*cpp $dir/*.h $dir/makefile 
    git add $dir/*cpp $dir/*.h $dir/makefile 
    LC=`cat $dir/*cpp $dir/*.h $dir/makefile | wc -l`
    TOTAL=`dc -e "$TOTAL $LC +p"`
done

echo Total lines so far: $TOTAL
