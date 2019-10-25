### A solution to https://open.kattis.com/problems/cats

import sys
#input = sys.stdin.readline

the_input = sys.stdin.readlines()
line_counter = -1

def input():
    global line_counter
    line_counter += 1
    return the_input[line_counter]

class DisjointSets:
    def __init__(self,n):
        self.roots = list(range(n))
        self.rank = n*[0]
    
    def __find(self, x):
        while x != self.roots[x]:
            p = self.roots[x]
            self.roots[x] = self.roots[p]
            x = p
        return x

    def union(self, x, y):
        xRoot = self.__find(x)
        yRoot = self.__find(y)

        if self.rank[xRoot] > self.rank[yRoot]:
            xRoot, yRoot = yRoot, xRoot
        # now y has higher or equal rank
        self.roots[xRoot] = yRoot
        if self.rank[xRoot] == self.rank[yRoot]:
            self.rank[yRoot] += 1

    def same(self, x, y):
        return self.__find(x) == self.__find(y)


def io():
    T = int(input().rstrip())
    for _ in range(T):
        M, C = [ int(s) for s in input().split() ]
        pairs = int(C*(C-1)/2)
        dist = [ [0]*C for _ in range(C) ]
        for _ in range(pairs):
            row = input().split()
            i, j, d = int(row[0]), int(row[1]), int(row[2])
            dist[i][j] = dist[j][i] = d
        print(solve(dist,M,C))

def solve(dist,M,C):
    edges = [ (i,j) for j in range(C) for i in range(C) if i<j ]
    ds = DisjointSets(C)
    key = lambda e : dist[e[0]][e[1]]
    edges.sort(key=key)
    walk = 0
    num_edges = 0
    for a,b in edges:
        if not ds.same(a,b):
            ds.union(a,b)
            walk += dist[a][b]
            num_edges += 1
            if num_edges == C - 1: break
    assert num_edges == C - 1
    if M < walk + C:
        return "no"
    else:
        return "yes"
        

if __name__ == "__main__":
    io()
