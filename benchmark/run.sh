#!/bin/bash

#TIMING

#run for different number of steps
for numatm in 100 200 400 1000 2000 #50000
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

extract_task1_time.sh
