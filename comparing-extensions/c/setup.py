# setting up extension to be 
# importable into python
from distutils.core import setup, Extension

module1 = Extension("prime",
        sources = ["primemodule.c"])

setup(name="Prime",
        version="1.0",
        description="Finding primes",
        ext_modules=[module1])
