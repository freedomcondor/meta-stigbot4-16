#include"function_opengl.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	int left, right;

	function_init();

	while (scanf("%d %d",&left,&right) == 2)
	{
		function_step(left,right);
		printf("left = %d, right = %d\n",left,right);
	}

	function_exit();
}
