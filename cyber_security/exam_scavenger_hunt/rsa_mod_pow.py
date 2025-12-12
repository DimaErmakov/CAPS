from mod_inverse import *

# returns a**b % c, but faster:
def mod_pow(a,b,c):
  if b == 0:
    return 1
  if b == 1:
    return a % c
    
  result = mod_pow(a, b//2, c)**2 % c
  if b % 2 == 1:
    result = (result * a) % c
  return result

if __name__=='__main__':
  # secret primes comprising B's private key:
  p = 176053
  q = 174527

  # B's public key is p*q and some prime e. These are shared freely with
  # all.
  n = p*q
  e = 881
  print( 'n', n )

  # B computes a valid d (to be kept secret). Opponents may need to
  # factor n in order to compute (p-1)*(q-1).
  d = compute_d_from_e_and_p_minus_1_q_minus_1(e, (p-1)*(q-1))
  print( 'd', d )
  print( 'verifying e*d % (p-1)*(q-1) = 1: ', e*d, '%', (p-1)*(q-1), '=', (e*d) % ((p-1)*(q-1)) )

  # an integer encoding of some message:
  m = 10311
  assert(m < n)

  # A encrypts the message for B. This ciphertext can be safely
  # transmitted from A to B, even if someone is listening in:
  #c = (m**e) % n
  c = mod_pow(m,e,n)
  print( 'ciphertext', c )


  ### B decrypts the message from A:

  # slowwww version:
  #print( 'decrypted message', (c**d) % n )

  # fast version:
  print( 'decrypted message', mod_pow(c,d,n) )
