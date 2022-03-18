// module for spam extension of python

// recommended definition for proper parameter extraction
#define PY_SSIZE_T_CLEAN
#include <Python.h>
// also includes <stdio.h>, <string.h>, <errno.h>, <stdlib.h>
// <stdlib.h> defines malloc(), free(), realloc()

// custom exceptions - declared at beginning
static PyObject *SpamError;

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

// definition of SpamError
PyMODINIT_FUNC
PyInit_spam(void)
{
		PyObject *m;

		m = PyModule_Create(&spammodule);
		if (m == NULL)
				return NULL;

		// error name is spam.error
		// Creates a Python Object in the Exception class
		// owned reference is required to avoid dangling pointers 
		// if this is removed in the Python code
		SpamError = PyErr_NewException("spam.error", NULL, NULL);
		Py_XINCREF(SpamError);
		if (PyModule_AddObject(m, "error", SpamError) < 0) {
				Py_XDECREF(SpamError);
				Py_CLEAR(SpamError);
				Py_DECREF(m);
				return NULL;
		}

		return m;
}

// raising spam.error for incorrect calls
static PyObject *
spam_system(PyObject *self, PyObject *args)
{
		const char *command;
		int sts;

		if (!PyArg_ParseTuple(args, "s", &command))
				// failed to parse args
				return NULL;
		// execute command in unix system
		sts = system(command);
		// check status of command
		if (sts < 0) {
				PyErr_SetString(SpamError, "System command failed");
				return NULL;
		}

		// output is a Python object, deal with this
		return PyLong_FromLong(sts);
}

// A side note on None in Python:
// if returning None in overlying Python code use
// Py_INCREF(Py_None);
// return Py_None;
// as nonetypes in Python are also objects


// To allow this to be called in Python, need method table
static PyMethodDef SpamMethods[] = {
		// VARARGS for general arguments
		// KEYWORDS for keyword arguments
		// 	in this case need extra set of parameters
		// 	and use PyArg_ParseTupleAndKeywords()
		{"system", spam_system, METH_VARARGS,
				"Execute a shell command."},
		{NULL, NULL, 0, NULL} /* sentinel? */
};

// method table
static struct PyModuleDef spammodule = {
		PyModuleDef_HEAD_INIT,
		"spam", /* name of module */
		spam_doc, /* documentation, may be NULL */
		-1, /* size of per-interpreter state of module
			   or -1 if module keeps state in global vars */
		SpamMethods
};

// pass this method table structure to Python interpreter initialization
PyMODINIT_FUNC
PyInit_spam(void)
{
		return PyModule_Create(&spammodule);
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
		if (PyImport_AppendInittab("spam", PyInit_spam) == -1) {
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
		PyObject *pmodule = PyImport_ImportModule("spam");
		if (!pmodule) {
				PyErr_Print();
				fprintf(stderr, "Error: could not import module 'spam'\n");
		}

		PyMem_RawFree(program);
		return 0;
}

// see more complicated example in distribution Modules/xxmodule.c
