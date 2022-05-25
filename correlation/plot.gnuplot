set autoscale

set xtic auto
set ytic auto
set xlabel "Step"
set ylabel "Total Correlation"


set terminal pdf
set output 'correlation_over_time.pdf'


# set output 'correlation_direct.pdf'
# set output 'correlation_FFT.pdf'


set style line 1 lc rgb "#0000FF" lw 2
set style line 2 lc rgb "#00FF00" lw 2


plot 'c_direct.txt' using($1) : ($2) with line linestyle 1 title "direct computation", \
     'c_fft.txt' using($1) : ($2) with line linestyle 2 title "FFT computation" \


