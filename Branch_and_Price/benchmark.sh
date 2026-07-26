#!/bin/bash
make rebuild

TIME_LIMIT=3600
k=1

if [ ! -f ./output.csv ]; then
    echo "Instance,Time(s),Upper Bound,Status" > ./output.csv
fi

for instance in instances/*; do
    inst_name="$(basename "$instance")"

    if grep -q "^${inst_name}," ./output.csv; then
        echo "Instância ${inst_name} já processada. Pulando..."
        k=$(($k+1))
        continue
    fi

    echo "Instance $k of 720"
    output="${inst_name}"

    echo "Processando ${inst_name}"

    out_cpp=$(timeout $TIME_LIMIT ./bp "${instance}")
    exit_status=$?

    if [ $exit_status -eq 124 ]; then
        echo " -> TIMEOUT"
        out_csv="TIMEOUT,TIMEOUT,TIMEOUT"
    elif [ $exit_status -ne 0 ]; then
        echo " -> ERROR"
        out_csv="ERROR,ERROR,ERROR"
    else
        time=$(echo "$out_cpp" | grep "Total_Time:" | awk '{print $2}')
        ub=$(echo "$out_cpp" | grep "UB:" | awk '{print $2}')
        status=$(echo "$out_cpp" | grep "Status:" | awk '{print $2}')
        out_csv="${time},${ub},${status}"
    fi

    output="${output},${out_csv}"

    echo "$output" >> ./output.csv

    k=$(($k+1))
done
