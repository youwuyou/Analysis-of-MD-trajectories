#!/bin/bash

#TIMING

#run for different number of steps
for numatm in 1000 2000 4000 10000 20000 50000 70000 100000 1000000
do
	
	#time step = 20 / number of atoms
	dt=$(awk -v na=$numatm 'BEGIN {print 20/na } ')
	
	echo "running MD for dt=${dt}"

	cp params.template params
	sed -i '' -e "s/\_NumberMDSteps\_/${numatm}/g" params
	sed -i '' -e "s/\_NumberPrintOutput\_/${numatm}/g" params
	sed -i '' -e "s/\_TimeStep\_/${dt}/g" params
	./mdatom params > fluctuation_average_${numatm}	

done

bash extract_task1_time.sh