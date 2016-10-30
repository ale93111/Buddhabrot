# -*- coding: utf-8 -*-
"""
Created on Sun Oct 30 02:57:38 2016

@author: alessandro
"""

import buddhabrot
import numpy as np
import matplotlib.pyplot as plt

path = "./"
#%%
Np = 2000000
w = 1024
h = 1024

#%%
b = buddhabrot.PyBuddhabrot(Np=Np,w=w,h=h)
b.pyfill()

#%%
img = np.reshape(b.hitcount, (w,h,3))

#%%
plt.figure(figsize=(9, 9))
plt.imshow(img)