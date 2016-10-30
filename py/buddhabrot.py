# -*- coding: utf-8 -*-
"""
Created on Sun Oct 30 02:57:38 2016

@author: alessandro
"""

import buddhabrot
import numpy as np
import matplotlib.pyplot as plt
import cv2

path = "./"
#%%
Np = 20*100000000
w = 3840#4*1024
h = 2160#4*1024

#%%
%%time
b = buddhabrot.PyBuddhabrot(Np,w,h,800,200,50,-1.125,1.125)

b.pyfill()
b.pynormalize()

#%%
img = np.reshape(b.hitcount, (w,h,3))
img = np.array(img)
img = img/65536
#%%
plt.figure(figsize=(9, 9))
plt.imshow(img)
#%%
plt.imsave(path+"d.png",img)
