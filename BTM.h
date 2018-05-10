
#ifndef BTM_C_API_H_
#define BTM_C_API_H_
#include "model.h"
#include "Python.h"
#ifdef __cplusplus
#define BTM_EXTERN_C extern "C"
#include <cstdio>
#else
#include <stdio.h>
#endif

#if defined(_MSC_VER) || defined(_WIN32)
#define BTM_DLL BTM_EXTERN_C //__declspec(dllexport)
#else
#define BTM_DLL BTM_EXTERN_C
#endif
	BTM_DLL PyObject *initmodel(PyObject *self, PyObject* args);
	BTM_DLL PyObject * train(PyObject *self, PyObject*args);
	BTM_DLL PyObject * predict(PyObject *self, PyObject*args);
	BTM_DLL PyObject * modeldel(PyObject *self, PyObject*args);
	BTM_DLL  PyObject * loadmodel(PyObject *self, PyObject*args);
	BTM_DLL  PyObject * savemodel(PyObject *self, PyObject*args);
#endif
