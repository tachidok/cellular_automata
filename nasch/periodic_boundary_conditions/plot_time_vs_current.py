import matplotlib.pyplot as plt
import numpy as np

Linewidth = 2;
# Density value 
rho = 0.8

# -------------------------------------------------------------------------------------
# Density vs Current plots
# -------------------------------------------------------------------------------------
fig1, ax1 = plt.subplots()

filename = 'RESLT/current_vs_time_bp_0_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 0.0$', color='red', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_vs_time_bp_0.1_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 0.1$', color='blue', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_vs_time_bp_0.2_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 0.2$', color='green', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_vs_time_bp_0.3_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 0.3$', color='violet', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_vs_time_bp_0.4_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 0.4$', color='orange', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_vs_time_bp_0.5_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 0.5$', color='cyan', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_vs_time_bp_0.6_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 0.6$', color='yellow', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_vs_time_bp_0.7_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 0.7$', color='pink', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_vs_time_bp_0.8_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 0.8$', color='black', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_vs_time_bp_0.9_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 0.9$', color='lime', linestyle='solid', linewidth=Linewidth)

filename = 'RESLT/current_vs_time_bp_1_rho_' + str(rho) + '.dat'
density, current = np.loadtxt(filename, delimiter='\t', unpack=True)
ax1.plot(density, current, label=r'$bp = 1.0$', color='saddlebrown', linestyle='solid', linewidth=Linewidth)

ax1.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
#plt.xlim([0,10])
#plt.ylim([0,1])
ax1.set_xlabel(r'Time (t)')
ax1.set_ylabel(r'Current $(J)$')
ax1.set_title("Time vs Current")
ax1.legend()
plt.show()
