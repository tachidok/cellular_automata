import os
import sys
import argparse
import matplotlib.pyplot as plt
import numpy as np

def main():
    Linewidth = 1
    
    # Create the parser to deal with the arguments
    parser = argparse.ArgumentParser("Plot density vs current and velocity using the files in the given folder")
        
    # Set the positional arguments
    parser.add_argument("RESLT_folder", type=str, help="The RESLT folder")
    
    # parse args
    args = parser.parse_args()
    
    # RESLT folder
    RESLT = args.RESLT_folder

    # Create two list with the break probabilities and colors to plot
    bp_list = [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1]
    bp_colors = ["red", "blue", "green", "violet", "orange", "cyan", "yellow", "pink", "black", "lime", "saddlebrown"]

    # -------------------------------------------------------------------------------------
    # Density vs Current plots
    # -------------------------------------------------------------------------------------
    fig1, ax1 = plt.subplots()
    for i in range(len(bp_list)):
        filename = RESLT + '/current_bp' + str(bp_list[i]) + '.dat'
        density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
        ax1.plot(density, current, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)
        
    ax1.grid()
    #plt.xticks(np.arange(0,10,step=1))
    #plt.yticks(np.arange(0,1.1,step=0.1))
    #plt.xlim([0,10])
    #plt.ylim([0,1])
    ax1.set_xlabel(r'Density $( \rho )$')
    ax1.set_ylabel(r'Current $(J)$')
    ax1.set_title("Density vs Current")
    ax1.legend()

    # Show the plot
    #plt.show()
    
    # -------------------------------------------------------------------------------------
    # Density vs Velocity plots
    # -------------------------------------------------------------------------------------
    fig1, ax2 = plt.subplots()

    for i in range(len(bp_list)):
        filename = RESLT + '/velocity_bp' + str(bp_list[i]) + '.dat'
        density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
        ax2.plot(density, velocity, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

    ax2.grid()
    #ax2.xticks(np.arange(0,10,step=1))
    #ax2.yticks(np.arange(0,1.1,step=0.1))
    #ax2.xlim([0,10])
    #ax2.ylim([0,1])
    ax2.set_xlabel(r'Density $( \rho )$')
    ax2.set_ylabel(r'Velocity $(v)$')
    ax2.set_title(r"Density vs Velocity")
    ax2.legend()
    
    # Show the plot
    plt.show()
    
if __name__ == '__main__':
    main()
