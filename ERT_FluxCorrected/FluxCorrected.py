import matplotlib
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import cm
from matplotlib import animation

resData = []
initialData = []
numGridCells = 0;
with open("simInitial.dat", "r") as initialFile:
    initials = initialFile.readlines()
    for line in initials:
        numGridCells = numGridCells + 1
        splitLine = line.strip('\n').split()
        initialData.append([numGridCells, float(splitLine[0]), float(splitLine[1])])

initArr = np.array(initialData)
resData.append(initialData)

fig, ax = plt.subplots()
#ax1 = ax.plot(initArr[:,0], initArr[:,1])

with open("simResults.dat", "r") as resultsFile:
    results = resultsFile.readlines();
    resData.append([])
    frame = 1
    step = 0
    for i, line in enumerate(results):
        strippedLine = line.strip('\n')
        if (strippedLine == ""):
            frame = frame + 1
            resData.append([])
            step = 0
        else:
            step = step + 1
            splitLine = strippedLine.split()
            resData[frame].append([step, float(splitLine[0]), float(splitLine[1])])

def update(frame):
    data = np.array(resData[frame])
    ax.clear()
    ax.set_xlim(0, numGridCells)
    ax.set_ylim(0, 16)
    ax.plot(data[:,0], data[:,1])
    return ax

ani = animation.FuncAnimation(fig=fig, func=update, frames=len(resData), interval=30)
ani.save("wave.gif", writer='imagemagick', fps=10)