# -*- coding: utf-8 -*-
"""
Created on Mon Aug 29 15:55:56 2016

@author: Yxr
"""
#O(h)
def treeInsert(T,z):
    y = None
    x = T.root
    while x:
        y = x
        if z.key < x.key:
            x = x.left
        else:
            x = x.right
    z.p = y
    if not y:
        T.root = z
    elif z.key < y.key:
        y.left = z
    else:
        y.right = z
    
def treeMinimum(x):
    while x.left:
        x = x.left
    return x

def treeDelete(T,z):
    if not z.left:
        transplant(T,z,z.right)
    elif not z.right:
        transplant(T,z,z.left)
    else:
        y = treeMinimum(z.right)
        if y.p != z:
            transplant(T,y,y.right)
            y.right = z.right
            y.right.p = y
        transplant(T,z,y)
        y.left = z.left
        y.left.p = y

def transplant(T,u,v):
    if not u.p:
        T.root = v
    elif u == u.p.left:
        u.p.left = v
    else:
        u.p.right = v
    if v:
        v.p = u.p
        