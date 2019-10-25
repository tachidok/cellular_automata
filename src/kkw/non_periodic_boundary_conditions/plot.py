import matplotlib.pyplot as plt
import numpy as np

Linewidth = 2;

# -------------------------------------------------------------------------------------
# Alpha vs Current plots
# -------------------------------------------------------------------------------------
fig1, ax1 = plt.subplots()

filename = 'RESLT/current_b0.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 0.0$', color='red', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_b0.1.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 0.1$', color='blue', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_b0.2.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 0.2$', color='green', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_b0.3.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 0.3$', color='violet', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_b0.4.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 0.4$', color='orange', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_b0.5.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 0.5$', color='cyan', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_b0.6.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 0.6$', color='yellow', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_b0.7.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 0.7$', color='pink', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_b0.8.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 0.8$', color='black', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_b0.9.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 0.9$', color='lime', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_b1.dat'
alpha, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(alpha, current, label=r'$\beta = 1.0$', color='saddlebrown', linestyle='solid', linewidth=Linewidth)

ax1.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
#plt.xlim([0,10])
#plt.ylim([0,1])
ax1.set_xlabel(r'Alpha $( \alpha )$')
ax1.set_ylabel(r'Current $(J)$')
ax1.set_title("Alpha vs Current")
ax1.legend()
plt.show()

# -------------------------------------------------------------------------------------
# Alpha vs Velocity plots
# -------------------------------------------------------------------------------------
fig2 = plt.figure(2)

filename = 'RESLT/velocity_b0.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 0.0$', color='red', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_b0.1.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 0.1$', color='blue', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_b0.2.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 0.2$', color='green', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_b0.3.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 0.3$', color='violet', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_b0.4.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 0.4$', color='orange', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_b0.5.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 0.5$', color='cyan', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_b0.6.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 0.6$', color='yellow', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_b0.7.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 0.7$', color='pink', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_b0.8.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 0.8$', color='black', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_b0.9.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 0.9$', color='lime', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_b1.dat'
alpha, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, velocity, label=r'$\beta = 1.0$', color='saddlebrown', linestyle='solid', linewidth=Linewidth)

plt.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
#plt.xlim([0,10])
#plt.ylim([0,1])
plt.xlabel(r'Alpha $( \alpha )$')
plt.ylabel(r'Velocity $(v)$')
plt.title(r"Alpha vs Velocity")
plt.legend()
plt.show()

# -------------------------------------------------------------------------------------
# Alpha vs Density plots
# -------------------------------------------------------------------------------------
fig3 = plt.figure(3)

filename = 'RESLT/density_b0.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 0.0$', color='red', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/density_b0.1.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 0.1$', color='blue', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/density_b0.2.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 0.2$', color='green', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/density_b0.3.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 0.3$', color='violet', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/density_b0.4.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 0.4$', color='orange', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/density_b0.5.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 0.5$', color='cyan', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/density_b0.6.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 0.6$', color='yellow', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/density_b0.7.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 0.7$', color='pink', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/density_b0.8.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 0.8$', color='black', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/density_b0.9.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 0.9$', color='lime', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/density_b1.dat'
alpha, density = np.loadtxt(filename, delimiter='\t', unpack=True)
plt.plot(alpha, density, label=r'$\beta = 1.0$', color='saddlebrown', linestyle='solid', linewidth=Linewidth)

plt.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
#plt.xlim([0,10])
#plt.ylim([0,1])
plt.xlabel(r'Alpha $( \alpha )$')
plt.ylabel(r'Density $(v)$')
plt.title("Alpha vs Density")
plt.legend()
plt.show()
