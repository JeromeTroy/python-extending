# benchmark prime number computation

import numpy as np
from time import time
from datetime import timedelta

from primemod import get_primes

num_primes = 5000

print("Python augmented with fortran")

start = time()
primes = get_primes(num_primes)
end = time()

dt = end - start
print("Total time: {:s}".format(
    str(timedelta(seconds=dt))
))

print("Final prime: ", primes[-1])
print(type(primes))
