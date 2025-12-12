from extended_euclidean_algorithm import *

# finds y : x*y % mod_base = 1
def compute_d_from_e_and_p_minus_1_q_minus_1(e, p_minus_one_q_minus_one):
  # e*d = k*(p-1)*(q-1) + 1
  # e*d - k*(p-1)*(q-1) = 1
  (d, neg_k), gcd = extended_euclidean(e, p_minus_one_q_minus_one)
  print(f'{neg_k=}')

  # arguments x and mod_base must be coprime:
  assert(gcd == 1)
  
  # in case d is <0:
  return d % p_minus_one_q_minus_one

if __name__=='__main__':
  a = 30
  b = 1021

  d = compute_d_from_e_and_p_minus_1_q_minus_1(e, p_minus_one_q_minus_one)
  print( 'Found inverse of', e, 'which is', d )
  print( 'Verifying inverse e*d % p_minus_one_q_minus_one = 1:', e*d, '%', p_minus_one_q_minus_one, '=', e*d % p_minus_one_q_minus_one )
