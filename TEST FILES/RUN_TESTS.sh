#!/bin/bash
total="RESULTS/TOTAL.result"
> $total
for filename in TESTS/*.txt; do
	name=${filename##*/}
	name=${name%.txt}
	name="$name.result"
	./DB_Client 12345 t $filename > RESULTS/$name

	resultFile="RESULTS/$name"
	if grep -Fq failed $resultFile
	then
	echo "$name...BAD" >> $total
	else
	echo "$name...GOOD" >> $total
	fi
done 
