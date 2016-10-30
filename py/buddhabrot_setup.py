# -*- coding: utf-8 -*-
"""
Created on Sun Oct 30 02:14:40 2016

@author: alessandro
"""

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(
    name = 'Buddhabrot',
    ext_modules=[
    Extension('buddhabrot',
              sources=['buddhabrot.pyx'],
              extra_compile_args=['-std=c++11','-O3'],
              #extra_link_args=['-lgomp'],
              language='c++')
    ],
    cmdclass = {'build_ext': build_ext}
)
