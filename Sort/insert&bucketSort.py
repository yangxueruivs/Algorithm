# -*- coding: utf-8 -*-
"""
Created on Thu Aug 18 11:35:19 2016

@author: Yxr
"""

def insertSort(a):
    n=len(a)
    if n<=1:
        return a
    for i in range(1,n):
        for j in range(len(a[:i])):
            if a[j] > a[i]:
                a.insert(j, a[i])
                del a[i+1]
    return a
    
def bucketSort(a):
    n, res = len(a), []
    bucket = [[] for i in range(n)]
    for i in range(n):
        bucket[int(a[i]/(1.0/n))].append(a[i])
    for j in range(n):
        bucket[j] = insertSort(bucket[j])
    for i in range(n):
        res += bucket[i]
    return res