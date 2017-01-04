# -*- coding: utf-8 -*-
"""
Created on Fri Sep 23 20:41:44 2016

@author: Yxr
"""

import sys
  
 
num, dic = int(sys.stdin.readline().strip()), {'E':0, 'F':0, 'G':0, 'H':0, 'I':0, 'N':0, 'O':0, 'R':0, 'S':0, 'T':0, 'U':0, 'V':0, 'W':0, 'X':0, 'Z':0}
dic2 = {0:0, 1:0, 2:0, 3:0, 4:0, 5:0, 6:0, 7:0, 8:0, 9:0}
ss = []
for i in range(num):
    s = sys.stdin.readline().strip()
    for j in s:
        dic[j] += 1
    if dic['Z'] > 0:
        dic2[2] = dic['Z']
        dic['E'] -= dic['Z']
        dic['R'] -= dic['Z']
        dic['O'] -= dic['Z']
    if dic['W'] > 0:
        dic2[4] = dic['W']
        dic['T'] -= dic['W']
        dic['O'] -= dic['W']
    if dic['U'] > 0:
        dic2[6] = dic['U']
        dic['F'] -= dic['U']
        dic['R'] -= dic['U']
        dic['O'] -= dic['U']
    if dic['X'] > 0:
        dic2[8] = dic['X']
        dic['S'] -= dic['X']
        dic['I'] -= dic['X']
    if dic['G'] > 0:
        dic2[0] = dic['G']
        dic['E'] -= dic['G']
        dic['I'] -= dic['G']
        dic['H'] -= dic['G']
        dic['T'] -= dic['G']
    if dic['O'] > 0:
        dic2[3] = dic['O']
        dic['N'] -= dic['O']
        dic['E'] -= dic['O']
    if dic['T'] > 0:
        dic2[5] = dic['T']
        dic['H'] -= dic['T']
        dic['R'] -= dic['T']
        dic['E'] -= 2*dic['T']
    if dic['S'] > 0:
        dic2[9] = dic['S']
        dic['E'] -= 2*dic['S']
        dic['V'] -= dic['S']
        dic['N'] -= dic['S']
    if dic['F'] > 0:
        dic2[7] = dic['F']
        dic['I'] -= dic['F']
        dic['V'] -= dic['F']
        dic['E'] -= dic['F']
    if dic['E'] > 0:
        dic2[1] = dic['E']
        dic['N'] -= 2*dic['E']
        dic['I'] -= dic['E']
    for l in range(10):
        for k in range(dic2[l]):
            print l,
            sys.stdout.softspace=0   
    print ""
    dic2 = {0:0, 1:0, 2:0, 3:0, 4:0, 5:0, 6:0, 7:0, 8:0, 9:0}
    dic = {'E':0, 'F':0, 'G':0, 'H':0, 'I':0, 'N':0, 'O':0, 'R':0, 'S':0, 'T':0, 'U':0, 'V':0, 'W':0, 'X':0, 'Z':0}