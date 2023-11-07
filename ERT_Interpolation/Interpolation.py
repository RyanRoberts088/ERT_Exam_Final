import math

import numpy as np
from scipy import interpolate
from scipy.spatial import Delaunay
import matplotlib.pyplot as plt

class Interpolate2DData:

    def __init__(self, data, xrng, yrng, rows, cols):
        self.data = data
        self.xrng = xrng
        self.yrng = yrng
        self.rows = rows
        self.cols = cols

    def MakePlotData(self):
        x = np.linspace(self.xrng[0], self.xrng[1], self.cols)
        y = np.linspace(self.yrng[0], self.yrng[1], self.rows)

        xx, yy = np.meshgrid(x, y)
        tri = Delaunay(self.data[:,:2])
        newData = []

        xMin = self.xrng[0] - 1
        xMax = self.xrng[1] + 1
        yMin = self.yrng[0] - 1
        yMax = self.yrng[1] + 1

        for i, neighs in enumerate(tri.neighbors):
            cur_s = self.data[tri.simplices[i]]
            for j, neigh in enumerate(neighs):
                if (neigh == -1):
                    cur_p = cur_s[j]
                    for k in range(len(neighs)):
                        if (j != k):
                            dxyz = cur_s[k] - cur_p
                            m = dxyz[1] / dxyz[0]

                            dx1 = xMin - cur_p[0]
                            dx2 = xMax - cur_p[0]
                            dy1 = yMin - cur_p[1]
                            dy2 = yMax - cur_p[1]
                            x1 = 0.0
                            y1 = 0.0
                            if (dx1 / dxyz[0] > 0):
                                y1 = m * dx1 + cur_p[1]
                                if (y1 >= yMin and y1 <= yMax):
                                    x1 = xMin
                                else:
                                    if (dy1 / dxyz[1] > 0):
                                        x1 = dy1 / m + cur_p[0]
                                        y1 = yMin
                                    else:
                                        x1 = dy2 / m + cur_p[0]
                                        y1 = yMax
                            else:
                                y1 = m * dx2 + cur_p[1]
                                if (y1 >= yMin and y1 <= yMax):
                                    x1 = xMax
                                else:
                                    if (dy1 / dxyz[1] > 0):
                                        x1 = dy1 / m + cur_p[0]
                                        y1 = yMin
                                    else:
                                        x1 = dy2 / m + cur_p[0]
                                        y1 = yMax

                            dyNew = y1 - cur_p[1]
                            dxNew = x1 - cur_p[0]
                            newZ = cur_p[2] + dxyz[2] * math.sqrt(dyNew * dyNew + dxNew * dxNew) \
                                   / math.sqrt(dxyz[0] * dxyz[0] + dxyz[1] * dxyz[1])
                            newData.append([x1, y1, newZ])

        self.data = np.append(self.data, np.array(newData), axis=0)
        self.plotData = interpolate.griddata(self.data[:,:2], self.data[:,2], (xx, yy), method='linear')

data = np.loadtxt("data.txt")
params = np.loadtxt("params.txt")

intData = Interpolate2DData(data, [params[0], params[1]], [params[2], params[3]], int(params[4]), int(params[5]))
intData.MakePlotData()

plt.imshow(intData.plotData, extent=(params[0],params[1],params[2],params[3]), cmap = plt.cm.plasma)
plt.colorbar()
plt.plot(data[:,0], data[:,1], "ok", label = "Data", markersize = 3)

plt.xlim(params[0], params[1])
plt.ylim(params[2], params[3])

plt.savefig('inter.pdf')