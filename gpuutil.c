#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <nvml.h>

void fail()
{
  nvmlReturn_t result;
  result = nvmlShutdown();
  if (NVML_SUCCESS != result)
    printf("Failed to shutdown NVML: %s\n", nvmlErrorString(result));
  exit(-1);
}


int showUtilization(int deviceIndex,int sleepInterval, int iterations) 
{
  nvmlReturn_t result;
  nvmlDevice_t device;

  unsigned long long lastSeenTimeStamp=0;
  nvmlValueType_t sampleValType;
  unsigned int sampleCount=1;
  nvmlSample_t samples;
  int sum=0;

  result = nvmlDeviceGetHandleByIndex(deviceIndex, &device);
  if (NVML_SUCCESS != result) { 
      printf("Failed to get handle for device %i: %s\n", deviceIndex, nvmlErrorString(result));
      fail();
    }

  for(int i=0;i<iterations;i++) {
    printf("About to query device utilization. sampleCount=%d\n",sampleCount);
    result = nvmlDeviceGetSamples(device, NVML_GPU_UTILIZATION_SAMPLES,
				  lastSeenTimeStamp,&sampleValType,&sampleCount,&samples);
    if (NVML_SUCCESS != result) { 
      printf("Failed to get samples for device %i: %s\n", deviceIndex, nvmlErrorString(result));
      fail();
    }
    int util= samples.sampleValue.uiVal;
    sum+=util;
    printf("Iteration %d. GPU utilization: %d\n",i,util );
    sleep(sleepInterval);
  }
  int average=sum/iterations;
  return average;
}

int initDone=0;

void init()
{
    nvmlReturn_t result;
    unsigned int device_count, i;

    result = nvmlInit();
    if (NVML_SUCCESS != result) { 
        printf("Failed to initialize NVML: %s\n", nvmlErrorString(result));
	fail();
    }
    initDone=1;
}

static PyObject *
getUtil(PyObject *self, PyObject *args)
{
  printf("getUtil called!\n");
  if(!initDone) init();
  int val=showUtilization(0,1,1);
  return Py_BuildValue("i", val);
}

static PyMethodDef module_methods[] = {
    
    {"getUtil",  getUtil, METH_VARARGS,
     "Query GPU utilization."},
    
    {NULL, NULL, 0, NULL}       
};



static struct PyModuleDef gpuutil =
{
    PyModuleDef_HEAD_INIT,
    "gpuutil", /* name of module */
    "usage: TODO\n", 
    -1,   /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    module_methods
};
PyMODINIT_FUNC PyInit_gpuutilmodule(void)
{
    return PyModule_Create(&gpuutil);
}
