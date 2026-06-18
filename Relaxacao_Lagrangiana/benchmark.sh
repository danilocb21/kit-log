#!/bin/bash
make rebuild

k=1

echo "Instance,Time DFS(s),Cost DFS,Time BFS(s),Cost BFS,Time BBS(s),Cost BBS" > ./output.csv

for instance in instances/*; do
    echo "Instance $k of 22"

    output="$(basename "$instance")"
    
    for strategy in DFS BFS BBS; do
        echo "Processando $(basename $instance) com $strategy"
        
        out_cpp=$(./tsp "${instance}" $strategy)
        out_csv="${out_cpp// /,}"
        
        output="${output},${out_csv}"
    done
    
    echo "$output" >> ./output.csv
    
    k=$(($k+1))
done