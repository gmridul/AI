set term postscript eps color

set output 'momentum_iter.eps'

#used to make the fonts appear larger;  makes the figure smaller but keeps the fonts same size
set size 0.6, 0.6
set yrange [0:5000]
#set key right bottom
set datafile separator ","

set xlabel "Momentum Factor"
set ylabel "Number of iterations"

plot 'xormomentum.dat' using 1:2 smooth unique with lines title "Case 1"
