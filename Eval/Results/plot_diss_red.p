set key outside
set key top right

set terminal wxt 0
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8, 16, 32, 64, 128)
set logscale x 2
set yrange[0:0.0025]
set xlabel 'Threads'
set ylabel 'Relative Counts'
set datafile separator comma
set title 'Enqueue CAS Fails'

set linetype 10 lc rgb '#4488bb' dt (5, 5, 10, 10)

set term png
set output "Pictures/diss_enq_cas_red.png"

plot 'scq_diss_enqdeq_0.50_0.00.txt' using 1:3 title '50% Enqueue 0% Prefill' with lines linetype 3,\
'' using 1:3:10 title '' with errorbars linetype 3,\
'scq_diss_enqdeq_0.50_0.90.txt' using 1:3 title '50% Enqueue 45% Prefill' with lines linetype 4,\
'' using 1:3:10 title '' with errorbars linetype 4,\
'scq_diss_pairs_0.00.txt' using 1:3 title 'Pairs 0% Prefill' with lines linetype 5,\
'' using 1:3:10 title '' with errorbars linetype 5,\
'scq_diss_pairs_0.90.txt' using 1:3 title 'Pairs 45% Prefill' with lines linetype 7,\
'' using 1:3:10 title '' with errorbars linetype 7


set terminal wxt 1
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8, 16, 32, 64, 128)
set logscale x 2
set yrange[0:0.0025]
set xlabel 'Threads'
set ylabel 'Relative Counts'
set datafile separator comma
set title 'Dequeue CAS Fails'

set term png
set output "Pictures/diss_deq_cas_red.png"

plot 'scq_diss_enqdeq_0.50_0.00.txt' using 1:7 title '50% Enqueue 0% Prefill' with lines linetype 3,\
'' using 1:7:14 title '' with errorbars linetype 3,\
'scq_diss_enqdeq_0.50_0.90.txt' using 1:7 title '50% Enqueue 45% Prefill' with lines linetype 4,\
'' using 1:7:14 title '' with errorbars linetype 4,\
'scq_diss_pairs_0.00.txt' using 1:7 title 'Pairs 0% Prefill' with lines linetype 5,\
'' using 1:7:14 title '' with errorbars linetype 5,\
'scq_diss_pairs_0.90.txt' using 1:7 title 'Pairs 45% Prefill' with lines linetype 7,\
'' using 1:7:14 title '' with errorbars linetype 7,\
'scq_diss_enqdeq_0.00_0.90.txt' using 1:7 title '100% Dequeue 45% Prefill' with lines linetype 8,\
'' using 1:7:14 title '' with errorbars linetype 8


set terminal wxt 2
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8, 16, 32, 64, 128)
set logscale x 2
set yrange[1:1.006]
set xlabel 'Threads'
set ylabel 'Relative Counts'
set datafile separator comma
set title 'Enqueue Loops'

set term png
set output "Pictures/diss_enq_loops_red.png"

plot 'scq_diss_enqdeq_0.50_0.00.txt' using 1:2 title '50% Enqueue 0% Prefill' with lines linetype 3,\
'' using 1:2:9 title '' with errorbars linetype 3,\
'scq_diss_enqdeq_0.50_0.90.txt' using 1:2 title '50% Enqueue 45% Prefill' with lines linetype 4,\
'' using 1:2:9 title '' with errorbars linetype 4,\
'scq_diss_pairs_0.00.txt' using 1:2 title 'Pairs 0% Prefill' with lines linetype 5,\
'' using 1:2:9 title '' with errorbars linetype 5,\
'scq_diss_pairs_0.90.txt' using 1:2 title 'Pairs 45% Prefill' with lines linetype 7,\
'' using 1:2:9 title '' with errorbars linetype 7

set terminal wxt 3
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8, 16, 32, 64, 128)
set logscale x 2
set yrange[1:1.006]
set xlabel 'Threads'
set ylabel 'Relative Counts'
set datafile separator comma
set title 'Dequeue Loops'

set term png
set output "Pictures/diss_deq_loops_red.png"

