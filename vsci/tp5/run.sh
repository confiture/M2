
ext=.eps
tabd=(1 2 5 10 20 50 100 200)

for f in $(ls data)
do
	for d in ${tabd[@]}
	do
		echo $f
		feps=$(echo $f | cut -d . -f 1)
		bin/tp5doug $d data/$f imagesDoug/$feps$d$ext
	done
done
