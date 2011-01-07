#!/bin/sh

ext=.vect
for i in $( seq $2 $3 $4 )
do
	echo $i
	r=$(echo "($i - $2)/($4 - $2)" | bc -l)
	b=$(echo "1 - $r" | bc -l)
	echo $r
	echo $b
	bin/tp1 $i $1 $5$i$ext $r 0.0 $b
done
