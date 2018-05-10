#include "BTM.h" 
	BTM_DLL  PyObject *
		initmodel(PyObject *self, PyObject* args)
	{
		int K;
		int W;
		double a;
		double b;
		int n_iter;
		int save_step;
		if(! PyArg_ParseTuple(args, "iiddii", &K, &W, &a, &b, &n_iter, &save_step))
			return NULL;
		Model* model = new Model(K, W, a, b, n_iter, save_step);
		return PyInt_FromSize_t((unsigned long long)model);
	}
	BTM_DLL  PyObject * train(PyObject *self, PyObject*args)
	{
		cout << "train";
		char* file;
		unsigned long long model;
		if( !PyArg_ParseTuple(args, "Ks", &model, &file))
			return NULL;
		//reinterpreter cast
		Model* rmodel = (Model*)model;
		rmodel->train(file);
		Py_INCREF(Py_None);

		return Py_None;
	}
	BTM_DLL  PyObject * savemodel(PyObject *self, PyObject*args)
	{
		cout << "savemodel";
		char* file;
		unsigned long long model;
		if (!PyArg_ParseTuple(args, "Ks", &model, &file))
			return NULL;
		//reinterpreter cast
		Model* rmodel = (Model*)model;
		rmodel->savemodel(file);
		Py_INCREF(Py_None);

		return Py_None;
	}
	BTM_DLL  PyObject * loadmodel(PyObject *self, PyObject*args)
	{
		cout << "savemodel";
		char* file;
		unsigned long long model;
		if (!PyArg_ParseTuple(args, "Ks", &model, &file))
			return NULL;
		//reinterpreter cast
		Model* rmodel = (Model*)model;
		rmodel->loadmodel(file);
		Py_INCREF(Py_None);

		return Py_None;
	}
	BTM_DLL  PyObject * predict(PyObject *self, PyObject*args)
	{
		cout << "predict";
		vector<int> doc;
		unsigned long long model;
		PyObject*tmp;
		if(!PyArg_ParseTuple(args, "KO", &model, &tmp))
			return NULL;
		
			int llength = PyObject_Size(tmp);
			for (int i = 0; i < llength; i++) 
			{
				PyObject* item = PyList_GetItem(tmp, i);
				int id = _PyInt_AsInt(item);
				doc.push_back(id);
			}
		//reinterpreter cast
		Model* rmodel = (Model*)model;
		auto topics=rmodel->predict(doc);
		PyObject*pylist= PyList_New(0);
		for(vector<double>::iterator iter=topics.begin();iter!=topics.end();iter++)
		{
			double tmp = *iter;
			PyList_Append(pylist, PyFloat_FromDouble(tmp));
		}
		
		return pylist;
	}
	BTM_DLL  PyObject * modeldel(PyObject *self, PyObject*args)
	{
		unsigned long long model;
		if(! PyArg_ParseTuple(args, "K", &model))
			return NULL;
		Model *rmodel = (Model*)model;
		delete rmodel;
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyMethodDef BTMMethods[] = {
		{ "initmodel",  initmodel, METH_VARARGS,"initmodel." },
		{ "delete",modeldel,METH_VARARGS,"deletemodel." },
		{"predict",predict,METH_VARARGS,"predict"},
		{"train",train,METH_VARARGS ,"train"},
		{ "savemodel",savemodel,METH_VARARGS,"save" },
		{ "loadmodel",loadmodel,METH_VARARGS ,"load" },
		{ NULL,              NULL }
	};
BTM_DLL	void initBTM(void){
		Py_InitModule("BTM", BTMMethods);
	}