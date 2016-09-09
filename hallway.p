set term png
set output 'hallway1-traps.png'
set xlabel 'worst-case threshold'
set ylabel 'total accumulated reward'
set style line 1 lc rgb '#0060ad' pt 7 ps 1.5  # blue circles, point size = ps
set style line 2 lc rgb 'red' pt 1 ps 1        # red crosses, point size = ps
# set style line 3 lc rgb 'black' lt 1 lw 2      # black line, line width = lw
# f(x) = a*log(b + c*x) + d
# myencoding = 'utf8'
# a = -1
# b = 1e-8
# c = 1
# d = 1
# FIT_LIMIT = 1e-6
# fit f(x) 'hallway_avg.dat' via a, b, c, d
plot 'hallway.dat' title 'raw data' with points ls 1,\
     'hallway_avg.dat' using 1:2:3 title 'mean (+/- std. dev.)' with errorlines ls 2 
#     f(x) title 'fit of avg' with lines ls 3
     
