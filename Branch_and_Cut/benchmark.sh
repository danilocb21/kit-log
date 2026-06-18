#!/bin/bash
make rebuild

TIME_LIMIT=150 
k=1

if [ ! -f ./output.csv ]; then
    echo "Instance,Time DFS(s),Cost DFS,Time BFS(s),Cost BFS,Time BBS(s),Cost BBS" > ./output.csv
fi

for instance in instances/*; do
    inst_name="$(basename "$instance")"

    if grep -q "^${inst_name}," ./output.csv; then
        echo "Instância ${inst_name} já processada. Pulando..."
        k=$(($k+1))
        continue
    fi

    echo "Instance $k of 22"
    output="${inst_name}"
    
    for strategy in DFS BFS BBS; do
        echo "Processando ${inst_name} com $strategy"
        
        out_cpp=$(timeout $TIME_LIMIT ./tsp "${instance}" $strategy)
        exit_status=$?

        if [ $exit_status -eq 124 ]; then
            echo " -> TIMEOUT ($strategy)"
            out_csv="TIMEOUT,TIMEOUT"
        elif [ $exit_status -ne 0 ]; then
            echo " -> ERRO ($strategy)"
            out_csv="ERROR,ERROR"
        else
            out_csv="${out_cpp// /,}"
        fi
        
        output="${output},${out_csv}"
    done
    
    echo "$output" >> ./output.csv
    
    k=$(($k+1))
done