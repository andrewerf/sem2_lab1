#include <string.h>

#include "polynomial.h"

size_t max(size_t a, size_t b)
{
    if(a > b)
        return a;
    else
        return b;
}

size_t min(size_t a, size_t b)
{
    if(a < b)
        return a;
    else
        return b;
}

void stupid_pow(void *res, void *val, size_t val_size, size_t n, void (*mul_func)(void*, void*, void*))
{
    memmove(res, val, val_size);
    for(size_t i = 1; i < n; ++i)
    {
        mul_func(res, res, val);
    }
}

bool polynomial_create(Polynomial *p, polynomial_type type)
{
    size_t element_size;
    void (*sum_func)(void*, void*, void*),
         (*mul_func)(void*, void*, void*),
         (*sub_func)(void*, void*, void*);
    void *zero, *one;

    switch (type) {
        case Double:
            element_size = sizeof(double);
            sum_func = double_add;
            mul_func = double_mul;
            sub_func = double_subtract;

            zero = malloc(element_size);
            one = malloc(element_size);
            *((double*)zero) = 0;
            *((double*)one) = 0;

            break;

        case DComplex:
            element_size = sizeof(DComplex_t);
            sum_func = dcompelx_add;
            mul_func = dcomplex_mul;
            sub_func = dcomplex_subtract;

            DComplex_t t;
            zero = malloc(element_size);
            one = malloc(element_size);

            t.real = 0;
            t.imaginary = 0;
            memcpy(zero, &t, element_size);
            t.real = 1;
            memcpy(one, &t, element_size);

            break;

        default:
            return false;
    }

    Array *array = malloc(sizeof(Array));
    array_create(array, element_size, 1);
    array_fill(array, zero);
    p->coefficients = array;
    p->add_func = sum_func;
    p->mul_func = mul_func;
    p->sub_func = sub_func;
    p->zero = zero;
    p->one = one;

    return true;
}

void polynomial_copy(Polynomial *to, Polynomial *from)
{
    to->coefficients = malloc(sizeof(Array));
    array_copy(to->coefficients, from->coefficients);
    to->sub_func = from->sub_func;
    to->mul_func = from->mul_func;
    to->add_func = from->add_func;
    to->zero = malloc(from->coefficients->_element_size);
    memcpy(to->zero, from->zero, from->coefficients->_element_size);
    to->one = malloc(from->coefficients->_element_size);
    memcpy(to->one, from->one, from->coefficients->_element_size);
    to->type = from->type;
}


void polynomial_add(Polynomial *p_res, Polynomial *p1, Polynomial *p2)
{
    array_realloc(p_res->coefficients, max(p1->coefficients->count, p2->coefficients->count));
    array_fill(p_res->coefficients, p_res->zero);

    if(p1->coefficients->count > p2->coefficients->count)
    {
        Polynomial *t = p1;
        p1 = p2;
        p2 = t;
    }

    size_t i = 0;
    for(; i < p1->coefficients->count; ++i)
    {
        void *k_res = malloc(p_res->coefficients->_element_size),
             *k1 = malloc(p1->coefficients->_element_size),
             *k2 = malloc(p2->coefficients->_element_size);

        array_get(p1->coefficients, i, k1);
        array_get(p2->coefficients, i, k2);
        p_res->add_func(k_res, k1, k2);
        array_set(p_res->coefficients, i, k_res);

        free(k1);
        free(k2);
        free(k_res);
    }

    for(; i < p2->coefficients->count; ++i)
    {
        array_set(p_res->coefficients, i, array_get_pointer(p2->coefficients, i));
    }
}

void polynomial_subtract(Polynomial *p_res, Polynomial *p1, Polynomial *p2)
{
    Polynomial *copy = malloc(sizeof(Polynomial));
    polynomial_copy(copy, p2);

    for(size_t i = 0; i < copy->coefficients->count; ++i)
    {
        void *val = malloc(copy->coefficients->_element_size);
        p2->sub_func(array_get_pointer(copy->coefficients, i), copy->zero, array_get_pointer(copy->coefficients, i));
        free(val);
    }

    polynomial_add(p_res, p1, copy);
    polynomial_free(copy);
}

void polynomial_substitute(Polynomial *p, void *y, void *x)
{
    void *x_i = malloc(p->coefficients->_element_size);
    memcpy(y, p->zero, p->coefficients->_element_size);
    memcpy(x_i, p->one, p->coefficients->_element_size);

    for(size_t i = 0; i < p->coefficients->count; ++i)
    {
        void *t = malloc(p->coefficients->_element_size);

        p->mul_func(t, x_i, array_get_pointer(p->coefficients, i));
        p->add_func(y, y, t);
        p->mul_func(x_i, x_i, x);

        free(t);
    }
}

void double_add(double *res, double *a, double *b)
{*res = *a + *b;}

void double_subtract(double *res, double *a, double *b)
{*res = *a - *b;}

void double_mul(double *res, double *a, double *b)
{*res = *a * *b;}

void dcompelx_add(DComplex_t *res, DComplex_t *a, DComplex_t *b)
{
    double_add(&res->real, &a->real, &b->real);
    double_add(&res->imaginary, &a->imaginary, &b->imaginary);
}

void dcomplex_subtract(DComplex_t *res, DComplex_t *a, DComplex_t *b)
{
    double_subtract(&res->real, &a->real, &b->real);
    double_subtract(&res->imaginary, &a->imaginary, &b->imaginary);
}

void dcomplex_mul(DComplex_t *res, DComplex_t *a, DComplex_t *b)
{
    res->real = a->real * b->real - a->imaginary * b->imaginary;
    res->imaginary = a->imaginary * b->real + b->imaginary * a->real;
}

void polynomial_free(Polynomial *p)
{
    array_free(p->coefficients);
    free(p->one);
    free(p->zero);
}
