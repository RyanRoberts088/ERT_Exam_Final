#!/usr/bin/gnuplot
set terminal png size 1280, 960
set output "EDP.png"
set xlabel "Plasma Frequency (MHz)"
set ylabel "Altitude (km)"
set xrange [0:12]
set yrange [0:600]
set grid ytics lt 0 lw 1 lc rgb "#bbbbbb"
set grid xtics lt 0 lw 1 lc rgb "#bbbbbb"
set style line 1 lt 1 lw 3 pt 3 linecolor rgb "red
plot "./EDP.dat" using 2:1 title "Vertical EDP" with lines