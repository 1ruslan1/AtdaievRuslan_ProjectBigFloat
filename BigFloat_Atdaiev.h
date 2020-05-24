#pragma once

#ifndef BIGFLOAT_H
#define ADD_H
struct BigFloat;
typedef struct BigFloat BigFloat;

bool MIN(int first, int second);

int abs_compare(BigFloat* first, BigFloat* second);
int compare(BigFloat* first, BigFloat* second);

unsigned int* to_add_arr(BigFloat* num);

void free_float(BigFloat* num);

BigFloat from_add_arr(int* arr);

BigFloat add_abs(BigFloat* first, BigFloat* second);

BigFloat sub_abs(BigFloat* first, BigFloat* second);

BigFloat add(BigFloat* first, BigFloat* second);

BigFloat sub(BigFloat* first, BigFloat* second);

unsigned int* to_mul_arr(BigFloat* num);

BigFloat from_mul_arr(unsigned int* num_arr, int mantissa);


BigFloat abs_multiply(BigFloat* first, BigFloat* second);

BigFloat multiply(BigFloat* first, BigFloat* second);

void shift_arr_left(unsigned int* arr);
void shift_arr_right(unsigned int* arr);

int comp_add_arr(unsigned int* f, unsigned int* s);

void inc(unsigned int* f, int pos);

BigFloat division(BigFloat* first, BigFloat* second);

void add_int(unsigned int* arr, int* len, int incr);

void mul_10(unsigned int* arr, int* len);

BigFloat input(char* num);

char* to_string(BigFloat* a);

BigFloat linear(BigFloat a, BigFloat b);

char* getExp(BigFloat* a);
char* getMant(BigFloat* a);
BigFloat setExp(char* a, size_t n);
BigFloat setMant(char* a, size_t n);

#endif