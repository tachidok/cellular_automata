#!/usr/bin/env python

import os
import sys
import argparse
import matplotlib.pyplot as plt
import numpy as np

def main():
    Linewidth = 1;
    
    # Create the parser to deal with the arguments
    parser = argparse.ArgumentParser("Plot density vs current and velocity using the files in the given folder")
    
    # Set the positional arguments
    parser.add_argument("RESLT_folder", type=str, help="The RESLT folder")
    parser.add_argument("bp", type=float, help="The break probability")
    
    # parse args
    args = parser.parse_args()

    # RESLT folder
    RESLT = args.RESLT_folder
    
    # Break probability
    bp = args.bp
    
    # -------------------------------------------------------------------------------------
    # Density vs Current plots
    # -------------------------------------------------------------------------------------
    fig1, ax1 = plt.subplots()

    filename = RESLT + '/current_bp' + str(bp) + '.dat'
    density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
    #ax1.scatter(density, current, label=r'Current $(J)$', color='red', linestyle='solid', linewidth=1)
    ax1.plot(density, current, label=r'Current $(J)$', color='red', linestyle='solid', linewidth=Linewidth)

    ax1.grid()
    #plt.xticks(np.arange(0,10,step=1))
    #plt.yticks(np.arange(0,1.1,step=0.1))
    #plt.xlim([0,10])
    #plt.ylim([0,1])
    ax1.set_xlabel(r'Density $( \rho )$')
    ax1.set_ylabel(r'Current $(J)$')
    ax1.set_title("Density vs Current")
    ax1.legend()
    #plt.show()
    
    # -------------------------------------------------------------------------------------
    # Density vs Velocity plots
    # -------------------------------------------------------------------------------------
    fig1, ax2 = plt.subplots()
    
    filename = RESLT + '/velocity_bp' + str(bp) + '.dat'
    density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
    ax2.plot(density, velocity, label=r'Velocity $(v)$', color='red', linestyle='solid', linewidth=Linewidth)
    
    ax2.grid()
    #plt.xticks(np.arange(0,10,step=1))
    #plt.yticks(np.arange(0,1.1,step=0.1))
    #plt.xlim([0,10])
    #plt.ylim([0,1])
    ax2.set_xlabel(r'Density $( \rho )$')
    ax2.set_ylabel(r'Velocity $(v)$')
    ax2.set_title(r"Density vs Velocity")
    ax2.legend()

    # Plot graphs
    plt.show()

if __name__ == '__main__':
    main()
