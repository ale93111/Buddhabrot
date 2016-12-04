# -*- coding: utf-8 -*-
"""
Created on Sun Dec  4 21:55:16 2016

@author: alessandro
"""

import numpy as np
import matplotlib.pyplot as plt
import cv2

path = "./Documenti/Buddhabrot/"

def load(name):
    a = cv2.imread(path+name)
    b = a.copy()
    b[:,:,0] = a[:,:,2]
    b[:,:,2] = a[:,:,0] 
    
    return b
#%%
b = []
for i in range(3):
    b.append(load("b"+str(i)+".png"))
    
b = np.array(b)

plt.imshow(b[0])
#%%
b_n = cv2.fastNlMeansDenoisingColoredMulti(b, imgToDenoiseIndex=1, temporalWindowSize=1, h=3, templateWindowSize=7, searchWindowSize=21)
#%%
plt.figure(figsize=(9, 9))
plt.imshow(b_n)
#%%
plt.imsave(path+"b_n.png",b_n)