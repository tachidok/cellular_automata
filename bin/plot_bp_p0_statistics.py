#!/usr/bin/env python

import os
import sys
import argparse
import matplotlib.pyplot as plt
import numpy as np

def main():
    Linewidth = 1
        
    # Create the parser to deal with the arguments
    parser = argparse.ArgumentParser("Plots data associated with the given probability p_0")
    
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
    # Create axis objects
    # -------------------------------------------------------------------------------------
    fig1, ax1 = plt.subplots()
    fig1, ax2 = plt.subplots()
    fig1, ax3 = plt.subplots()
    fig1, ax4 = plt.subplots()
    fig1, ax5 = plt.subplots()
    fig1, ax6 = plt.subplots()
    fig1, ax7 = plt.subplots()
    fig1, ax8 = plt.subplots()
    fig1, ax9 = plt.subplots()
    
    for i in range(len(bp_list)):
        filename = ''
        if abs(bp_p0 - 0.0) < 1.0e-3:
            filename = RESLT + '/output_bp_p0_0_p1_' + str(bp_list[i]) + '.dat'
        elif abs(bp_p0 - 1.0) < 1.0e-3:
            filename = RESLT + '/output_bp_p0_1_p1_' + str(bp_list[i]) + '.dat'
        else:
            filename = RESLT + '/output_bp_p0_' + str(bp_p0) + '_p1_' + str(bp_list[i]) + '.dat'
            
        headers_list = list()
            
        # Read the first line
        with open(filename) as file:
            header_line = file.readline()
            headers_list = header_line.split()
        
        # Read data
        density, velocity, current, delay, travel_time, queue_length, CO2, NOx, VOC, PM = np.loadtxt(filename, delimiter='\t', unpack=True, skiprows=1)
        
        # Density - velocity plot
        ax1.plot(density, velocity, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - current plot
        ax2.plot(density, current, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - delay plot
        ax3.plot(density, delay, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)
        
        # Density - travel time plot
        ax4.plot(density, travel_time, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - queue length plot
        ax5.plot(density, queue_length, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)
        
        # Density - CO2 plot
        ax6.plot(density, CO2, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - NOx plot
        ax7.plot(density, NOx, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - VOC plot
        ax8.plot(density, VOC, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - PM plot
        ax9.plot(density, PM, label=rf'$p_0 = {bp_p0}, p_1 = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

    ax1.grid()
    ax1.set_xlabel(r'Density $( \rho )$')
    ax1.set_ylabel(r'Velocity $(v)$')
    ax1.set_title(r'Density vs Velocity (p_0 = {bp_p0})')
    ax1.legend()

    ax2.grid()
    ax2.set_xlabel(r'Density $( \rho )$')
    ax2.set_ylabel(r'Current $(J)$')
    ax2.set_title(rf'Density vs Current (p_0 = {bp_p0})')
    ax2.legend()

    ax3.grid()
    ax3.set_xlabel(r'Density $( \rho )$')
    ax3.set_ylabel(r'Delay')
    ax3.set_title(rf'Density vs Delay (p_0 = {bp_p0})')
    ax3.legend()
    
    ax4.grid()
    ax4.set_xlabel(r'Density $( \rho )$')
    ax4.set_ylabel(r'Travel time')
    ax4.set_title(rf'Density vs Travel time (p_0 = {bp_p0})')
    ax4.legend()
    
    ax5.grid()
    ax5.set_xlabel(r'Density $( \rho )$')
    ax5.set_ylabel(r'Queue length')
    ax5.set_title(rf'Density vs Queue length (p_0 = {bp_p0})')
    ax5.legend()
    
    ax6.grid()
    ax6.set_xlabel(r'Density $( \rho )$')
    ax6.set_ylabel(r'$CO_2$')
    ax6.set_title(rf'Density vs $CO_{2}$ (p_0 = {bp_p0})')
    ax6.legend()
        
    ax7.grid()
    ax7.set_xlabel(r'Density $( \rho )$')
    ax7.set_ylabel(r'$NO_x$')
    ax7.set_title(rf'Density vs $NO_x$ (p_0 = {bp_p0})')
    ax7.legend()
    
    ax8.grid()
    ax8.set_xlabel(r'Density $( \rho )$')
    ax8.set_ylabel(r'VOC')
    ax8.set_title(rf'Density vs VOC (p_0 = {bp_p0})')
    ax8.legend()
    
    ax9.grid()
    ax9.set_xlabel(r'Density $( \rho )$')
    ax9.set_ylabel(r'PM')
    ax9.set_title(rf'Density vs PM (p_0 = {bp_p0})')
    ax9.legend()
    
    plt.show()

if __name__ == '__main__':
    main()
