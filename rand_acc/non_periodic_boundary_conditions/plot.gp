# Line width of the axes
set border linewidth 1.5
# Line styles
set style line 1 lc rgb 'red' lt 1 lw 2 pt 5 ps 0.3   # --- red
set style line 2 lc rgb 'blue' lt 1 lw 2 pt 7 ps 0.3   # --- blue
set style line 3 lc rgb 'green' lt 1 lw 2 pt 9 ps 0.3   # --- green
set style line 4 lc rgb 'violet' lt 1 lw 2 pt 11 ps 0.3   # --- violet
set style line 5 lc rgb 'orange' lt 1 lw 2 pt 13 ps 0.3   # --- orange
set style line 6 lc rgb 'cyan' lt 1 lw 2 pt 15 ps 0.3   # --- cyan
set style line 7 lc rgb 'yellow' lt 1 lw 2 pt 3 ps 0.3   # --- yellow
set style line 8 lc rgb 'pink' lt 1 lw 2 pt 2 ps 0.3   # --- pink
set style line 9 lc rgb 'black' lt 1 lw 2 pt 1 ps 0.3   # --- black
set style line 10 lc rgb 'dark-green' lt 1 lw 2 pt 64 ps 0.3   # --- dark-green
set style line 11 lc rgb 'brown' lt 1 lw 2 pt 65 ps 0.3   # --- brown

# Legend
set key top left

set grid

#set terminal wxt 0 size 640,480 enhanced font 'Verdana,10' persist
set terminal pngcairo size 640,480 enhanced font 'Verdana,10'
set output 'alpha_vs_current.png'

#set xrange [0:500]
#set yrange [0:40]
set xlabel 'Alpha ({/Symbol a})'
set ylabel 'Current (J)'
set tics scale 0.75

plot 'RESLT/current_b0.dat' using 1:2 title '{/Symbol b} = 0.0' w linespoints ls 1, \
     'RESLT/current_b0.1.dat' using 1:2 title '{/Symbol b} = 0.1' w linespoints ls 2, \
     'RESLT/current_b0.2.dat' using 1:2 title '{/Symbol b} = 0.2' w linespoints ls 3, \
     'RESLT/current_b0.3.dat' using 1:2 title '{/Symbol b} = 0.3' w linespoints ls 4, \
     'RESLT/current_b0.4.dat' using 1:2 title '{/Symbol b} = 0.4' w linespoints ls 5, \
     'RESLT/current_b0.5.dat' using 1:2 title '{/Symbol b} = 0.5' w linespoints ls 6, \
     'RESLT/current_b0.6.dat' using 1:2 title '{/Symbol b} = 0.6' w linespoints ls 7, \
     'RESLT/current_b0.7.dat' using 1:2 title '{/Symbol b} = 0.7' w linespoints ls 8, \
     'RESLT/current_b0.8.dat' using 1:2 title '{/Symbol b} = 0.8' w linespoints ls 9, \
     'RESLT/current_b0.9.dat' using 1:2 title '{/Symbol b} = 0.9' w linespoints ls 10, \
     'RESLT/current_b1.dat' using 1:2 title '{/Symbol b} = 1.0' w linespoints ls 11

#set terminal wxt 1 size 640,480 enhanced font 'Verdana,10' persist
set terminal pngcairo size 640,480 enhanced font 'Verdana,10'
set output 'alpha_vs_velocity.png'

set key top right
#set xrange [0:500]
#set yrange [0:40]
set xlabel 'Alpha ({/Symbol a})'
set ylabel 'Velocity (v)'
set tics scale 0.75

plot 'RESLT/velocity_b0.dat' using 1:2 title '{/Symbol b} = 0.0' w linespoints ls 1, \
     'RESLT/velocity_b0.1.dat' using 1:2 title '{/Symbol b} = 0.1' w linespoints ls 2, \
     'RESLT/velocity_b0.2.dat' using 1:2 title '{/Symbol b} = 0.2' w linespoints ls 3, \
     'RESLT/velocity_b0.3.dat' using 1:2 title '{/Symbol b} = 0.3' w linespoints ls 4, \
     'RESLT/velocity_b0.4.dat' using 1:2 title '{/Symbol b} = 0.4' w linespoints ls 5, \
     'RESLT/velocity_b0.5.dat' using 1:2 title '{/Symbol b} = 0.5' w linespoints ls 6, \
     'RESLT/velocity_b0.6.dat' using 1:2 title '{/Symbol b} = 0.6' w linespoints ls 7, \
     'RESLT/velocity_b0.7.dat' using 1:2 title '{/Symbol b} = 0.7' w linespoints ls 8, \
     'RESLT/velocity_b0.8.dat' using 1:2 title '{/Symbol b} = 0.8' w linespoints ls 9, \
     'RESLT/velocity_b0.9.dat' using 1:2 title '{/Symbol b} = 0.9' w linespoints ls 10, \
     'RESLT/velocity_b1.dat' using 1:2 title '{/Symbol b} = 1.0' w linespoints ls 11

#set terminal wxt 2 size 640,480 enhanced font 'Verdana,10' persist
set terminal pngcairo size 640,480 enhanced font 'Verdana,10'
set output 'alpha_vs_density.png'

set key top left
#set xrange [0:500]
#set yrange [0:40]
set xlabel 'Alpha ({/Symbol a})'
set ylabel 'Density ({/Symbol r})'
set tics scale 0.75

plot 'RESLT/density_b0.dat' using 1:2 title '{/Symbol b} = 0.0' w linespoints ls 1, \
     'RESLT/density_b0.1.dat' using 1:2 title '{/Symbol b} = 0.1' w linespoints ls 2, \
     'RESLT/density_b0.2.dat' using 1:2 title '{/Symbol b} = 0.2' w linespoints ls 3, \
     'RESLT/density_b0.3.dat' using 1:2 title '{/Symbol b} = 0.3' w linespoints ls 4, \
     'RESLT/density_b0.4.dat' using 1:2 title '{/Symbol b} = 0.4' w linespoints ls 5, \
     'RESLT/density_b0.5.dat' using 1:2 title '{/Symbol b} = 0.5' w linespoints ls 6, \
     'RESLT/density_b0.6.dat' using 1:2 title '{/Symbol b} = 0.6' w linespoints ls 7, \
     'RESLT/density_b0.7.dat' using 1:2 title '{/Symbol b} = 0.7' w linespoints ls 8, \
     'RESLT/density_b0.8.dat' using 1:2 title '{/Symbol b} = 0.8' w linespoints ls 9, \
     'RESLT/density_b0.9.dat' using 1:2 title '{/Symbol b} = 0.9' w linespoints ls 10, \
     'RESLT/density_b1.dat' using 1:2 title '{/Symbol b} = 1.0' w linespoints ls 11

