import os
import sys
import argparse
import matplotlib.pyplot as plt
import numpy as np

def main():
    Linewidth = 1
        
    # Create the parser to deal with the arguments
    parser = argparse.ArgumentParser("Plots all the associated data files with the given probability p_0")
        
    # Set the positional arguments
    parser.add_argument("RESLT_folder", type=str, help="The RESLT folder")
    parser.add_argument("p0", type=float, help="The break probability p_0", choices=[0, 0.2, 0.4, 0.6, 0.8, 1])
    
    # parse args
    args = parser.parse_args()
    
    # RESLT folder
    RESLT = args.RESLT_folder
    
    # Break probability
    bp_p0 = args.p0
    
    # Create two list with the break probabilities and colors to plot
    bp_list = []
    if abs(bp_p0 - 0.0) < 1.0e-3:
        bp_list = [0]
    elif abs(bp_p0 - 0.2) < 1.0e-3:
        bp_list = [0, 0.2]
    elif abs(bp_p0 - 0.4) < 1.0e-3:
        bp_list = [0, 0.2, 0.4]
    elif abs(bp_p0 - 0.6) < 1.0e-3:
        bp_list = [0, 0.2, 0.4, 0.6]
    elif abs(bp_p0 - 0.8) < 1.0e-3:
        bp_list = [0, 0.2, 0.4, 0.6, 0.8]
    elif abs(bp_p0 - 1.0) < 1.0e-3:
        bp_list = [0, 0.2, 0.4, 0.6, 0.8, 1]
    else:
        bp_list = []
    
    # The list with the colors to plot
    bp_colors = ["red", "blue", "green", "violet", "orange", "cyan"]
    
    # -------------------------------------------------------------------------------------
    # Density vs Current plots
    # -------------------------------------------------------------------------------------
    fig1, ax1 = plt.subplots()
    
    for i in range(len(bp_list)):
        filename = ''
        if abs(bp_p0 - 0.0) < 1.0e-3:
            filename = RESLT + '/current_bp_p0_0_p1_' + str(bp_list[i]) + '.dat'
        elif abs(bp_p0 - 1.0) < 1.0e-3:
            filename = RESLT + '/current_bp_p0_1_p1_' + str(bp_list[i]) + '.dat'
        else:
            filename = RESLT + '/current_bp_p0_' + str(bp_p0) + '_p1_' + str(bp_list[i]) + '.dat'
        
        density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
        ax1.plot(density, current, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

    ax1.grid()
    #plt.xticks(np.arange(0,10,step=1))
    #plt.yticks(np.arange(0,1.1,step=0.1))
    #plt.xlim([0,10])
    #plt.ylim([0,1])
    ax1.set_xlabel(r'Density $( \rho )$')
    ax1.set_ylabel(r'Current $(J)$')
    ax1.set_title(rf'Density vs Current (p_0 = {bp_p0})')
    ax1.legend()

    #plt.show()

    # -------------------------------------------------------------------------------------
    # Density vs Velocity plots
    # -------------------------------------------------------------------------------------
    fig1, ax2 = plt.subplots()

    for i in range(len(bp_list)):
        filename = ''
        if abs(bp_p0 - 0.0) < 1.0e-3:
            filename = RESLT + '/velocity_bp_p0_0_p1_' + str(bp_list[i]) + '.dat'
        elif abs(bp_p0 - 1.0) < 1.0e-3:
            filename = RESLT + '/velocity_bp_p0_1_p1_' + str(bp_list[i]) + '.dat'
        else:
            filename = RESLT + '/velocity_bp_p0_' + str(bp_p0) + '_p1_' + str(bp_list[i]) + '.dat'
        
        density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
        ax2.plot(density, velocity, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

    ax2.grid()
    #ax2.xticks(np.arange(0,10,step=1))
    #ax2.yticks(np.arange(0,1.1,step=0.1))
    #ax2.xlim([0,10])
    #ax2.ylim([0,1])
    ax2.set_xlabel(r'Density $( \rho )$')
    ax2.set_ylabel(r'Velocity $(v)$')
    ax2.set_title(r'Density vs Velocity (p_0 = {bp_p0})')
    ax2.legend()

    # Show all plots
    plt.show()

if __name__ == '__main__':
    main()
