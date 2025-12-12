import sys
# We can make an iterative version, but for now, this is fine:
sys.setrecursionlimit(100000)

# get mod_pow function:
from rsa_mod_pow import *

import random

def decompose_p_minus_one_into_s_and_d(p):
  p -= 1
  s = 0
  while p % 2 == 0:
    s += 1
    p = p // 2
  return (s, p)

def miller_rabin_may_be_prime_helper(a, s, d, p):
  x_mod_p = mod_pow(a, 2**s * d, p)

  if x_mod_p == p-1:
    # Base case congruent to -1
    return True
  if x_mod_p == 1 and s > 0:
    return miller_rabin_may_be_prime_helper(a, s-1, d, p)
  if x_mod_p == 1 and s == 0:
    # No more powers of two to eliminate; base case congruent to 1.
    return True
  # Not a valid recursive or base case:
  # a is a witness for the compositeness of p
  return False

def miller_rabin_may_be_prime(a, p):
  if p < 2:
    # composite
    return False
  if p == 2:
    return True

  if mod_pow(a, p-1, p) != 1:
    return False

  s, d = decompose_p_minus_one_into_s_and_d(p)
  return miller_rabin_may_be_prime_helper(a, s-1, d, p)

def find_big_prime(bits, number_a_to_try):
  while True:
    #innocent until proven guilty
    # (i.e., prime until proven not prime):
    success = True

    p = random.getrandbits(bits)*2+1
    print( 'candidate', p )
    for a in [2,3,5,7]:
      if not miller_rabin_may_be_prime(a, p):
        success = False
        break
    if not success:
      continue
    for i in range(number_a_to_try):
      a = (random.getrandbits(bits) % (p-2)) + 2 # in {2, 3, ..., p-1}
      if not miller_rabin_may_be_prime(a, p):
        success = False
        break
    if success:
      return p

def main(args):
  if len(args) != 2:
    print( 'usage: generate_prime <bits> <number_of_primality_checks>' )
  else:
    bits, num_checks = int(args[0]), int(args[1])
    print( find_big_prime(bits, num_checks) )

if __name__=='__main__':
  main(sys.argv[1:])
