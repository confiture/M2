numSeed=Seed
dotppm=.ppm
for file in $(ls $2/*.ppm)
do
	echo $file
	fileSex=$(echo $(basename $file) | cut -d '.' -f 1)
	echo $fileSex
	for r in $(seq 1 4)
	do
		$1 -i $file -n 8 -g 4 -o $(echo $3/$fileSex$numSeed$r$dotppm) -d 3 -r $r -cr
	done
done



