set terminal wxt 0
set title "Pairs at 0% prefill"
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8,16,32,64,128)
set logscale x 2
set yrange[0:60]
set xlabel 'Threads'
set ylabel 'Mops/sec'
set datafile separator comma
plot 'wait_pairs_0.50_0.00.txt' using 1:2 title '1e3' with lines linetype 1,\
'' using 1:2:3 title '' with errorbars linetype 1,\
'' using 1:4 title '1e4' with lines linetype 2,\
'' using 1:4:5 title '' with errorbars linetype 2,\
'' using 1:6 title '1e5' with lines linetype 3,\
'' using 1:6:7 title '' with errorbars linetype 3,\
'' using 1:8 title '0' with lines linetype 4,\
'' using 1:8:9 title '' with errorbars linetype 4

set terminal wxt 1
set title "50/50 enqdeq at 0% prefill"
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8,16,32,64,128)
set logscale x 2
set yrange[0:60]
set xlabel 'Threads'
set ylabel 'Mops/sec'
set datafile separator comma
plot 'wait_enqdeq_0.50_0.00.txt' using 1:2 title '1e3' with lines linetype 1,\
'' using 1:2:3 title '' with errorbars linetype 1,\
'' using 1:4 title '1e4' with lines linetype 2,\
'' using 1:4:5 title '' with errorbars linetype 2,\
'' using 1:6 title '1e5' with lines linetype 3,\
'' using 1:6:7 title '' with errorbars linetype 3,\
'' using 1:8 title '0' with lines linetype 4,\
'' using 1:8:9 title '' with errorbars linetype 4
