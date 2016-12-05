#include <iostream>
#include <stdlib.h>
#include <python2.7/Python.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include "buddhabrot.h"


void init_numpy()
{
	import_array();
}

int main(int argc, char *argv[])
{
	int Npoints = 10000000;
	long w = 1024;//3200;
	long h = 1024;//2400;
	
	Buddhabrot b(Npoints,w,h);
	//b.minY = -1.5f;
	//b.maxY =  1.5f;

	b.fill();
	b.Normalizehitcount();
	
	//convert vector to C array
	void* hitcount = &b.hitcount[0];
	
	npy_intp dims = 3*w*h;
	
	Py_Initialize();
	init_numpy();
	PyRun_SimpleString("from time import time,ctime\n" "print 'Today is',ctime(time())\n");
	
	PyObject* pyhitcount = PyArray_SimpleNewFromData(1, &dims, NPY_INT, hitcount);

	PyObject *pName, *pModule, *plot_func, *pArgTuple, *pValue;
	PyObject *sys = PyImport_ImportModule("sys");
	PyObject *path = PyObject_GetAttrString(sys, "path");
	PyList_Append(path, PyBytes_FromString("/home/alessandro/Documenti/Buddhabrot/"));
	
	const char* Name = "plot";
	pName = PyBytes_FromString("plot"/*argv[1]*/);   //Get the name of the module
	pModule = PyImport_Import(pName);     //Get the module

	if (pModule != NULL) {
		plot_func = PyObject_GetAttrString(pModule, "plot"/*argv[2]*/);   //Get the function by its name
		
		if (plot_func && PyCallable_Check(plot_func)) 
		{
			//Set up a tuple that will contain the function arguments.
			pArgTuple = PyTuple_New(3);

			//Set the argument tuple to contain the two input tuples
			PyTuple_SetItem(pArgTuple, 0, pyhitcount);
			PyTuple_SetItem(pArgTuple, 1, PyInt_FromLong(w));
			PyTuple_SetItem(pArgTuple, 2, PyInt_FromLong(h));

			//Call the python function
			pValue = PyObject_CallObject(plot_func, pArgTuple);

		}
	}
	else 
	{
		std::cout << "Error" << std::endl;
	}


	std::cout << "fine" << std::endl;	
	//PyArrayObject* array = (PyArrayObject *)PyArray_GETCONTIGUOUS(hitcount);
	//PyObject shape[] = {w,h,3};
	//PyObject* img = PyArray_Reshape(PyArray_DATA((PyArrayObject*)pyhitcount), &shape);
	
	
	
	Py_DECREF(pyhitcount);
	Py_DECREF(path);
	Py_DECREF(sys);
	Py_DECREF(pName);
	Py_DECREF(pModule);
	Py_DECREF(plot_func);
	Py_DECREF(pArgTuple);
	Py_DECREF(pValue);
	
  	Py_Finalize();

	return 0;
}