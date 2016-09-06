#!/bin/bash
for filename in TESTS/*.txt; do
	name=${filename##*/}
	name=${name%.txt}
	name="$name.result"
	./DB_Client 12345 t $filename > RESULTS/$name
done 
