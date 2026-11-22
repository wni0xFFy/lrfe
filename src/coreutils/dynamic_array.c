#include <coreutils.h>
#include <stdlib.h>
#include <stdint.h>

int delete_array(dynamic_array_t arr){
	free(arr.data);
	return 0;
}

dynamic_array_t create_array(int capacity, int value_type_size){
	dynamic_array_t arr = {0};
	arr.data = malloc(capacity * value_type_size);
	arr.size = 0;
	arr.capacity = capacity;
	return arr;
}

uint8_t check_array_capacity(dynamic_array_t* arr, int element_size){
	if(arr->size + 5 >= arr->capacity){
		void* tmp = realloc(arr->data, element_size * (arr->size + 16));
		if(tmp == NULL) return 1;
		arr->data = tmp;
		arr->capacity = arr->size + 16;
	}
	return 0;
}

//maybe i'll write it
// uint8_t push_to_array(dynamic_array_t* arr, uint8_t* value, int element_size){
// 	check_array_capacity(arr, element_size);

// 	uint8_t* p = arr->data;
// 	*(p + arr->size - 1) = (uint8_t*)value;
// 	arr->size += element_size;

// 	return 0;
// }