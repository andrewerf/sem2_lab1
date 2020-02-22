#include <string.h>

#include "array.h"



bool array_create(Array *array, size_t element_size, size_t allocate)
{
	array->count = 0;
	array->_element_size = 0;
	array->_allocated = 0;
	if(allocate == 0)
		allocate = array->_buffer_size;

	array->_array = malloc(allocate * element_size);
	if(array->_array != NULL){
		array->_allocated = allocate;
		array->_element_size = element_size;
		return true;
	}
	else{
		return false;
	}
}

bool array_realloc(Array *array, size_t new_allocated)
{
	void *temp = malloc(new_allocated*array->_element_size);
	if(temp == NULL)
		return false;

	memmove(temp, array->_array, array->count * array->_element_size);
	free(array->_array);
	array->_array = temp;
	array->_allocated = new_allocated;

	return true;
}

bool array_extend(Array *array)
{
	return array_realloc(array, array->_allocated + array->_buffer_size);
}


void* array_get_pointer(const Array *array, size_t pos)
{
	return (char*)array->_array + pos*array->_element_size;
}

void array_set(Array *array, size_t pos, const void *element)
{
	memcpy(array_get_pointer(array, pos), element, array->_element_size);
}

void array_get(const Array *array, size_t pos, void *element)
{
	memcpy(element, array_get_pointer(array, pos), array->_element_size);
}


void array_fill(Array *array, const void *element)
{
	for(size_t i = 0; i < array->_allocated; i += 1){
		array_set(array, i, element);
	}
	array->count = array->_allocated;
}

void array_generate(Array *array, size_t count, void (*f)(size_t, void*))
{
	for(size_t i = 0; i < count; ++i){
		f(i, array_get_pointer(array, i));
	}
	array->count = count;
}


bool array_push_back(Array *array, void *element)
{
	if(array->count == array->_allocated)
		if(!array_extend(array))
			return false;

	array_set(array, array->count, element);
	array->count += 1;
	return true;
}

void array_pop_back(Array *array)
{
	array->count -= 1;
}


void array_map(Array *array, void* (*f)(void*))
{
	for(size_t i = 0; i < array->count; ++i){
		array_set(array, i, f(array_get_pointer(array, i)));
	}
}

void array_where(Array *array, bool (*h)(void*))
{
	size_t last_pos = 0;

	for(size_t i = 0; i < array->count; ++i){

		if(h(array_get_pointer(array, i))){
			if(i > last_pos)
				array_set(array, last_pos, array_get_pointer(array, i));
			++last_pos;
		}

	}

	array->count = last_pos;
}

void array_reduce(Array *array, void *start, void (*f)(void*, void*, void*), void* result)
{
	f(array_get_pointer(array, 0), start, result);
	for(size_t i = 1; i < array->count; ++i){
		f(array_get_pointer(array, i), result, result);
	}
}






















