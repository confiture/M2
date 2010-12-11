di=0
dj=0
ddi=130
ddj=50
for i in synthetic/*.pgm
do
	echo $i
	ddi=$(echo "$ddi+$di" | bc -l)
	ddj=$(echo "$ddj+$dj" | bc -l)
	delta=$(bin/tp8 $i synthetic/taz.pgm cadres/$(basename $i) $ddi $ddj)
	di=$(echo $delta | cut -d ' ' -f 1)
	dj=$(echo $delta | cut -d ' ' -f 2)
	echo $delta
	echo $di
	echo $dj
done
