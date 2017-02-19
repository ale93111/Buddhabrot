#include <iostream>
#include <stdlib.h>
#include <string>
#include <python2.7/Python.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include "buddhabrot.h"

void init()
{
	import_array();
}

int main(int argc, char *argv[])
{
	int Npoints = 50000000;
	long w = 3200;
	long h = 2400;
	
	Buddhabrot b(Npoints,w,h);
	b.minY = -1.5f;
	b.maxY =  1.5f;

	b.fill();
	b.Normalizehitcount();
	
	//convert vector to C array
	void* hitcount = &b.hitcount[0];
	
	npy_intp dims = 3*w*h;
	
	Py_Initialize();
	init();
	PyRun_SimpleString("from time import time,ctime\n" "print 'Today is',ctime(time())\n");
	
	PyObject* a = PyArray_SimpleNewFromData(1, &dims, NPY_FLOAT, (void*)hitcount);

	PyObject* shape = PyTuple_New(3);
	PyTuple_SetItem(shape, 0, PyLong_FromSize_t(w));
	PyTuple_SetItem(shape, 1, PyLong_FromSize_t(h));
	PyTuple_SetItem(shape, 2, PyLong_FromSize_t(3));
	PyObject *img = PyArray_Reshape((PyArrayObject*)a,shape);

	PyObject *pModule, *plot_func, *pArgTuple, *pValue, *func_imsave;
	//PyObject *sys = PyImport_ImportModule("sys");
	//PyObject *path = PyObject_GetAttrString(sys, "path");
	//PyList_Append(path, PyBytes_FromString("/home/alessandro/Documenti/Buddhabrot/"));
	   
	pModule = PyImport_ImportModule("matplotlib.pyplot"); //Get the module

	if (pModule != NULL) {
		plot_func = PyObject_GetAttrString(pModule, "imshow");   //Get the function by its name
		func_imsave = PyObject_GetAttrString(pModule, "imsave");

		if (plot_func && PyCallable_Check(plot_func)) 
		{
			//Set up a tuple that will contain the function arguments.
			pArgTuple = PyTuple_New(1);

			//Set the argument tuple to contain the two input tuples
			PyTuple_SetItem(pArgTuple, 0, img);

			//Call the python function
			pValue = PyObject_CallObject(plot_func, pArgTuple);

		}
	}
	else 
	{
		std::cout << "Error" << std::endl;
	}

	std::string filename = "./a.png";
	PyObject* pyfilename = PyString_FromString(filename.c_str());

	PyObject* args = PyTuple_New(2);
	PyTuple_SetItem(args, 0, pyfilename);
	PyTuple_SetItem(args, 1, img);

	PyObject* res = PyObject_CallObject(func_imsave, args);


	std::cout << "fine" << std::endl;	
	
	Py_DECREF(res);
	Py_DECREF(pyfilename);
	Py_DECREF(args);
	Py_DECREF(img);
	Py_DECREF(pModule);
	Py_DECREF(plot_func);
	Py_DECREF(pArgTuple);
	Py_DECREF(pValue);
	
  	Py_Finalize();

	return 0;
}