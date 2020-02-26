#include <string.h>

#include "array.h"


void swap(void *a, void *b, size_t size)
{
	void *temp = malloc(size);
	memcpy(temp, a, size);
	memcpy(a, b, size);
	memcpy(b, temp, size);

	free(temp);
}


bool array_create(Array *array, size_t element_size, size_t allocate)
{
	if(element_size == 0)
		return false;

	array->count = 0;
	array->_element_size = 0;
	array->_allocated = 0;
	if(allocate == 0)
		allocate = array->buffer_size;

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
	return array_realloc(array, array->_allocated + array->buffer_size);
}

void array_free(Array *array)
{
	free(array->_array);
	array->count = 0;
	array->_allocated = 0;
}


void* array_get_pointer(const Array *array, size_t pos)
{
	return (char*)array->_array + pos*array->_element_size;
}

bool array_set(Array *array, size_t pos, const void *element)
{
	if(pos >= array->_allocated)
		return false;
	memcpy(array_get_pointer(array, pos), element, array->_element_size);
	return true;
}

bool array_get(const Array *array, size_t pos, void *element)
{
	if(pos >= array->count)
		return false;
	memcpy(element, array_get_pointer(array, pos), array->_element_size);
	return true;
}


bool array_fill(Array *array, const void *element)
{
	for(array->count = 1; array->count <= array->_allocated; array->count += 1){
		if(!array_set(array, array->count-1, element))
			return false;
	}
	array->count -= 1;
	return true;
}

bool array_generate(Array *array, size_t count, void (*f)(size_t, void*))
{
	if(count > array->_allocated)
		return false;

	for(size_t i = 0; i < count; ++i){
		f(i, array_get_pointer(array, i));
	}
	array->count = count;
	return true;
}


bool array_push_back(Array *array, void *element)
{
	if(array->count == array->_allocated)
		if(!array_extend(array))
			return false;

	array->count += 1;
	array_set(array, array->count-1, element);
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


void array_qsort(Array *array, size_t first, size_t last, bool (*less)(void*,void*))
{
	size_t l = first;
	size_t r = last;

	if(l >= r)
		return;

	size_t m = (l+r)/2;
	do{
		while(less(array_get_pointer(array, l), array_get_pointer(array, m)))
			l += 1;
		while(less(array_get_pointer(array, m), array_get_pointer(array, r)))
			r -= 1;

		if(l <= r){
			swap(array_get_pointer(array, l), array_get_pointer(array, r), array->_element_size);

			l += 1;
			r -= 1;
		}
	} while(l <= r);

	array_qsort(array, first, r, less);
	array_qsort(array, l, last, less);
}














