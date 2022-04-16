import prime
from time import time

print("imports done")
num_primes = 50
primes = prime.find_primes(num_primes)

print("last prime", primes[-1])