plot 'scq_diss_enqdeq_0.50_0.00.txt' using 1:5 title '50% Enqueue 0% Prefill' with lines linetype 3,\
'' using 1:5:12 title '' with errorbars linetype 3,\
'scq_diss_enqdeq_0.50_0.90.txt' using 1:5 title '50% Enqueue 45% Prefill' with lines linetype 4,\
'' using 1:5:12 title '' with errorbars linetype 4,\
'scq_diss_pairs_0.00.txt' using 1:5 title 'Pairs 0% Prefill' with lines linetype 5,\
'' using 1:5:12 title '' with errorbars linetype 5,\
'scq_diss_pairs_0.90.txt' using 1:5 title 'Pairs 45% Prefill' with lines linetype 7,\
'' using 1:5:12 title '' with errorbars linetype 7,\
'scq_diss_enqdeq_0.00_0.90.txt' using 1:5 title '100% Dequeue 45% Prefill' with lines linetype 8,\
'' using 1:5:12 title '' with errorbars linetype 8


set terminal wxt 4
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8, 16, 32, 64, 128)
set logscale x 2
set yrange[0:0.0018]
set xlabel 'Threads'
set ylabel 'Relative Counts'
set datafile separator comma
set title 'Enqueue Unsafe'

set term png
set output "Pictures/diss_enq_unsafe_red.png"

plot 'scq_diss_enqdeq_0.50_0.00.txt' using 1:4 title '50% Enqueue 0% Prefill' with lines linetype 3,\
'' using 1:4:11 title '' with errorbars linetype 3,\
'scq_diss_enqdeq_0.50_0.90.txt' using 1:4 title '50% Enqueue 45% Prefill' with lines linetype 4,\
'' using 1:4:11 title '' with errorbars linetype 4,\
'scq_diss_pairs_0.00.txt' using 1:4 title 'Pairs 0% Prefill' with lines linetype 5,\
'' using 1:4:11 title '' with errorbars linetype 5,\
'scq_diss_pairs_0.90.txt' using 1:4 title 'Pairs 45% Prefill' with lines linetype 7,\
'' using 1:4:11 title '' with errorbars linetype 7

set terminal wxt 5
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8, 16, 32, 64, 128)
set logscale x 2
set yrange[0:0.013]
set xlabel 'Threads'
set ylabel 'Relative Counts'
set datafile separator comma
set title 'Dequeue Old Element'

set term png
set output "Pictures/diss_deq_old_red.png"

plot 'scq_diss_enqdeq_0.50_0.00.txt' using 1:6 title '50% Enqueue 0% Prefill' with lines linetype 3,\
'' using 1:6:13 title '' with errorbars linetype 3,\
'scq_diss_enqdeq_0.50_0.90.txt' using 1:6 title '50% Enqueue 45% Prefill' with lines linetype 4,\
'' using 1:6:13 title '' with errorbars linetype 4,\
'scq_diss_pairs_0.00.txt' using 1:6 title 'Pairs 0% Prefill' with lines linetype 5,\
'' using 1:6:13 title '' with errorbars linetype 5,\
'scq_diss_pairs_0.90.txt' using 1:6 title 'Pairs 45% Prefill' with lines linetype 7,\
'' using 1:6:13 title '' with errorbars linetype 7,\
'scq_diss_enqdeq_0.00_0.90.txt' using 1:6 title '100% Dequeue 45% Prefill' with lines linetype 8,\
'' using 1:6:13 title '' with errorbars linetype 8

set terminal wxt 6
set key autotitle columnhead
set xrange[1:128]
set xtics (1,2,4,8, 16, 32, 64, 128)
set logscale x 2
set yrange[0:0.013]
set xlabel 'Threads'
set ylabel 'Relative Counts'
set datafile separator comma
set title 'Catchup Calls'

set term png
set output "Pictures/diss_deq_catchup_red.png"

set key outside
set key top right

plot 'scq_diss_enqdeq_0.50_0.00.txt' using 1:8 title '50% Enqueue 0% Prefill' with lines linetype 3,\
'' using 1:8:15 title '' with errorbars linetype 3,\
'scq_diss_enqdeq_0.50_0.90.txt' using 1:8 title '50% Enqueue 45% Prefill' with lines linetype 4,\
'' using 1:8:15 title '' with errorbars linetype 4,\
'scq_diss_pairs_0.00.txt' using 1:8 title 'Pairs 0% Prefill' with lines linetype 5,\
'' using 1:8:15 title '' with errorbars linetype 5,\
'scq_diss_pairs_0.90.txt' using 1:8 title 'Pairs 45% Prefill' with lines linetype 7,\
'' using 1:8:15 title '' with errorbars linetype 7,\
'scq_diss_enqdeq_0.00_0.90.txt' using 1:8 title '100% Dequeue 45% Prefill' with lines linetype 8,\
'' using 1:8:15 title '' with errorbars linetype 8
