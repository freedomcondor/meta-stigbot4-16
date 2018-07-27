#include"function.h"

#include <stdio.h>
#include <iostream>

int main(int argc, char* argv[])
{
	int ret;
	ret = function_init();
	if (ret < 0)
		return -1;

	while (1)		// 
	{
		function_step();
	}

	function_exit();
	return 0;
}
