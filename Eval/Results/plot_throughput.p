set key top right
set key inside
set terminal wxt 0
set title "Dequeues on Empty Queue"
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8,16,32,64,128)
set logscale x 2
set yrange[0:3500]
set xlabel 'Threads'
set ylabel 'Mops/sec'
set datafile separator comma

set term png
set output "Pictures/tp_deq_empty.png"

plot 'enqdeq_0.00_0.00.txt' using 1:2 title 'SCQ' with lines linetype 1,\
'' using 1:2:3 title '' with errorbars linetype 1,\
'' using 1:4 title 'SCQP' with lines linetype 2,\
'' using 1:4:5 title '' with errorbars linetype 2,\
'' using 1:6 title 'BLQ1' with lines linetype 3,\
'' using 1:6:7 title '' with errorbars linetype 3,\
'' using 1:8 title 'BLQ2' with lines linetype 4,\
'' using 1:8:9 title '' with errorbars linetype 4


set terminal wxt 1
set title "Pairs on Empty Queue"
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8,16,32,64,128)
set logscale x 2
set yrange[0:60]
set xlabel 'Threads'
set ylabel 'Mops/sec'
set datafile separator comma

set term png
set output "Pictures/tp_pairs_empty.png"

plot 'pairs_0.50_0.00.txt' using 1:2 title 'SCQ' with lines linetype 1,\
'' using 1:2:3 title '' with errorbars linetype 1,\
'' using 1:4 title 'SCQP' with lines linetype 2,\
'' using 1:4:5 title '' with errorbars linetype 2,\
'' using 1:6 title 'BLQ1' with lines linetype 3,\
'' using 1:6:7 title '' with errorbars linetype 3,\
'' using 1:8 title 'BLQ2' with lines linetype 4,\
'' using 1:8:9 title '' with errorbars linetype 4

set terminal wxt 2
set title "Pairs on 45% Prefilled Queue"
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8,16,32,64,128)
set logscale x 2
set yrange[0:60]
set xlabel 'Threads'
set ylabel 'Mops/sec'
set datafile separator comma

set term png
set output "Pictures/tp_pairs_90.png"

plot 'pairs_0.50_0.90.txt' using 1:2 title 'SCQ' with lines linetype 1,\
'' using 1:2:3 title '' with errorbars linetype 1,\
'' using 1:4 title 'SCQP' with lines linetype 2,\
'' using 1:4:5 title '' with errorbars linetype 2,\
'' using 1:6 title 'BLQ1' with lines linetype 3,\
'' using 1:6:7 title '' with errorbars linetype 3,\
'' using 1:8 title 'BLQ2' with lines linetype 4,\
'' using 1:8:9 title '' with errorbars linetype 4

set terminal wxt 3
set title "50% Enqueues on 45% Prefilled Queue"
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8,16,32,64,128)
set logscale x 2
set yrange[0:60]
set xlabel 'Threads'
set ylabel 'Mops/sec'
set datafile separator comma

set term png
set output "Pictures/tp_50enq_90.png"

plot 'enqdeq_0.50_0.90.txt' using 1:2 title 'SCQ' with lines linetype 1,\
'' using 1:2:3 title '' with errorbars linetype 1,\
'' using 1:4 title 'SCQP' with lines linetype 2,\
'' using 1:4:5 title '' with errorbars linetype 2,\
'' using 1:6 title 'BLQ1' with lines linetype 3,\
'' using 1:6:7 title '' with errorbars linetype 3,\
'' using 1:8 title 'BLQ2' with lines linetype 4,\
'' using 1:8:9 title '' with errorbars linetype 4

set terminal wxt 4
set title "50% Enqueues on Empty Queue"
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8,16,32,64,128)
set logscale x 2
set yrange[0:60]
set xlabel 'Threads'
set ylabel 'Mops/sec'
set datafile separator comma

set term png
set output "Pictures/tp_50enq_empty.png"

plot 'enqdeq_0.50_0.00.txt' using 1:2 title 'SCQ' with lines linetype 1,\
'' using 1:2:3 title '' with errorbars linetype 1,\
'' using 1:4 title 'SCQP' with lines linetype 2,\
'' using 1:4:5 title '' with errorbars linetype 2,\
'' using 1:6 title 'BLQ1' with lines linetype 3,\
'' using 1:6:7 title '' with errorbars linetype 3,\
'' using 1:8 title 'BLQ2' with lines linetype 4,\
'' using 1:8:9 title '' with errorbars linetype 4

set terminal wxt 5
set title "99% Dequeues on Empty Queue"
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8,16,32,64,128)
set logscale x 2
set yrange[0:60]
set xlabel 'Threads'
set ylabel 'Mops/sec'
set datafile separator comma

set term png
set output "Pictures/tp_99deq_empty.png"

plot 'enqdeq_0.01_0.00.txt' using 1:2 title 'SCQ' with lines linetype 1,\
'' using 1:2:3 title '' with errorbars linetype 1,\
'' using 1:4 title 'SCQP' with lines linetype 2,\
'' using 1:4:5 title '' with errorbars linetype 2,\
'' using 1:6 title 'BLQ1' with lines linetype 3,\
'' using 1:6:7 title '' with errorbars linetype 3,\
'' using 1:8 title 'BLQ2' with lines linetype 4,\
'' using 1:8:9 title '' with errorbars linetype 4

set terminal wxt 6
set title "30% Enqueues on Empty Queue"
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8,16,32,64,128)
set logscale x 2
set yrange[0:40]
set xlabel 'Threads'
set ylabel 'Mops/sec'
set datafile separator comma

set term png
set output "Pictures/tp_30enq_empty.png"

plot 'enqdeq_0.30_0.00.txt' using 1:2 title 'SCQ' with lines linetype 1,\
'' using 1:2:3 title '' with errorbars linetype 1,\
'' using 1:4 title 'SCQP' with lines linetype 2,\
'' using 1:4:5 title '' with errorbars linetype 2,\
'' using 1:6 title 'BLQ1' with lines linetype 3,\
'' using 1:6:7 title '' with errorbars linetype 3,\
'' using 1:8 title 'BLQ2' with lines linetype 4,\
'' using 1:8:9 title '' with errorbars linetype 4

set terminal wxt 7
set title "30% Enqueues on 45% Prefilled Queue"
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8,16,32,64,128)
set logscale x 2
set yrange[0:40]
set xlabel 'Threads'
set ylabel 'Mops/sec'
set datafile separator comma

set term png
set output "Pictures/tp_30enq_90.png"

plot 'enqdeq_0.30_0.90.txt' using 1:2 title 'SCQ' with lines linetype 1,\
'' using 1:2:3 title '' with errorbars linetype 1,\
'' using 1:4 title 'SCQP' with lines linetype 2,\
'' using 1:4:5 title '' with errorbars linetype 2,\
'' using 1:6 title 'BLQ1' with lines linetype 3,\
'' using 1:6:7 title '' with errorbars linetype 3,\
'' using 1:8 title 'BLQ2' with lines linetype 4,\
'' using 1:8:9 title '' with errorbars linetype 4
