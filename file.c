#include <Python.h>

typedef struct {
	double *data;
	size_t size;
} Matrix;


static PyObject *fromCToPyMatrix(Matrix *matrix) {
	size_t size = matrix->size;
	PyObject *ans = PyList_New(size);
	for (size_t i = 0; i < size; ++i) {
		PyObject *row = PyList_New(size);
		for (size_t j = 0; j < size; ++j) {
			PyObject *pyElement = PyFloat_FromDouble(matrix->data[i * size + j]);
			PyList_SetItem(row, j, pyElement);
		}
		PyList_SetItem(ans, i, row);
	}
	return ans;
}

static void cDot(Matrix *first, Matrix *second, Matrix *ans, size_t size) {
	ans->size = size;
	ans->data = calloc(size * size, sizeof(double));
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; ++j) {
			ans->data[i * size + j] = 0.0;
			for (size_t k = 0; k < size; ++k)
				ans->data[i * size + j] += first->data[i * size + k] * second->data[k * size + j];
		}
	}
}

static void fromPyToCMatrix(PyObject *pyMatrix, Matrix *cMatrix, size_t size) {
	cMatrix->size = size;
	cMatrix->data = calloc(size * size, sizeof(double));
	size_t height = PyList_Size(pyMatrix);
	for (size_t i = 0; i < size; ++i) {
		if (height < size) {
			PyObject *row = PyList_GetItem(pyMatrix, i);
			size_t weight = PyList_Size(row);
			for (size_t j = 0; j < size; ++j) {
				cMatrix->data[i * size + j] = 0.0;
				if (weight < size) {
					PyObject *pyElement = PyList_GetItem(row, j);
					double cElement = PyFloat_AsDouble(pyElement);
					cMatrix->data[i * size + j] = cElement;
				}
			}
		} else {
			for (size_t j = 0; j < size; ++j)
				cMatrix->data[i * size + j] = 0.0;
		}
	}
}


static PyObject *dot(PyObject *module, PyObject *args) {
	PyObject *pyShape = PyTuple_GetItem(args, 0);
	PyObject *pyFirstMatrix = PyTuple_GetItem(args, 1);
	PyObject *pySecondMatrix = PyTuple_GetItem(args, 2);

	size_t cShape = PyLong_AsSize_t(pyShape);
	Matrix cFirstMatrix, cSecondMatrix, ansMatrix;
	fromPyToCMatrix(pyFirstMatrix, &cFirstMatrix, cShape);
	fromPyToCMatrix(pySecondMatrix, &cSecondMatrix, cShape);
	cDot(&cFirstMatrix, &cSecondMatrix, &ansMatrix, cShape);

	PyObject *ans = fromCToPyMatrix(&ansMatrix);

	free(cFirstMatrix.data);
	free(cSecondMatrix.data);
	free(ansMatrix.data);

	return ans;
}


PyMODINIT_FUNC createModuleMatrix() {
	static PyMethodDef dotMethods[] = {
	    {
	        .ml_name = "dot",
	        .ml_meth = dot,
	        .ml_flags = METH_VARARGS,
	        .ml_doc = "Multiply two matrix"
	    },
	    {NULL, NULL, 0, NULL}
	};
	
	static PyModuleDef myModule = {
	    .m_base = PyModuleDef_HEAD_INIT,
	    .m_name = "matrix",
	    .m_size = -1,
	    .m_methods = dotMethods,
	};

	return PyModule_Create(&myModule);
}




int main() {
		
	return 0;
}
