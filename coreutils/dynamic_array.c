#include <coreutils.h>
#include <stdlib.h>

int delete_array(dynamic_array_t arr){
	free(arr.data);
	return 0;
}