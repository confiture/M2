dotppm=.ppm
for file in $(ls $2/*.ppm)
do
	echo $file
	fileSex=$(echo $(basename $file) | cut -d '.' -f 1)
	echo $fileSex
	$1 -i $file -n 8 -g 5 -o $(echo $3/$fileSex$dotppm) -d 5 -tr
done



