# -*- coding: utf-8 -*-
"""
Created on Mon Aug 22 12:40:25 2016

@author: Yxr
"""

def hashFun(k, i):
    pass

def hashInsert(T, k):
    i = 0
    while i != m:
        j = hashFun(k, i)
        if not T[j]:
            T[j] = k
            return j
        else:
            i += 1
    return "hash table overflow."
    
def hashSearch(T, k):
    i = 0
    while not T[j] and i != m:
        j = hashFun(k, i)
        if T[j] == k:
            return j
        i += 1
    return None