import sys

if len(sys.argv) < 2:
    print("Error: Filename not specified.")
    sys.exit(0)

else:
    fname = sys.argv[1]

import networkx

N = 100
INIT = 2

G = networkx.barabasi_albert_graph(N, INIT)
A = networkx.linalg.graphmatrix.adjacency_matrix(G).toarray()

x, y = A.shape

fptr = open(fname, "w")

ret = str(N) + "\n"

for i in range(x):
    for j in range(y):
        if j == y-1:
            ret += str(A[i][j]) + "\n"
        else:
            ret += str(A[i][j]) + " "

fptr.write(ret)

fptr.close()