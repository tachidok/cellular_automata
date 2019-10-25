import matplotlib.pyplot as plt
import numpy as np

Linewidth = 2;

# -------------------------------------------------------------------------------------
# Density vs Current plots
# -------------------------------------------------------------------------------------
fig1, ax1 = plt.subplots()

filename = 'RESLT/current_bp_p0_0.3_p1_0.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$p_0 = 0.3, p_1 = 0.00$', color='red', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp_p0_0.3_p1_0.05.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$p_0 = 0.3, p_1 = 0.05$', color='blue', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_bp_p0_0.3_p1_0.1.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$p_0 = 0.3, p_1 = 0.1$', color='green', linestyle='solid', linewidth=Linewidth)

ax1.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
#plt.xlim([0,10])
#plt.ylim([0,1])
ax1.set_xlabel(r'Density $( \rho )$')
ax1.set_ylabel(r'Current $(J)$')
ax1.set_title(r'Density vs Current $(p_0 = 0.3)$')
ax1.legend()
plt.show()

# -------------------------------------------------------------------------------------
# Density vs Velocity plots
# -------------------------------------------------------------------------------------
fig1, ax2 = plt.subplots()

filename = 'RESLT/velocity_bp_p0_0.3_p1_0.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$p_0 = 0.3, p_1 = 0.00$', color='red', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp_p0_0.3_p1_0.05.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$p_0 = 0.3, p_1 = 0.05$', color='blue', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/velocity_bp_p0_0.3_p1_0.1.dat'
density, velocity = np.loadtxt(filename, delimiter='\t', unpack=True)
ax2.plot(density, velocity, label=r'$p_0 = 0.3, p_1 = 0.1$', color='green', linestyle='solid', linewidth=Linewidth)

ax2.grid()
#ax2.xticks(np.arange(0,10,step=1))
#ax2.yticks(np.arange(0,1.1,step=0.1))
#ax2.xlim([0,10])
#ax2.ylim([0,1])
ax2.set_xlabel(r'Density $( \rho )$')
ax2.set_ylabel(r'Velocity $(v)$')
ax2.set_title(r'Density vs Velocity $(p_0 = 0.3)$')
ax2.legend()
plt.show()
