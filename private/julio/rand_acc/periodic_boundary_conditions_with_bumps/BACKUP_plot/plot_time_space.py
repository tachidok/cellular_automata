import numpy as np
import matplotlib.pyplot as plt
import argparse

Linewidth = 1;

def construct_filename(p0, p1, rho):
    # Construct filename
    filename = 'RESLT/current_bp_p0_'
    if p0 == 0:
        filename = filename + '0'
    else:
        filename = filename + str(p0)

    filename = filename + '_p1_'
    if p1 == 0:
        filename = filename + '0'
    else:
        filename = filename + str(p1)

    filename = filename + '_rho_'
    if rho == 0:
        filename = filename + '0'
    else:
        filename = filename + str(rho)

    filename = filename + '.dat'
    return filename

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
    ax1.set_xlabel(r'Space $(s)$')
    ax1.set_ylabel(r'Time $(t)$')
    
    title_string = "Time vs Space : " + filename
    ax1.set_title(title_string)
    #ax1.set_title(r'Time vs Space {filename}')
    #ax1.legend()
    plt.show()
    
    # Save plot to file
    fig1.savefig('my_image.png')
    #fig1.savefig('my_image.png', transparent=True)
    
    plt.close()
        
if __name__ == '__main__':
    main()
    
