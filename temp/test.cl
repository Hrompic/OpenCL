__kernel void test(__global int *message)
{
	int gid = get_global_id(0);
	for(int i=0; i<(1<<24); i++)
		message[gid] += gid*sin((float)i);
}
