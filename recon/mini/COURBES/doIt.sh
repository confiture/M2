#!/bin/sh

# la partie 1, on fait varier le e
for i in $(seq 1 1 3)
do
	fe=1;
	for j in $(seq 10 3 30)
	do
		fe=$(echo $j/10.0 | bc -l)
		bin/mini $i $fe ../images/partie1-jeu$i-fe$j
	done
done

# la partie 2, on fait varier le e et le r
for i in $(seq 1 1 3)
do
	fe=1;
	for j in $(seq 10 3 30)
	do
		fe=$(echo $j/10.0 | bc -l)
		for k in $(seq 10 3 30)
		do
			fr=$(echo $k/10.0 | bc -l)
			bin/mini $i $fe $fr ../images/partie2-jeu$i-fe$j-fr$k
		done
	done
done

