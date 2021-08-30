#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <frameobject.h>


typedef void(voidPyFunction)(PyObject*);


static void default_coroutine_handler(PyObject* coroutine);

static PyObject* async_loop = Py_None;

static PyObject* get_async_loop_callable(PyObject* self, PyObject* args) {
    return async_loop;
}

static PyObject* set_async_loop_callable(PyObject* self, PyObject* callable) {
    if (!PyArg_ParseTuple(callable, "O", &callable)) {
        PyErr_SetString(PyExc_TypeError, "expected exactly 1 argument");
        return nullptr;
    }
    if (callable != Py_None) {
        if (!PyCallable_Check(callable)) {
            PyErr_SetString(PyExc_TypeError, "supplied argument must be callable");
            return nullptr;
        }
    }
    if (async_loop == callable) Py_RETURN_NONE;
    Py_INCREF(callable);
    Py_DECREF(async_loop);
    async_loop = callable;
    Py_RETURN_NONE;
}

static PyMethodDef PyNodeMethods[] = {
    { "get_coroutine_handler", get_async_loop_callable, METH_VARARGS, "get the coroutine handler" },
    { "set_coroutine_handler", set_async_loop_callable, METH_VARARGS, "set the coroutine handler" },
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef pynodemodule = {
    PyModuleDef_HEAD_INIT,
    "pynode.core",   /* name of module */
    "PyNode allowes you to run asynchronous tasks cuncurrently.", /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    PyNodeMethods
};

static voidPyFunction* PyCoro_Dealloc_Original;

static void PyCoro_Dealloc_Override(PyObject* self) {
    if (self != nullptr) {
        PyGenObject* gen = (PyGenObject*)self;
        if (gen->gi_code != nullptr && ((PyCodeObject*)gen->gi_code)->co_flags & CO_COROUTINE) {
            if (gen->gi_frame != nullptr) {
                if (async_loop == Py_None) {
                    default_coroutine_handler(self);
                }
                else {
                    PyObject_CallFunctionObjArgs(async_loop, self, nullptr);
                }
            }
            else if (self->ob_refcnt == 0) {
                PyCoro_Dealloc_Original(self);
            }
        }
    }
}

static void default_coroutine_handler(PyObject* coroutine) {
    static PyObject* asyncio = PyImport_ImportModule("asyncio");
    if (asyncio != nullptr) {
        PyObject* event_loop = PyObject_CallFunctionObjArgs(PyObject_GetAttrString(asyncio, "get_event_loop"), nullptr);
        PyObject* running = PyObject_CallFunctionObjArgs(PyObject_GetAttrString(event_loop, "is_running"), nullptr);
        if (running == Py_True) {
            PyObject_CallFunctionObjArgs(PyObject_GetAttrString(event_loop, "create_task"), coroutine, nullptr);
        }
        else {
            PyObject_CallFunctionObjArgs(PyObject_GetAttrString(event_loop, "run_until_complete"), coroutine, nullptr);
        }
    }
}

PyMODINIT_FUNC PyInit_core(void) {
    
    PyCoro_Dealloc_Original = PyCoro_Type.tp_dealloc;
    PyCoro_Type.tp_finalize = &PyCoro_Dealloc_Override;
    return PyModule_Create(&pynodemodule);
}
