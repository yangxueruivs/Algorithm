# -*- coding: utf-8 -*-
"""
Created on Wed Sep 14 11:00:54 2016

@author: Yxr
"""

#DFT,FFT
import numpy as np

def naive_DFT(x):
    x = np.asarray(x, dtype=float)
    N = x.shape[0]
    n = np.arange(N)
    k = n.reshape((N,1))
    M = np.exp(-2j*np.pi*k*n/N)
    return np.dot(M,x)
    
def FFT(x):
    x = np.asarray(x,dtype=float)
    N = x.shape[0]
    
    if N%2>0:
        raise ValueError("size!")
    elif N<= 32:
        return naive_DFT(x)
    else:
        X_even = FFT(x[::2])
        X_odd = FFT(x[1::2])
        factor = np.exp(-2j*np.pi*np.arange(N)/N)
        return np.concatenate([X_even+factor[:N/2]*X_odd, X_even+factor[N/2:]*X_odd])