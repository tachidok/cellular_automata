#!/usr/bin/env python

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
        filename = RESLT + '/output_bp_' + str(bp_list[i]) + '.dat'
        
        headers_list = list()
            
        # Read the first line
        with open(filename) as file:
            header_line = file.readline()
            headers_list = header_line.split()
        
        # Read data
        density, velocity, current, delay, travel_time, queue_length, CO2, NOx, VOC, PM = np.loadtxt(filename, delimiter='\t', unpack=True, skiprows=1)
        
        # Density - velocity plot
        ax1.plot(density, velocity, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - current plot
        ax2.plot(density, current, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - delay plot
        ax3.plot(density, delay, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)
        
        # Density - travel time plot
        ax4.plot(density, travel_time, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - queue length plot
        ax5.plot(density, queue_length, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)
        
        # Density - CO2 plot
        ax6.plot(density, CO2, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - NOx plot
        ax7.plot(density, NOx, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - VOC plot
        ax8.plot(density, VOC, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

        # Density - PM plot
        ax9.plot(density, PM, label=rf'$bp = {bp_list[i]}$', color=bp_colors[i], linestyle='solid', linewidth=Linewidth)

    ax1.grid()
    ax1.set_xlabel(r'Density $( \rho )$')
    ax1.set_ylabel(r'Velocity $(v)$')
    ax1.set_title(r'Density vs Velocity')
    ax1.legend()

    ax2.grid()
    ax2.set_xlabel(r'Density $( \rho )$')
    ax2.set_ylabel(r'Current $(J)$')
    ax2.set_title(rf'Density vs Current')
    ax2.legend()

    ax3.grid()
    ax3.set_xlabel(r'Density $( \rho )$')
    ax3.set_ylabel(r'Delay (t)')
    ax3.set_title(rf'Density vs Delay')
    ax3.legend()
    
    ax4.grid()
    ax4.set_xlabel(r'Density $( \rho )$')
    ax4.set_ylabel(r'Travel time')
    ax4.set_title(rf'Density vs Travel time')
    ax4.legend()
    
    ax5.grid()
    ax5.set_xlabel(r'Density $( \rho )$')
    ax5.set_ylabel(r'Queue length')
    ax5.set_title(rf'Density vs Queue length')
    ax5.legend()
    
    ax6.grid()
    ax6.set_xlabel(r'Density $( \rho )$')
    ax6.set_ylabel(r'$CO_2$ (kg)')
    ax6.set_title(rf'Density vs $CO_{2}$')
    ax6.legend()
        
    ax7.grid()
    ax7.set_xlabel(r'Density $( \rho )$')
    ax7.set_ylabel(r'$NO_x$ (kg)')
    ax7.set_title(rf'Density vs $NO_x$')
    ax7.legend()
    
    ax8.grid()
    ax8.set_xlabel(r'Density $( \rho )$')
    ax8.set_ylabel(r'VOC (kg)')
    ax8.set_title(rf'Density vs VOC')
    ax8.legend()
    
    ax9.grid()
    ax9.set_xlabel(r'Density $( \rho )$')
    ax9.set_ylabel(r'PM (kg)')
    ax9.set_title(rf'Density vs PM')
    ax9.legend()
    
    plt.show()
                
if __name__ == '__main__':
    main()
