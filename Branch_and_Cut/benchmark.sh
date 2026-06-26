#!/bin/bash
make rebuild

k=1

if [ ! -f ./output.csv ]; then
    echo "Instance,Tree Size,Time(s),Lower Bound,Upper Bound,Status" > ./output.csv
fi

for instance in instances/*; do
    inst_name="$(basename "$instance")"

    if grep -q "^${inst_name}," ./output.csv; then
        echo "Instância ${inst_name} já processada. Pulando..."
        k=$(($k+1))
        continue
    fi

    echo "Instance $k of 58"
    output="${inst_name}"
    
    echo "Processando ${inst_name}"
    
    out_cpp=$(./tsp "${instance}")
    exit_status=$?

    if [ $exit_status -eq 124 ]; then
        echo " -> TIMEOUT"
        out_csv="TIMEOUT,TIMEOUT,TIMEOUT,TIMEOUT,TIMEOUT"
    elif [ $exit_status -ne 0 ]; then
        echo " -> ERROR"
        out_csv="ERROR,ERROR,ERROR,ERROR,ERROR"
    else
        out_csv="${out_cpp// /,}"
    fi
    
    output="${output},${out_csv}"
    
    echo "$output" >> ./output.csv
    
    k=$(($k+1))
done