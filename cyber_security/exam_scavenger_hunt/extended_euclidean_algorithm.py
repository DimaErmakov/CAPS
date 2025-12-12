def mat_mult(list_mat_a, list_mat_b):
  res_r = len(list_mat_a)
  res_c = len(list_mat_b[0])
  
  result = [ [0]*res_c for i in range(res_r) ]
  for i in range(res_r):
    for j in range(res_c):
      for k in range(len(list_mat_b)):
        result[i][j] += list_mat_a[i][k]*list_mat_b[k][j]
  return result

def extended_euclidean(a, b):
  x = min(a,b)
  y = max(a,b)

  coefs = [[1,0],[0,1]]
  
  # invariant: y > x > r:
  while x != 0:
    q = y // x
    r = y % x
    x_next, y_next = (r, x)

    coefs = mat_mult(coefs, [[-q, 1],[1, 0]])

    x, y = x_next, y_next

  c_x, c_y = coefs[0][1], coefs[1][1]
  if a > b:
    return (c_y, c_x), y
  return (c_x, c_y), y

if __name__=='__main__':
  big=100
  small=15
  print( extended_euclidean(big,small) )
