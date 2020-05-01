#define CL_TARGET_OPENCL_VERSION 120

#include <CL/opencl.h>
#include <stdio.h>
#include <stdlib.h>

#define D(d) printf("line:%d\t%d\n", __LINE__, d)
#define Dp(d) printf("%p\n", d)
#define Ds(d) printf("%s\n", d)
cl_program pr;
cl_kernel kr;
void init_kern(cl_context context)
{
	FILE *f = fopen("test.cl","rb");
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);
	rewind(f);
	char buf[sz+1];
//	char *buf = malloc(sz+1);
	fread(buf, 1, sz, f);
	//sz = 0;
	buf[sz] = '\0';
	D(sz);
	Ds(buf);
	cl_int ret;
	fclose(f);
	char *bufptr = buf;
	pr = clCreateProgramWithSource(context, 1,
								(const char **)&bufptr, (size_t *)&sz,
								&ret);
	D(ret);
}
void build(cl_device_id dId)
{
	cl_int ret;
	ret = clBuildProgram(pr, 1, &dId, NULL, NULL, NULL);
	D(ret);
	kr = clCreateKernel(pr,"test", &ret);
	D(ret);
}
void param(cl_context context, cl_command_queue queue)
{
	cl_int ret;
	cl_mem mobj;
	int memlen = 20;
	cl_int *mem = malloc(memlen * sizeof (cl_int));
	for(int i=0; i<memlen; i++)
		mem[i] = i;
	mobj = clCreateBuffer(context, CL_MEM_READ_WRITE,
							memlen*sizeof(cl_int), NULL,
							&ret);
	D(ret);
	ret = clEnqueueWriteBuffer(queue, mobj,
								CL_TRUE, 0,
								memlen*sizeof(cl_int), mem,
								0, NULL,
								NULL);
	D(ret);
	ret = clSetKernelArg(kr, 0,
						sizeof(cl_mem), (void *)&mobj);
	D(ret);
	size_t global_work_size[1] = { 20 };
	ret = clEnqueueNDRangeKernel(queue, kr,
								1, NULL,
								global_work_size, NULL,
								0, NULL,
								NULL);
	ret = clEnqueueReadBuffer(queue, mobj,
							CL_TRUE, 0,
							memlen*sizeof(float), mem,
							0, NULL,
							NULL);
	D(ret);
	for(int i=0; i<memlen; i++)
		printf("%d\t", mem[i]);
	Ds("\n");
}
int main()
{
	cl_platform_id pId;
	cl_uint num;
	cl_int ret;
	cl_device_id dId;
	clGetPlatformIDs(1, &pId, &num);
	clGetDeviceIDs(pId, CL_DEVICE_TYPE_GPU, 1, &dId, &num);
	char buf[100];
	clGetPlatformInfo(pId, CL_PLATFORM_PROFILE, 100, buf, NULL);
	Ds(buf);
	clGetPlatformInfo(pId, CL_PLATFORM_VERSION, 100, buf, NULL);
	Ds(buf);
	clGetPlatformInfo(pId, CL_PLATFORM_NAME, 100, buf, NULL);
	Ds(buf);
	clGetPlatformInfo(pId, CL_PLATFORM_VENDOR, 100, buf, NULL);
	Ds(buf);
	clGetDeviceInfo(dId, CL_DEVICE_TYPE, 100, buf, NULL);
	Ds(buf);
	clGetDeviceInfo(dId, CL_DEVICE_VENDOR, 100, buf, NULL);
	Ds(buf);
	clGetDeviceInfo(dId, CL_DEVICE_PROFILE, 100, buf, NULL);
	Ds(buf);
	clGetDeviceInfo(dId, CL_DEVICE_NAME, 100, buf, NULL);
	Ds(buf);
	clGetDeviceInfo(dId, CL_DEVICE_EXTENSIONS, 100, buf, NULL);
	Ds(buf);

	cl_context context = clCreateContext(NULL, 1, &dId, NULL, NULL, &ret);
	Dp(context);
	init_kern(context);
	cl_command_queue queue = clCreateCommandQueue(context, dId, 0, &ret);
	Dp(queue);

	build(dId);
	param(context, queue);
}
