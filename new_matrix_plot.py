#script to plot formfactors to check that they are correct
import matplotlib.pyplot as plt
import numpy as np
import math as m
import sys
pi = np.pi

if(len(sys.argv) == 1):

    print("please specify file to plot!")    



if(len(sys.argv) == 2):

    in_file = sys.argv[1]

    dat = np.genfromtxt(in_file, 'complex')

    #get length of BZ side
    dim_xy = len(dat)

    print(dim_xy)

    #create mesh grid for all later plots 
    x = np.linspace(-dim_xy/2.0 + 1.0, dim_xy/2.0 + 1.0, dim_xy+1)
    x = x - 0.5
    y = x
    x,y = np.meshgrid(x,y)


    #create subplot with real and imaginary part
    plt.figure("1 Matrix")
    p1=plt.subplot(211)
    p1.set_title('Dat1 - Real')
    p1=plt.pcolormesh(x, y, np.real(dat), cmap='jet')#, vmin=-2.0, vmax=4.7)
    plt.colorbar()
        
    p2=plt.subplot(212)
    p2.set_title('Dat1 - Imag')
    p2=plt.pcolormesh(x, y, np.imag(dat), cmap='jet')#, vmin=-2.0, vmax=4.7)
    plt.colorbar()

    plt.show()
