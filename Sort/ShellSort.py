# -*- coding: utf-8 -*-
"""
Created on Fri Sep 23 13:19:35 2016

@author: Yxr
"""

#Shell Sort
def shell(l):
    n = len(l)
    length = n/4
    for gap in range(n/2, 0, -length):
        for i in range(gap, n):
            for j in range(i-gap,-1,-gap):
                if l[j] < l[j + gap]:
                    break
                l[j], l[j + gap] = l[j + gap], l[j] 
        length /= 2
    return l