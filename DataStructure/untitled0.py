# -*- coding: utf-8 -*-
"""
Created on Tue Nov 01 18:30:06 2016

@author: Yxr
"""

import numpy as np
import matplotlib.pyplot as plt

plt.axis([0, 100, 0, 1])
plt.ion()

for i in range(5):
    y = np.random.random()
    plt.scatter(i, y)
    plt.pause(1)