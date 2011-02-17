p=.png
for i in $(ls -r $1)
do
  f=$(echo $i | cut -d '.' -f 1 )
  convert $1/$i $1/$f$p
done

# for i in $(find . -path *.$1)
# do
#   f=$(echo $(basename $i) | cut -d '.' -f 1 )
#   convert $i $(dirname $i)/$f$2
# done
