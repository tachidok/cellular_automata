import matplotlib.pyplot as plt
import numpy as np

Linewidth = 2;

# -------------------------------------------------------------------------------------
# Density vs Current plots
# -------------------------------------------------------------------------------------
fig1, ax1 = plt.subplots()

filename = 'RESLT/current_bp0.9.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.scatter(density, current, label=r'Current $(J)$', color='red', linestyle='solid', linewidth=Linewidth)

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

filename = 'RESLT/velocity_bp0.9.dat'
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
plt.show()
