set autoscale

set xtic auto
set ytic auto
set title "Timing Average and Fluctuation 50 Atoms"
set xlabel "N steps"
set ylabel "Wallclock time [ns]"


set terminal pdf 
set output 'timing_average_fluctuation_50atoms.pdf'

set style line 1 lc rgb "#0000FF" lw 2
set style line 2 lc rgb "#00FF00" lw 2
set style line 3 lc rgb "#FF0000" lw 2

plot 'task1_time_inaccurate' using($1) : ($2) with line linestyle 1 title "inaccurate fluctuation", \
     'task1_time_accurate' using($1) : ($2) with line linestyle 2 title "accurate fluctuation" \


