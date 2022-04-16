// module for spam extension of python

// recommended definition for proper parameter extraction
#define PY_SSIZE_T_CLEAN
#include <Python.h>
// also includes <stdio.h>, <string.h>, <errno.h>, <stdlib.h>
// <stdlib.h> defines malloc(), free(), realloc()


// building a python list from a c array
static PyObject *
make_python_list(int array[], size_t size)
{
		PyObject * list = PyList_New(size);
		for (size_t i=0; i!=size; ++i)
		{
				long val = array[i];
				PyList_SET_ITEM(list, i, PyLong_FromLong(val));
		}
		return list;
}


// prime number computations
static PyObject *
find_primes(PyObject *self, PyObject *args)
{

		printf("Executing");
		if (!PyArg_ParseTuple(args, "i"))
				return NULL;

		long num_primes_long = PyLong_AsLong(args);
		size_t num_primes = num_primes_long;
		int primes[num_primes];
		primes[0] = 2;

		size_t primes_found = 1;
		int prime_cand = 3;

		size_t index = 0;

		while (primes_found < num_primes)
		{
				index = 0;
				while (index < primes_found)
				{
						if (prime_cand % primes[index] == 0)
						{
								break;
						} else {
								index = index + 1;
						}
				}

				if (index == primes_found)
				{
						primes[primes_found] = prime_cand;
						primes_found = primes_found + 1;
				}
				prime_cand = prime_cand + 1;
		}

		printf("Prime candiate");
		printf("%d", prime_cand);

		size_t list_length = primes_found;
		return make_python_list(primes, list_length);

}


// Some useful error tools
// generally problems indicated with error value (NULL pointer, val=-1, etc.)
// error setting/gathering tools
// 	PyErr_SetString(exception, string)
// 	PyErr_SetFromErrno(exception) -> constructed exception
// 	PyErr_SetObject(exception, value)
// 	PyErr_Occurred() -> did an error occur?
// 		often unneeded, should be obvious from function return value
// Generally don't build an error here, rather
// do exception handling in Python
// PyErr_* for more specific error handling at this level

// To clear exceptions which already happened: PyErr_Clear()
// only do this if the error should not bubble up to Python

// Failure of malloc()/realloc() -> PyErr_NoMemory()
// PyArg_ParseTuple() + friends -> +# (okay); 0, -1 (failed)

// A side note on None in Python:
// if returning None in overlying Python code use
// Py_INCREF(Py_None);
// return Py_None;
// as nonetypes in Python are also objects


// To allow this to be called in Python, need method table
static PyMethodDef PrimeMethods[] = {
		// VARARGS for general arguments
		// KEYWORDS for keyword arguments
		// 	in this case need extra set of parameters
		// 	and use PyArg_ParseTupleAndKeywords()
		{"find_primes", find_primes, METH_VARARGS,
				"Compute prime numbers."},
		{NULL, NULL, 0, NULL} /* sentinel? */
};


// method table
static struct PyModuleDef primemodule = {
		PyModuleDef_HEAD_INIT,
		"prime", /* name of module */
		NULL, /* documentation, may be NULL */
		-1, /* size of per-interpreter state of module
			   or -1 if module keeps state in global vars */
		PrimeMethods
};

// pass this method table structure to Python interpreter initialization
PyMODINIT_FUNC
PyInit_prime(void)
{
		return PyModule_Create(&primemodule);
}

// execution of module through python
int 
main(int argc, char *argv[])
{
		wchar_t *program = Py_DecodeLocale(argv[0], NULL);
		if (program == NULL) {
				fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
				exit(1);
		}

		// Add a built-in module before Py_Initialize
		if (PyImport_AppendInittab("prime", PyInit_prime) == -1) {
				fprintf(stderr, "Error: could not extend in-built modules table \n");
				exit(1);
		}

		// pass argv[0] to Python interpreter
		Py_SetProgramName(program);

		// initialize interpreter.
		// Required step.  
		// It this fails, it will be a fatal error
		Py_Initialize();

		// optionally import the module
		// alternatively importing can be deferred until 
		// the embedded script imports it
		PyObject *pmodule = PyImport_ImportModule("prime");
		if (!pmodule) {
				PyErr_Print();
				fprintf(stderr, "Error: could not import module 'prime'\n");
		}

		PyMem_RawFree(program);
		return 0;
}

// see more complicated example in distribution Modules/xxmodule.c
