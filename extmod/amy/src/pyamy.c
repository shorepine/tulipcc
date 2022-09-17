#include <Python.h>
#include <math.h>
#include "amy.h"

// Python module wrapper for AMY commands

static PyObject * send_wrapper(PyObject *self, PyObject *args) {
    char *arg1;
    if (! PyArg_ParseTuple(args, "s", &arg1)) {
        return NULL;
    }
    amy_play_message(arg1);
    return Py_None;
}


// TODO, take an optional arg for sound device ID
static PyObject * live_wrapper(PyObject *self, PyObject *args) {
    amy_live_start();
    return Py_None;
}

static PyObject * pause_wrapper(PyObject *self, PyObject *args) {
    amy_live_stop();
    return Py_None;
}


static PyObject * render_wrapper(PyObject *self, PyObject *args) {
    int16_t * result = fill_audio_buffer_task();
    // Create a python list of ints (they are signed shorts that come back)
    PyObject* ret = PyList_New(BLOCK_SIZE); 
    for (int i = 0; i < BLOCK_SIZE; i++) {
        PyObject* python_int = Py_BuildValue("i", result[i]);
        PyList_SetItem(ret, i, python_int);
    }
    return ret;
}


static PyMethodDef libAMYMethods[] = {
    {"render", render_wrapper, METH_VARARGS, "Render audio"},
    {"send", send_wrapper, METH_VARARGS, "Send a message"},
    {"live", live_wrapper, METH_VARARGS, "Live AMY"},
    {"pause", pause_wrapper, METH_VARARGS, "Pause AMY"},
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef libamyDef =
{
    PyModuleDef_HEAD_INIT,
    "libamy", /* name of module */
    "",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    libAMYMethods
};

PyMODINIT_FUNC PyInit_libamy(void)
{
    amy_start();
    return PyModule_Create(&libamyDef);

}
