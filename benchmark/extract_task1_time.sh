#!/bin/bash
<<COMMENT
echo "#timesteps  time" > task1_time_accurate

for numatm in 1000 2000 4000 10000 20000 50000 70000 100000 1000000
do

	printf "${numatm}" >> task1_time_accurate
	
	awk 'NR == 69 { 
  		print " " $8
  		}' fluctuation_average_${numatm} >> task1_time_accurate
  		

done
COMMENT

#<<COMMENT
echo "#timesteps  time" > task1_time_inaccurate

for numatm in 1000 2000 4000 10000 20000 50000 70000 100000 1000000 
do

	printf "${numatm}" >> task1_time_inaccurate
	
	awk 'NR == 69 { 
  		print " " $8
  		}' fluctuation_average_${numatm} >> task1_time_inaccurate
  		

done
#COMMENT

echo "#timesteps  time" > task2_time_direct

for numatm in 1000 2000 4000 10000 20000 50000 70000 100000 1000000 
do

	printf "${numatm}" >> task2_time_direct
	
	awk 'NR == 69 { 
  		print " " $8
  		}' fluctuation_average_${numatm} >> task2_time_direct
  		

done