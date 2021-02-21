#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {

	size_t count;
	size_t buffer_size;

	void *_array;
	size_t _allocated;
	size_t _element_size;

} Array;

bool array_create(Array *array, size_t element_size, size_t allocate);
bool array_realloc(Array *array, size_t new_allocated);
bool array_extend(Array *array);
void array_free(Array *array);

void array_copy(Array *to, Array *from);

void* array_get_pointer(const Array *array, size_t pos);
bool array_set(Array *array, size_t pos, const void *element);
bool array_get(const Array *array, size_t pos, void *element);

bool array_fill(Array *array, const void *element);
bool array_generate(Array *array, size_t count, void (*f)(size_t, void*));

bool array_push_back(Array *array, void *element);
void array_pop_back(Array *array);

void array_map(Array *array, void (*f)(void *));
void array_where(Array *array, bool (*h)(void*));
void array_reduce(Array *array, void *start, void (*f)(void*, void*, void*), void* result);

bool array_concat(Array *dest, Array *a, Array *b);
void array_qsort(Array *array, size_t first, size_t last, bool (*less)(void*,void*));

#endif // ARRAY_H
