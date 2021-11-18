from matplotlib import pyplot
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt





x1 = []
y1 = []
for line in open('ex6channel0.txt'):
    values = line.strip('=>').split()
    x1.append(values[0])
    y1.append(values[2])


plt.bar(x1, y1)
plt.xlabel('Sample')
plt.ylabel('Count')
plt.title('Histogram')
plt.savefig('ex6channel0.png')

x2 = []
y2 = []
for line in open('ex6channel1.txt'):
    values = line.strip('=>').split()
    x2.append(values[0])
    y2.append(values[2])
plt.bar(x2, y2)
plt.xlabel('Sample')
plt.ylabel('Count')
plt.title('Histogram')
plt.savefig('ex6channel1.png')