make rebuild

k=1

printf "Instance,Time DFS(s),Cost DFS,Time BFS(s),Cost BFS,Time BBS(s),Cost BBS" >> ./output.csv

for instance in data/instances/*; do
    echo $instance >> ./output.csv
    
    echo "Instance $k of 9"
    for strategy in DFS BFS BBS; do
        echo "Processando $instance com $strategy"
        
        ./bin/tsp ${instance} $strategy >> ./output.csv
    done
    k=$(($k+1))
done
