#!/usr/bin/python
# -*- coding: utf-8 -*-
##################################
# Author:           Lukas Nevrkla
##################################

# Used indexing of utility space:
#  1      2      3      4      5
#  6      7      8      9     10
# 11     12     13     14     15
# 16     17     18     19     20

# USER INPUT -----------------------

Alpha = 0.1
Gamma = 0.9
Path = '1 2 7 12 11 16 17 18 13 14 9'
Utilities = \
    """
 0.064  0.063 -0.050 -0.311 -0.109
  rew=1  0.042 -0.106 rew=-1 -0.724
  0.739  0.100 -0.107 -0.462 -0.372
  0.312  0.033 -0.057 -0.179 -0.112
"""

# ----------------------------------

RewPrefixLen = len('rew=')


def parse(str):
    return str.replace('\n', ' ').split()


def parseUtilities(str):
    splitted = parse(str)
    for (i, val) in enumerate(splitted):
        try:
            splitted[i] = {
                'utility': float(val),
                'reward': 0.0,
                'isReward': False,
                'raw': val,
                }
        except ValueError:
            splitted[i] = {
                'utility': 0.0,
                'reward': float(val[RewPrefixLen:len(val)]),
                'isReward': True,
                'raw': val,
                }

    return splitted


def printUtilities(utilities, lineCnt):
    itemsPerLine = len(utilities) // lineCnt
    for i in range(0, lineCnt):
        split = utilities[i * itemsPerLine:(i + 1) * itemsPerLine]
        print(''.join(split))


def calcUtility(curr, next):
    return (1 - Alpha) * curr['utility'] + \
        Alpha * (next['reward'] + Gamma * next['utility'])


if __name__ == '__main__':
    utilities = Utilities.strip()
    lineCnt = len(utilities.splitlines())
    utilities = parseUtilities(Utilities)
    path = [int(i) - 1 for i in parse(Path)]

    for (i, val) in enumerate(path[:-1]):
        curr = utilities[val]
        next = utilities[path[i + 1]]
        utilities[val]['utility'] = calcUtility(curr, next)

    utilities = [(format(i['raw'], '>7') if i['isReward'] 
        else '{:7.4f}'.format(round(i['utility'], 4)))
        for i in utilities]

    printUtilities(utilities, lineCnt)
