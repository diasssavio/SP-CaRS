#!/bin/bash
for j in instances/insts/euclidean/small_insts/*.txt
do
	FILE=$(basename "$j" .txt)
	echo "$FILE"
	for i in $(seq 30)
	do
		./CaRS-ILS $j $i >> logs/$FILE.csv
		echo "$i...ok!"
	done
done
# echo "dados_att48eA"
# for i in $(seq 30)
# do
#  	./CaRS-ILS $i < instances/small_insts/dados_att48eA.txt >> logs/dados_att48eA.csv
# 	echo "$i...ok!"
# done
# echo "dados_berlin52eA"
# for i in $(seq 30)
# do
#  	./CaRS-ILS $i < instances/small_insts/dados_berlin52eA.txt >> logs/dados_berlin52eA.csv
# 	echo "$i...ok!"
# done
# echo "dados_eil76eB"
# for i in $(seq 30)
# do
#  	./CaRS-ILS $i < instances/small_insts/dados_eil76eB.txt >> logs/dados_eil76eB.csv
# 	echo "$i...ok!"
# done
# echo "dados_rat99eB"
# for i in $(seq 30)
# do
#  	./CaRS-ILS $i < instances/small_insts/dados_rat99eB.txt >> logs/dados_rat99eB.csv
# 	echo "$i...ok!"
# done
# echo "dados_st70eB"
# for i in $(seq 30)
# do
#  	./CaRS-ILS $i < instances/small_insts/dados_st70eB.txt >> logs/dados_st70eB.csv
# 	echo "$i...ok!"
# done
