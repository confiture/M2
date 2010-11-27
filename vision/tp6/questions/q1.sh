numGroupe=Gr
dotpgm=.pgm
for file in $(ls $2/*.pgm)
do
	echo $file
	fileSex=$(echo $(basename $file) | cut -d '.' -f 1)
	echo $fileSex
	for g in $(seq 3 6)
	do
		echo $3/$fileSex$g$dotpgm
		$1 -i $file -n 8 -g $g -o $(echo $3/$fileSex$g$dotpgm)
	done
done



