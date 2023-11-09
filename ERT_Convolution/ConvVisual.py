import matplotlib
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import cm

fig, ax = plt.subplots()

xData = np.loadtxt("convolvedX.dat")
yData = np.loadtxt("convolvedY.dat")
zDataRaw = np.loadtxt("convolved.dat")[:,2]
zData = np.log10(zDataRaw.reshape((xData.size, yData.size)))
Xm, Ym = np.meshgrid(yData, xData)

surf = ax.contourf(Xm, Ym, zData, cmap=cm.plasma, levels=20)
ax.set_title(r"Base-10 log of convolved data $h(x,y)$")
fig.colorbar(surf, shrink=0.5, aspect=5, label=r'$\log_{10}h(x,y)$')
fig.savefig('conv.pdf')