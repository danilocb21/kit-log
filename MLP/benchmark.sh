#!/bin/bash
make rebuild

k=1

echo "Instance,Time(s),Cost" > ./output.csv

for instance in data/instances/*; do
    echo "Instance $k of 49"

    output="$(basename "$instance")"
    
	echo "Processando $(basename $instance)"
	
	out_cpp=$(./mlp "${instance}")
	out_csv="${out_cpp// /,}"
	
	output="${output},${out_csv}"
    
    echo "$output" >> ./output.csv
    
    k=$(($k+1))
done