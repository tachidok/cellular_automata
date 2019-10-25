# Line width of the axes
set border linewidth 1.5
# Line styles
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0.3   # --- blue
set style line 2 lc rgb '#dd181f' lt 1 lw 2 pt 5 ps 0.3   # --- red
set style line 3 lc rgb 'violet' lt 1 lw 2 pt 9 ps 0.3   # --- violet
set style line 4 lc rgb 'green' lt 1 lw 2 pt 11 ps 0.3   # --- green
set style line 5 lc rgb 'black' lt 1 lw 2 pt 13 ps 0.3   # --- black
set style line 6 lc rgb 'yellow' lt 1 lw 2 pt 13 ps 0.3   # --- yellow
set style line 7 lc rgb 'cyan' lt 1 lw 2 pt 13 ps 0.3   # --- cyan

# Legend
set key top right

set grid

#set terminal wxt 0 size 640,480 enhanced font 'Verdana,10' persist
set terminal pngcairo size 640,480 enhanced font 'Verdana,10'
set output 'Current.png'

#set xrange [0:500]
#set yrange [0:40]
set xlabel 'Density ({/Symbol r})'
set ylabel 'Current (J)'
set tics scale 0.75

plot 'current_0.0.dat' using 1:2 title 'Break probability 0' w linespoints ls 1, \
     'current_0.2.dat' using 1:2 title 'Break probability 0.2' w linespoints ls 2, \
     'current_0.4.dat' using 1:2 title 'Break probability 0.4' w linespoints ls 3, \
     'current_0.6.dat' using 1:2 title 'Break probability 0.6' w linespoints ls 4, \
     'current_0.8.dat' using 1:2 title 'Break probability 0.8' w linespoints ls 5, \
     'current_1.0.dat' using 1:2 title 'Break probability 1.0' w linespoints ls 6

#set terminal wxt 1 size 640,480 enhanced font 'Verdana,10' persist
set terminal pngcairo size 640,480 enhanced font 'Verdana,10'
set output 'Velocity.png'

#set xrange [0:500]
#set yrange [0:40]
set xlabel 'Density ({/Symbol r})'
set ylabel 'Velocity (v)'
set tics scale 0.75

plot 'velocity_0.0.dat' using 1:2 title 'Break probability 0' w linespoints ls 1, \
     'velocity_0.2.dat' using 1:2 title 'Break probability 0.2' w linespoints ls 2, \
     'velocity_0.4.dat' using 1:2 title 'Break probability 0.4' w linespoints ls 3, \
     'velocity_0.6.dat' using 1:2 title 'Break probability 0.6' w linespoints ls 4, \
     'velocity_0.8.dat' using 1:2 title 'Break probability 0.8' w linespoints ls 5, \
     'velocity_1.0.dat' using 1:2 title 'Break probability 1.0' w linespoints ls 6

