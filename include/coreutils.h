#pragma once	

#include <stdint.h>

typedef struct{
	void* data;
	int size;
	int capacity;	
} dynamic_array_t;

int delete_array(dynamic_array_t arr);
dynamic_array_t create_array(int size, int value_type_size);
uint8_t check_array_capacity(dynamic_array_t* arr, int element_size);
//uint8_t add_to_array(dynamic_array_t arr, void* value); - i'm too lazy to fix it now

