from matplotlib import pyplot
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt





x = []
y = []
for line in open('ex5out.txt'):
    values = line.strip('=>').split()
    x.append((int(values[2])))
    y.append(values[0])


plt.bar(y, x)
plt.xlabel('Letter')
plt.ylabel('Count')
plt.title('Histogram')
plt.savefig('ex5out.png')