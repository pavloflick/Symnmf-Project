#define PY_SSIZE_T_CLEAN
#include <stdlib.h>
#include <Python.h>
#include "symnmf.h"

static PyObject *symnmf(PyObject *self, PyObject* args);
static PyObject *sym(PyObject *self, PyObject *args);
static PyObject *ddg(PyObject *self, PyObject *args);
static PyObject *norm(PyObject *self, PyObject *args);

PyMODINIT_FUNC PyInit_mysymnmfsp(void);
static double **convert_PyMatrix_To_CMatrix(PyObject *pyMatrix, int N, int dim);
static PyObject *convert_CMatrix_To_PyMatrix(double **ret_matrix, int N, int dim);

static PyObject *symnmf(PyObject *self, PyObject* args) {
    PyObject *pyW, *pyH;
    int k, N, dim;
    double **W, **H;

    // Update to expect k as a parameter
    if (!PyArg_ParseTuple(args, "OOiii", &pyW, &pyH, &k, &N, &dim)) {
        return NULL;  
    }

    W = convert_PyMatrix_To_CMatrix(pyW, N, N);
    H = convert_PyMatrix_To_CMatrix(pyH, N, k);

    // Call symnmf_c with k
    H = symnmf_c(H, W, N, k);

    PyObject *result = convert_CMatrix_To_PyMatrix(H, N, k);
    
    freeMatrix(W, N);
    freeMatrix(H, N);

    return result;
}

static PyObject *sym(PyObject *self, PyObject *args) {
    PyObject *pyX, *pyA;
    int N, dim;
    double **X, **A;

    if (!PyArg_ParseTuple(args, "Oii", &pyX, &N, &dim)) {
        return NULL;  
    }

    X = convert_PyMatrix_To_CMatrix(pyX, N, dim);
    A = sym_c(X, N, dim);

    pyA = convert_CMatrix_To_PyMatrix(A, N, N);

    freeMatrix(X, N);
    freeMatrix(A, N);

    return pyA;
}

static PyObject *ddg(PyObject *self, PyObject *args) {
    PyObject *pyX, *pyD;
    int N, dim;
    double **X, **D;

    if (!PyArg_ParseTuple(args, "Oii", &pyX, &N, &dim)) {
        return NULL;  
    }

    X = convert_PyMatrix_To_CMatrix(pyX, N, dim);
    D = ddg_c(X, N, dim);

    pyD = convert_CMatrix_To_PyMatrix(D, N, N);

    freeMatrix(X, N);
    freeMatrix(D, N);

    return pyD;
}

static PyObject *norm(PyObject *self, PyObject *args) {
    PyObject *pyX, *pyW;
    int N, dim;
    double **X, **W;

    if (!PyArg_ParseTuple(args, "Oii", &pyX, &N, &dim)) {
        return NULL;  
    }

    X = convert_PyMatrix_To_CMatrix(pyX, N, dim);
    W = norm_c(X, N, dim);

    pyW = convert_CMatrix_To_PyMatrix(W, N, N);

    freeMatrix(X, N);
    freeMatrix(W, N);

    return pyW;
}

static PyMethodDef module_methods[] = {
    { "symnmf", (PyCFunction) symnmf, METH_VARARGS, "Perform full the symNMF and output H" },
    { "sym", (PyCFunction) sym, METH_VARARGS, "Calculate and output the similarity matrix" },
    { "ddg", (PyCFunction) ddg, METH_VARARGS, "Calculate and output the Diagonal Degree Matrix" },
    { "norm", (PyCFunction) norm, METH_VARARGS, "Calculate and output the normalized similarity matrix" },
    { NULL, NULL, 0, NULL }  /* Sentinel indicating the end of the array */
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "mysymnmfsp",
    NULL,
    -1,
    module_methods
};

PyMODINIT_FUNC PyInit_mysymnmfsp(void) {
    return PyModule_Create(&symnmfmodule);
}

static double **convert_PyMatrix_To_CMatrix(PyObject *pyMatrix, int m, int n) {
    PyObject *curr_lst, *curr_cord;
    double **CMatrix;
    
    CMatrix = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++) {
        curr_lst = PyList_GET_ITEM(pyMatrix, i);
        CMatrix[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            curr_cord = PyList_GET_ITEM(curr_lst, j);
            CMatrix[i][j] = PyFloat_AsDouble(curr_cord);
            if (CMatrix[i][j] == -1.0 && PyErr_Occurred()) {
                // Handle the conversion error
                fprintf(stderr, "Error converting Python object to double\n");
                exit(1);
            }
        }
    }
    return CMatrix;
}

static PyObject *convert_CMatrix_To_PyMatrix(double **CMatrix, int m, int n) {
    PyObject *pyMatrix = PyList_New(m);
    for (int i = 0; i < m; i++) {
        PyObject *curr_lst = PyList_New(n);
        for (int j = 0; j < n; j++) {
            PyObject *pycord = Py_BuildValue("d", CMatrix[i][j]);
            PyList_SetItem(curr_lst, j, pycord);
        }
        PyList_SetItem(pyMatrix, i, curr_lst);
    }
    return pyMatrix;
}
