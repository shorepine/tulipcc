from ulab.numpy import zeros

def block(S):
    w = sum([len(m[0]) for m in S[0]])
    h = sum([len(row[0]) for row in S])
    M = zeros((h, w))
    i = 0
    j = 0
    for row in S:
        di = len(row[0])
        for matrix in row:
            dj = len(matrix[0])
            M[i:i + di, j:j + dj] = matrix
            j += dj
        i += di
        j = 0
    return M