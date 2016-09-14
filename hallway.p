set term png
set output 'hallway1-traps.png'
set xlabel 'worst-case threshold'
set xrange [-100:6000]
set ylabel 'total accumulated reward'
set style line 1 lc rgb '#0060ad' pt 7 ps 1.5  # blue circles, point size = ps
set style line 2 lc rgb 'red' pt 1 ps 1        # red crosses, point size = ps
plot 'hallway.dat' title 'raw data' with points ls 1,\
     'hallway_avg.dat' using 1:2:3 title 'mean (+/- std. dev.)' with errorbars ls 2 
