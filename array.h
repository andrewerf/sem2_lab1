#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {

	size_t count;

	size_t _buffer_size;
	void *_array;
	size_t _allocated;
	size_t _element_size;

} Array;

bool array_create(Array *array, size_t element_size, size_t allocate);
bool array_realloc(Array *array, size_t new_allocated);
bool array_extend(Array *array);

void* array_get_pointer(const Array *array, size_t pos);
void array_set(Array *array, size_t pos, const void *element);
void array_get(const Array *array, size_t pos, void *element);

void array_fill(Array *array, const void *element);

bool array_push_back(Array *array, void *element);
void array_pop_back(Array *array);


#endif // ARRAY_H
