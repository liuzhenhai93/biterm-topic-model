//#include "stdafx.h"
#include <iostream>
#include"BTM.h"
using namespace std;
extern "C" {
	void initBTMmodel(void);/* Forward */

	int main(int argc, char **argv)
	{
		/* Pass argv[0] to the Python interpreter */
		Py_SetProgramName(argv[0]);
		
		/* Initialize the Python interpreter.  Required. */
		Py_Initialize();

		/* Add a static module */
		initBTMmodel();

		/* Define sys.argv.  It is up to the application if you
		want this; you can also leave it undefined (since the Python
		code is generally not a main program it has no business
		touching sys.argv...)

		If the third argument is true, sys.path is modified to include
		either the directory containing the script named by argv[0], or
		the current working directory.  This can be risky; if you run
		an application embedding Python in a directory controlled by
		someone else, attackers could put a Trojan-horse module in the
		directory (say, a file named os.py) that your application would
		then import and run.
		*/
		PySys_SetArgvEx(argc, argv, 0);

		/* Do some application specific code */
		printf("start execute some python code\n\n");

		/* Execute some Python statements (in module __main__) */
		PyRun_SimpleString("import sys\n");
		PyRun_SimpleString("print sys.builtin_module_names\n");
		PyRun_SimpleString("print sys.modules.keys()\n");
		PyRun_SimpleString("print sys.executable\n");
		PyRun_SimpleString("print sys.argv\n");
		PyRun_SimpleString("import BTM\n");
		PyRun_SimpleString("model= BTM.initmodel(20,99509,2.5,0.01,5,101)\n");
		PyRun_SimpleString("print \"everything went ok 1\"\n");
		//PyRun_SimpleString("BTM.train(model,\"data.txt\")");
		PyRun_SimpleString("BTM.loadmodel(model,\"mymodel\")");
		PyRun_SimpleString("print BTM.predict(model,[1,4324,434,44,4365,99])\n\n");
		PyRun_SimpleString("print \"everything went ok 2\"\n");
		PyRun_SimpleString("import os \n");
		PyRun_SimpleString("print os.getcwd()\n");
		printf("\nsee you next time!\n\n");
		int i;
		cin >> i;
		/* Exit, cleaning up the interpreter */
		Py_Exit(0);
		/*NOTREACHED*/
	}

	/* A static module */

	/* 'self' is not used */
	static PyObject *
		xyzzy_foo(PyObject *self, PyObject* args)
	{
		return PyInt_FromLong(42L);
	}

	static PyMethodDef BTMMethods[] = {
		{ "initmodel",  initmodel, METH_VARARGS,"initmodel." },
		{ "delete",modeldel,METH_VARARGS,"deletemodel." },
		{ "predict",predict,METH_VARARGS,"predict" },
		{ "train",train,METH_VARARGS ,"train" },
		{ "savemodel",savemodel,METH_VARARGS,"save" },
		{ "loadmodel",loadmodel,METH_VARARGS ,"load" },
		{ NULL,              NULL }
	};

	void
		initBTMmodel(void)
	{
		PyImport_AddModule("BTM");
		Py_InitModule("BTM", BTMMethods);
	};
}
