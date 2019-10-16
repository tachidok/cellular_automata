import matplotlib.pyplot as plt
import numpy as np

Linewidth = 2;

# -------------------------------------------------------------------------------------
# Density vs Current plots
# -------------------------------------------------------------------------------------
fig1, ax1 = plt.subplots()

filename = 'RESLT/current_bp0.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 0.0$', color='red', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp0.1.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 0.1$', color='blue', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp0.2.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 0.2$', color='green', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp0.3.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 0.3$', color='violet', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp0.4.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 0.4$', color='orange', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp0.5.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 0.5$', color='cyan', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp0.6.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 0.6$', color='yellow', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp0.7.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 0.7$', color='pink', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp0.8.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 0.8$', color='black', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp0.9.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 0.9$', color='lime', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp1.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'$bp = 1.0$', color='saddlebrown', linestyle='solid', linewidth=Linewidth)

ax1.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
#plt.xlim([0,10])
#plt.ylim([0,1])
ax1.set_xlabel(r'Density $( \rho )$')
ax1.set_ylabel(r'Current $(J)$')
ax1.set_title("Density vs Current")
ax1.legend()
plt.show()

# -------------------------------------------------------------------------------------
# Density vs Velocity plots
# -------------------------------------------------------------------------------------
fig1, ax2 = plt.subplots()

filename = 'RESLT/velocity_bp0.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 0.0$', color='red', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp0.1.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 0.1$', color='blue', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp0.2.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 0.2$', color='green', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp0.3.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 0.3$', color='violet', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp0.4.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 0.4$', color='orange', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp0.5.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 0.5$', color='cyan', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp0.6.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 0.6$', color='yellow', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp0.7.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 0.7$', color='pink', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp0.8.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 0.8$', color='black', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp0.9.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 0.9$', color='lime', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp1.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$bp = 1.0$', color='saddlebrown', linestyle='solid', linewidth=Linewidth)

ax2.grid()
#ax2.xticks(np.arange(0,10,step=1))
#ax2.yticks(np.arange(0,1.1,step=0.1))
#ax2.xlim([0,10])
#ax2.ylim([0,1])
ax2.set_xlabel(r'Density $( \rho )$')
ax2.set_ylabel(r'Velocity $(v)$')
ax2.set_title(r"Density vs Velocity")
ax2.legend()
plt.show()
