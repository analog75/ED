#!/usr/bin/python
import sys, math, random, copy
neighbors_of = {}
# nw ne e se sw w
neighbors_of['q'] = [ 'w', 'a']
neighbors_of['w'] = [ 'e', 's', 'a', 'q']
neighbors_of['e'] = [ 'r', 'd', 's', 'w']
neighbors_of['r'] = [ 't', 'f', 'd', 'e']
neighbors_of['t'] = [ 'y', 'g', 'f', 'r']
neighbors_of['y'] = [ 'u', 'h', 'g', 't']
neighbors_of['u'] = [ 'i', 'j', 'h', 'y']
neighbors_of['i'] = [ 'o', 'k', 'j', 'u']
neighbors_of['o'] = [ 'p', 'l', 'k', 'i']
neighbors_of['p'] = [ 'l', 'o']
neighbors_of['a'] = ['q', 'w', 's', 'z']
neighbors_of['s'] = ['w', 'e', 'd', 'x', 'z', 'a']
neighbors_of['d'] = ['e', 'r', 'f', 'c', 'x', 's']
neighbors_of['f'] = ['r', 't', 'g', 'v', 'c', 'd']
neighbors_of['g'] = ['t', 'y', 'h', 'b', 'v', 'f']
neighbors_of['h'] = ['y', 'u', 'j', 'n', 'b', 'g']
neighbors_of['j'] = ['u', 'i', 'k', 'm', 'n', 'h']
neighbors_of['k'] = ['i', 'o', 'l', 'm', 'j']
neighbors_of['l'] = ['o', 'p', 'k']
neighbors_of['z'] = ['a', 's', 'x']
neighbors_of['x'] = ['s', 'd', 'c', 'z']
neighbors_of['c'] = ['d', 'f', 'v', 'x']
neighbors_of['v'] = ['f', 'g', 'b', 'c']
neighbors_of['b'] = ['g', 'h', 'n', 'v']
neighbors_of['n'] = ['h', 'j', 'm', 'b']
neighbors_of['m'] = ['j', 'k', 'n']
keys = sorted(neighbors_of.keys())
dists = {el:{} for el in keys}
def distance(start, end, raw):
if start == end:
if raw:
return 0
else:
return 1
visited = [start]
queue = []
for key in neighbors_of[start]:
queue.append({'char': key, 'dist': 1})
while True:
key = queue.pop(0)
visited.append(key['char'])
if key['char'] == end:
return key['dist']


