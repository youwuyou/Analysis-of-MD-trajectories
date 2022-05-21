#!/bin/bash

echo "#timesteps  time" > task1_time

for numatm in 100 200 400 1000 2000
do

	dt=$(awk -v na=$numatm 'BEGIN {print 20/na } ')
	printf "${dt}" >> task1_time
	
	awk 'NR == 69 { 
  		print " " $8
  		}' fluctuation_average_${numatm} >> task1_time
  		

done
