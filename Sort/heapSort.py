# -*- coding: utf-8 -*-
"""
Created on Mon Oct 10 16:51:11 2016

@author: Yxr
"""

#heapSort
def heapify(heap,k,n):
    N=n-1
    while 2*k<N:
        j=2*k+1
        if j<N and heap[j]<heap[j+1]:
            j+=1
        if heap[k]<heap[j]:
            heap[k],heap[j] = heap[j],heap[k]
            k = j
        else:
            break

def heapSort(heap):
    n=len(heap)
    for i in range((n-1)/2, -1, -1):
        heapify(heap, i, n)
    while n-1>0:
        heap[0], heap[n-1]=heap[n-1], heap[0]
        heapify(heap, 0, n-1)
        n -= 1
    return heap