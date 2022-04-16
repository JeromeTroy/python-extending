# benchmark prime number computation

import numpy as np
from time import time
from datetime import timedelta

num_primes = 5000
primes = []

primes.append(2)
prime_cand = 3

print("pure python, no tricks")
start = time()
while len(primes) < num_primes:
    ind = 0
    # test for divisibility in found primes list
    while ind < len(primes):
        if prime_cand % primes[ind] == 0:
            # divisible, no prime
            break
        else:
            ind += 1

    if ind == len(primes):
        # if we get here, this number is prime
        primes.append(prime_cand)
    
    # next number to check
    prime_cand += 1

end = time()

dt = end - start
print("Total time: {:s}".format(
    str(timedelta(seconds=dt))
))
last_prime = primes[-1]

print("using numpy")
primes = np.zeros(num_primes, dtype="int")
primes[0] = 2
primes_found = 1
prime_cand = 3

start = time()
while primes_found < num_primes:
    ind = 0
    is_new_prime = np.sum(
            prime_cand % primes[:primes_found - 1] == 0) == 0

    if is_new_prime:
        primes[primes_found] = prime_cand
        primes_found += 1

    prime_cand += 1

end = time()
dt = end - start
print("Total time: {:s}".format(
    str(timedelta(seconds=dt))
))

print("verification of same output")
print(last_prime)
print(primes[-1])
