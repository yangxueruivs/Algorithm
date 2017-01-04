# -*- coding: utf-8 -*-
"""
Created on Mon Sep 05 16:48:47 2016

@author: Yxr
"""

#BFS
def enqueue(Q,s):
    pass

def dequeue(Q):
    pass

def bfs(G,s):
    for each in G:
        each.color = 'white'
        each.d = 99999
        each.p = None
    s.color = 'grey'
    s.d = 0
    s.p = None
    Q = None
    enqueue(Q,s)
    while Q:
        u = dequeue(Q)
        for v in u.adj:
            if v.color == 'white':
                v.color = 'grey'
                v.d = u.d + 1
                v.p = u
                enqueue(Q,v)
        u.color = 'black'