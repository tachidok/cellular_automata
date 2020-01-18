#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import argparse

EXTRA_SMALL_SIZE = 8
SMALL_SIZE = 10
MEDIUM_SIZE = 12
BIG_SIZE = 14
EXTRA_BIG_SIZE = 16

plt.rc('font', size=SMALL_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=EXTRA_BIG_SIZE)    # fontsize of the axes title
plt.rc('axes', labelsize=BIG_SIZE)    # fontsize of the x and y labels
plt.rc('xtick', labelsize=MEDIUM_SIZE)    # fontsize of the tick labels
plt.rc('ytick', labelsize=MEDIUM_SIZE)    # fontsize of the tick labels
plt.rc('legend', fontsize=EXTRA_BIG_SIZE)   # legend fontsize
plt.rc('figure', titlesize=MEDIUM_SIZE)  # fontsize of the figure title

Linewidth = 1;

def main():
        
    # Create the parser to deal with the arguments
    parser = argparse.ArgumentParser(description="Plots the time space diagram")
    
    #parser.add_argument("p0", help="The probability P_0", type=float)
    #parser.add_argument("p1", help="The probability P_1", type=float)
    #parser.add_argument("rho", help="The density", type=float)
    parser.add_argument("filename", help="The file storing the time vs space information", type=str)
    
    # parse args
    args = parser.parse_args()

    # Construct filename
    #filename = construct_filename(args.p0, args.p1, args.rho)
    filename = args.filename
    print(filename)
    
    # -------------------------------------------------------------------------------------
    # Read data to plot
    # -------------------------------------------------------------------------------------
    matrix = np.loadtxt(filename, dtype=int) # Reads # rows of the
                                             # file into lists, then
                                             # create a list of lists
                                             # with the information in
                                             # the rows
                                             
    lane_size = len(matrix[0])
    x_space = list(range(0,lane_size))
    
    # -------------------------------------------------------------------------------------
    # Time space plot
    # -------------------------------------------------------------------------------------
    fig1, ax1 = plt.subplots()
    
    # Save image to file
    #plt.imsave('my_image.png', matrix, cmap=plt.cm.binary)
    
    ax1.imshow(matrix, interpolation='none', cmap=plt.cm.binary)
    #ax1.imshow(matrix, interpolation='none')
    #ax1.set_axis_off()
    #ax1.scatter(x_space, matrix[0], color='black')
    ax1.set_xlabel(r'Space $(x)$')
    ax1.set_ylabel(r'Time $(t)$')
    
    title_string = "Time vs Space"
    ax1.set_title(title_string)
    ax1.set_xticks(np.arange(0,1600,step=200))
    #ax1.set_title(r'Time vs Space {filename}')
    #ax1.legend()
    plt.show()
    
    # Save plot to file
    #img_filename = filename + '.png'
    #fig1.savefig(img_filename)
    #fig1.savefig('my_image.png', transparent=True)
    
    plt.close()
        
if __name__ == '__main__':
    main()
    
