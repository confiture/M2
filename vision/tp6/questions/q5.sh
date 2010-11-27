numGroupe=Gr
dotppm=.ppm
for file in $(ls $2/*.ppm)
do
	echo $file
	fileSex=$(echo $(basename $file) | cut -d '.' -f 1)
	echo $fileSex
	for g in $(seq 2 10)
	do
		$1 -i $file -n 8 -g $g -o $(echo $3/$fileSex$numGroupe$g$dotppm) -d 3
	done
done



