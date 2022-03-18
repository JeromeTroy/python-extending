// module for spam extension of python

// recommended definition for proper parameter extraction
#define PY_SSIZE_T_CLEAN
#include <Python.h>
// also includes <stdio.h>, <string.h>, <errno.h>, <stdlib.h>
// <stdlib.h> defines malloc(), free(), realloc()

// spam.system in python
static PyObject *
spam_system(PyObject *self, PyObject *args)
{
		const char *command;
		int sts;

		if (!PyArg_ParseTuple(args, "s", &command))
				// incorrect type presented in argument, err out
				return NULL;
		sts = sytem(command);
		return PyLong_FromLong(sts);
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
