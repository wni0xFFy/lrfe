#include <coreutils.h>
#include <stdlib.h>
#include <stdint.h>

int delete_array(dynamic_array_t arr){
	free(arr.data);
	return 0;
}

dynamic_array_t create_array(int size, int value_type_size){
	dynamic_array_t arr = {0};
	arr.data = malloc(size * value_type_size);
	arr.size = 0;
	arr.capacity = size;
	return arr;
}

// uint8_t add_to_array(dynamic_array_t arr, uint8_t* value){
// 	if(arr.capacity <= arr.size + 1){
// 		void* tmp = realloc(arr.data, arr.capacity + 16);
// 		if(tmp == NULL) return 1;
// 		arr.data = tmp;
// 	}
// 	uint8_t* p = arr.data;
// 	*(p + arr.size - 1) = (uint8_t*)value;
// 	arr.size += 1;

// 	return 0;
// }