# -*- coding: utf-8 -*-
"""
Created on Sat Aug  6 23:31:53 2016

@author: alessandro
"""

cimport numpy as np
import numpy as np
from libcpp.vector cimport vector
from cython.operator cimport dereference as deref
from cython cimport view

cdef extern from "/home/alessandro/Documents/Buddhabrot/buddhabrot.h":
        
    cdef cppclass Buddhabrot:
        
        int width, height
        float minX, maxX, minY, maxY
        float radius
        int Ndim
        
        vector[int] hitcount
            
        int NiterationsR
        int NiterationsG 
        int NiterationsB
    
        int Niterations
        int Miterations
        
        int Npoints
    
        void  allocator(int w, int h, int d)
        float saturate(float x)
        float smoothstep(float a, float b, float x)
        int   incrementcount(float x, float y, int countr)
        float Finddublmax(int k,int w, int h)
        void  Normalizehitcount()
        void  fill()
        
        Buddhabrot() except +
        Buddhabrot(int Np, int w, int h, int NR, int NG, int NB) except +
        

    
cdef class PyBuddhabrot:
    cdef Buddhabrot c_buddhabrot      # hold a C++ instance which we're wrapping
    
    def __cinit__(self, int Np, int w, int h, int NR=800, int NG=200, int NB=50,float mY=-2.0, float MY=2.0):
        self.c_buddhabrot = Buddhabrot(Np,w,h,NR,NG,NB)
        self.c_buddhabrot.minY = mY
        self.c_buddhabrot.maxY = MY 
            
    def pyfill(self):
        return self.c_buddhabrot.fill()
    def pynormalize(self):
        return self.c_buddhabrot.Normalizehitcount()

    @property
    def Npoints(self):
        return self.c_buddhabrot.Npoints    
    @property
    def hitcount(self):
        return self.c_buddhabrot.hitcount
    @property
    def width(self):
        return self.c_buddhabrot.width
    @property
    def height(self):
        return self.c_buddhabrot.height
    @property
    def NiterationsR(self):
        return self.c_buddhabrot.NiterationsR
    @property
    def NiterationsG(self):
        return self.c_buddhabrot.NiterationsG
    @property
    def NiterationsB(self):
        return self.c_buddhabrot.NiterationsB
        
    @property
    def minX(self):
        return self.c_buddhabrot.minX
    @property
    def minY(self):
        return self.c_buddhabrot.minY
    @property
    def maxX(self):
        return self.c_buddhabrot.maxX
    @property
    def maxY(self):
        return self.c_buddhabrot.maxY
    @property
    def radius(self):
        return self.c_buddhabrot.radius