import os
import sys
import argparse
import matplotlib.pyplot as plt
import numpy as np

from celluloid import Camera # Used to ease the animation generation process

import csv # to read and write comma separated values (csv)

EXTRA_SMALL_SIZE = 10
SMALL_SIZE = 12
MEDIUM_SIZE = 14
BIG_SIZE = 16
EXTRA_BIG_SIZE = 18

plt.rc('font', size=SMALL_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=EXTRA_BIG_SIZE)    # fontsize of the axes title
plt.rc('axes', labelsize=BIG_SIZE)    # fontsize of the x and y labels
plt.rc('xtick', labelsize=BIG_SIZE)    # fontsize of the tick labels
plt.rc('ytick', labelsize=BIG_SIZE)    # fontsize of the tick labels
plt.rc('legend', fontsize=EXTRA_BIG_SIZE)   # legend fontsize
plt.rc('figure', titlesize=MEDIUM_SIZE)  # fontsize of the figure title

def generate_gif(file_name, initial_index, final_index, output_filename):

    fig = plt.figure()
    #ax = plt.axes()
    #ax = plt.axes(xlim=(0,10),ylim=(0,10))
    camera = Camera(fig)
    
    for index in range(initial_index, final_index + 1):

        print(f"Visiting file: {index}")
        # An empty matrix
        matrix = []

        index_string = '{:05}'.format(index)
        with open(file_name+index_string+'.dat') as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
        
            for row in csv_reader:

                # Get the number of columns
                ncols = len(row)
                #print(f"The number of columns is: {nrows}")
                
                # Convert from 
                for i in range(ncols):
                    row[i]=float(row[i])
                    #print(f'\t{row[0]} {row[1]} {row[2]}')
                    
                matrix.append(row)
                
        # Plot with reversed color map
        #plt.imshow(matrix, cmap='hot', interpolation='nearest')
        #plt.imshow(matrix, cmap='hot_r', interpolation='nearest')
        plt.imshow(matrix, cmap='gray_r', interpolation='nearest')
        
        #plt.title("Static field");
        #plt.show()
        camera.snap() # Use this instead of plt.show()
        # Clear an axis
        #plt.cla()
        # Clear entiere figure
        #plt.clf()

    print("Generating 'gif' file")
    animation = camera.animate(interval = 200, repeat = True, repeat_delay = 500)
    animation.save(output_filename+'.gif', writer='imagemagick')
    
def plot_field(file_name):
    
    # An empty matrix
    matrix = []
    
    with open(file_name+'00000.dat') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        
        for row in csv_reader:

            # Get the number of columns
            nrows = len(row)
            #print(f"The number of columns is: {nrows}")

            # Convert from 
            for i in range(nrows):
                row[i]=float(row[i])
            #print(f'\t{row[0]} {row[1]} {row[2]}')

            matrix.append(row)
            
    #print(matrix)
    
    # Plot with reversed color map
    #plt.imshow(matrix, cmap='hot', interpolation='nearest')
    plt.imshow(matrix, cmap='hot_r', interpolation='nearest')
    #plt.imshow(matrix, cmap='gray_r', interpolation='nearest')

    #plt.title("Static field");
    plt.show()
    
def main():
    
    # Create the parser to deal with the arguments
    parser = argparse.ArgumentParser("Plot evacuation dynamics")
    
    # Set the positional arguments
    parser.add_argument('--RESLT_PATH', dest='RESLT_PATH', type=str, required=True, help='Specify the path where the result files are stored')
    
    # Set the positional arguments
    parser.add_argument('--initial_index', dest='initial_index', type=int, required=True, help='Specify the index of the file that should be used as the initial position of the animation')
    
    parser.add_argument('--final_index', dest='final_index', type=int, required=True, help='Specify the index of the file that should be used as the final position of the animation')
    
    # parse args
    args = parser.parse_args()
    
    # RESLT folder
    RESLT = args.RESLT_PATH

    # Initial and final indexes
    initial_index = args.initial_index
    final_index = args.final_index

    print(f"Path to folder: {RESLT}")
    
    plot_field(RESLT + "static_field_")
    #plot_field(RESLT + "dynamic_field_")
    #plot_field(RESLT + "occupancy_matrix_")
    plot_field(RESLT + "obstacle_matrix_")

    #generate_gif(RESLT + "dynamic_field_")
    generate_gif(RESLT + "occupancy_matrix_", initial_index, final_index, "occupation")

if __name__ == '__main__':
    main()
