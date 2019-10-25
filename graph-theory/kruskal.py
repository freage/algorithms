"""
Find the MST of a complete graph using Kruskal's algorithm.
"""

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

def solve(dist,N):
    ### Graph of `N` nodes with edge weights in the distance matrix
    edges = [ (i,j) for j in range(N) for i in range(N) if i<j ]
    ds = DisjointSets(N)
    key = lambda e : dist[e[0]][e[1]]
    edges.sort(key=key)
    size = 0
    tree = set()
    num_edges = 0
    for a,b in edges:
        if not ds.same(a,b):
            ds.union(a,b)
            tree.add((a,b))
            size += dist[a][b]
            num_edges += 1
            if num_edges == N - 1: break
    assert num_edges == N - 1
    return tree, size
