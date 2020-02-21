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
	else
		return false;
}

bool array_realloc(Array *array, size_t new_allocated)
{
	void *temp = malloc(new_allocated*array->_element_size);
	if(temp == NULL)
		return false;

	memmove(temp, array->_array, array->count * array->_allocated);
	free(array->_array);
	array->_array = temp;
	array->_allocated = new_allocated;

	return true;
}

bool array_extend(Array *array)
{
	return array_realloc(array, array->_allocated + array->_buffer_size);
}


void array_set(Array *array, size_t pos, const void *element)
{
	memcpy((char*)array->_array + pos*array->_element_size, element, array->_element_size);
}

void array_get(Array *array, size_t pos, void *element)
{
	memcpy(element, (char*)array->_array + pos*array->_element_size, array->_element_size);
}


void array_fill(Array *array, const void *element)
{
	for(size_t i = 0; i < array->_allocated; i += 1){
		array_set(array, i, element);
	}
	array->count = array->_allocated;
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
