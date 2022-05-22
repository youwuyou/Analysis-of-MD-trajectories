#set autoscale
set logscale xy 

set xtic auto
set ytic auto
set xlabel "N steps"
set ylabel "Wallclock time [us]"
set title "Timing Autocorrelation Function 100 Atoms"

set terminal pdf 
set output 'timing_autocorrelation_function_100atoms.pdf'

set style line 1 lc rgb "#0000FF" lw 2
set style line 2 lc rgb "#00FF00" lw 2
set style line 3 linecolor rgb '#dd181f' linetype 1 linewidth 2
set style line 4 linecolor rgb '#0060ad' linetype 1 linewidth 2

plot 'task2_time_direct' using($1) : ($2) with line linestyle 1 title "direct", \
     'task2_time_FFT' using($1) : ($2) with line linestyle 2 title "FFT", \
     x title "N", x**2 title "N^2" 


