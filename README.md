# python-extending
Following tutorial for extending Python with C and Fortran

## Some directions for C
After building the extension module c code, 
we need to compile it and link it to python.

In this case this is done via 
section 1.5 on https://docs.python.org/3/extending/extending.html
however this claims it rebuilds the interpreter for the specific 
project.  Instead we can use details provided
https://docs.python.org/3/extending/building.html#building
for "dynamic learning"

to compile, build setup.py as directed, 
then run 
$python setup.py build

## Some directions for Fortran
After building the extension module fortran code
we need to compile it so that python can use it.
This is done using f2py:

$f2py -c <fortran file> -m <module name for python>

