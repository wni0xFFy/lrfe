#pragma once	

typedef struct{
	void* data;
	int size;
	int capacity;	
} dynamic_array_t;

int delete_array(dynamic_array_t arr);