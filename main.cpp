#include <Python.h>
#include <dictobject.h>
#include <import.h>
#include <listobject.h>
#include <moduleobject.h>

int main() {
  Py_Initialize();

  const char *python_code =
      "import psutil\n"
      "mem = psutil.Process().memory_info().rss / 1024 / 1024\n"
      "print(mem)\n"
      "def foo(arr):\n"
      "    return arr * 2\n"
      "arr2 = foo(arr)\n";

  for (int i = 0; i < 10; ++i) {
    PyObject *compiled_code =
        Py_CompileString(python_code, "memleak", Py_file_input);
    if (compiled_code == NULL) {
      PyErr_Print();
      Py_Finalize();
      return 1;
    }

    PyObject *dict = PyDict_New();
    if (dict == NULL) {
      PyErr_Print();
      Py_Finalize();
      return 1;
    }

    PyObject *dict2 = PyDict_New();
    if (dict2 == NULL) {
      PyErr_Print();
      Py_Finalize();
      return 1;
    }

    PyObject *builtins = PyEval_GetBuiltins();
    if (builtins == NULL) {
      PyErr_Print();
      Py_Finalize();
      return 1;
    }
    Py_INCREF(builtins);

    if (PyDict_SetItemString(dict, "__builtins__", builtins) != 0) {
      PyErr_Print();
      Py_Finalize();
      return 1;
    }

    const int SIZE = 10 * 1024 * 1024 / 8;
    PyObject *list = PyList_New(SIZE);
    if (list == NULL) {
      PyErr_Print();
      Py_Finalize();
      return 1;
    }

    for (int i = 0; i < SIZE; i++) {
      PyObject *pyint = PyLong_FromLong(i);
      if (pyint == NULL) {
        PyErr_Print();
        Py_Finalize();
        return 1;
      }
      PyList_SetItem(list, i, pyint);
    }

    if (PyDict_SetItemString(dict, "arr", list) != 0) {
      PyErr_Print();
      Py_Finalize();
      return 1;
    }

    PyObject *result = PyEval_EvalCode(compiled_code, dict, dict);
    if (result == NULL) {
      PyErr_Print();
      Py_Finalize();
      return 1;
    }

    Py_DECREF(dict);
    Py_DECREF(list);
    Py_DECREF(dict2);
    Py_DECREF(builtins);
    Py_DECREF(result);
    Py_DECREF(compiled_code);
  }

  Py_Finalize();
  return 0;
}
