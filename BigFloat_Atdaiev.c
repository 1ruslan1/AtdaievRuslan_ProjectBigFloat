#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "BigFloat_Atdaiev.h"

int max_length = 10;

long long int_modulo = 10;//(long long)((unsigned int)-1) + 1;

struct BigFloat
{
	int exp;
	unsigned int* exp_data;
	int mant;
	unsigned int* mant_data;
	bool is_positive;
};
typedef struct BigFloat BigFloat;

char* getExp(BigFloat* a);
char* getMant(BigFloat* a);
BigFloat setExp(char* a, size_t n);
BigFloat setMant(char* a, size_t n);

bool MIN(int first, int second)
{
	return first < second ? first : second;
}

int abs_compare(BigFloat* first, BigFloat* second)
{
	if (first->exp > second->exp)
	{
		return 1;
	}
	else if (first->exp < second->exp)
	{
		return -1;
	}

	for (int i = first->exp; i >= 0; --i)
	{
		if (first->exp_data[i] > second->exp_data[i])
		{
			return 1;
		}
		else if (first->exp_data[i] < second->exp_data[i])
		{
			return -1;
		}
	}

	for (int i = 0; i < MIN(first->mant, second->mant); ++i)
	{
		if (first->mant_data[i] > second->mant_data[i])
		{
			return 1;
		}
		else if (first->mant_data[i] < second->mant_data[i])
		{
			return -1;
		}
	}
	if (first->mant > second->mant)
	{
		return 1;
	}
	else if (first->mant < second->mant)
	{
		return -1;
	}

	return 0;

}

int compare(BigFloat* first, BigFloat* second)
{
	int cmp = abs_compare(first, second);
	if (first->is_positive && second->is_positive)
	{
		return cmp;
	}
	else if (!first->is_positive && !second->is_positive)
	{
		return (-1) * cmp;
	}
	else
	{
		return first->is_positive ? 1 : -1;
	}
}

unsigned int* to_add_arr(BigFloat* num)
{
	unsigned int* res = malloc(sizeof(int) * 2 * max_length);
	for (int i = 0; i < 2 * max_length; ++i)
	{
		res[i] = 0;
	}
	for (int i = 0; i < num->exp; ++i)
	{
		res[max_length - 1 - i] = num->exp_data[i];
	}
	for (int i = 0; i < num->mant; ++i)
	{
		res[max_length + i] = num->mant_data[i];
	}

	return res;
}

void free_float(BigFloat* num)
{
	free(num->exp_data);
	free(num->mant_data);
}

BigFloat from_add_arr(int* arr)
{
	BigFloat res;
	res.exp_data = (unsigned int*)malloc(sizeof(int) * max_length);
	res.mant_data = (unsigned int*)malloc(sizeof(int) * max_length);
	res.exp = max_length;
	res.mant = max_length;
	for (int i = 0; i < max_length - 1; ++i)
	{
		if (arr[i] == 0)
		{
			res.exp--;
		}
		else
		{
			break;
		}
	}
	for (int i = 2 * max_length - 1; i > max_length; --i)
	{
		if (arr[i] == 0)
		{
			res.mant--;
		}
		else
		{
			break;
		}
	}
	for (int i = 0; i < res.exp; ++i)
	{
		res.exp_data[i] = arr[max_length - 1 - i];
	}
	for (int i = 0; i < res.mant; ++i)
	{
		res.mant_data[i] = arr[max_length + i];
	}
	return res;
}

BigFloat add_abs(BigFloat* first, BigFloat* second)
{
	BigFloat res;

	res.is_positive = true;
	unsigned int* f = to_add_arr(first);
	unsigned int* s = to_add_arr(second);
	unsigned int* res_arr = malloc(sizeof(int) * 2 * max_length);
	for (int i = 0; i < 2 * max_length; ++i)
	{
		res_arr[i] = 0;
	}
	long long leftover = 0;
	for (int i = 2 * max_length - 1; i >= 0; --i)
	{
		long long r = (long long)f[i] + (long long)s[i] + leftover;
		res_arr[i] = r % int_modulo;
		leftover = r / int_modulo;
	}
	res = from_add_arr(res_arr);
}

BigFloat sub_abs(BigFloat* first, BigFloat* second)
{
	BigFloat res;
	res.exp_data = (unsigned int*)malloc(sizeof(int) * max_length);
	res.mant_data = (unsigned int*)malloc(sizeof(int) * max_length);
	res.is_positive = true;
	unsigned int* f = to_add_arr(first);
	unsigned int* s = to_add_arr(second);
	unsigned int* res_arr = malloc(sizeof(int) * 2 * max_length);
	for (int i = 0; i < 2 * max_length; ++i)
	{
		res_arr[i] = 0;
	}
	long long leftover = 0;
	for (int i = 2 * max_length - 1; i >= 0; --i)
	{
		long long r = (long long)f[i] - (long long)s[i] - leftover;
		if (r < 0)
		{
			r += int_modulo;
			leftover = 1;
		}
		else
		{
			leftover = 0;
		}
		res_arr[i] = r % int_modulo;
	}
	res = from_add_arr(res_arr);
}

BigFloat add(BigFloat* first, BigFloat* second)
{
	if (first->is_positive && second->is_positive)
	{
		return add_abs(first, second);
	}
	if (!first->is_positive && !second->is_positive)
	{
		BigFloat res = add_abs(first, second);
		res.is_positive = false;
		return res;
	}
	int cmp = abs_compare(first, second);
	BigFloat res;
	if (cmp > 0)
	{
		res = sub_abs(first, second);
		res.is_positive = first->is_positive;
	}
	else if (cmp < 0)
	{
		res = sub_abs(second, first);
		res.is_positive = second->is_positive;
	}
	else
	{
		res.exp = 1;
		res.mant = 1;
		res.is_positive = true;
		res.exp_data = (unsigned int*)malloc(max_length * sizeof(int));
		res.mant_data = (unsigned int*)malloc(max_length * sizeof(int));
		for (int i = 0; i < max_length; i++) res.exp_data[i] = (unsigned int)0;
		for (int i = 0; i < max_length; i++) res.mant_data[i] = (unsigned int)0;
	}
	return res;
}

BigFloat sub(BigFloat* first, BigFloat* second)
{
	second->is_positive = !second->is_positive;
	BigFloat res = add(first, second);
	second->is_positive = !second->is_positive;
	return res;
}

unsigned int* to_mul_arr(BigFloat* num)
{
	int pos = 0;
	unsigned int* res_arr = malloc(sizeof(int) * 2 * max_length);
	for (int i = 0; i < 2 * max_length; ++i)
	{
		res_arr[i] = 0;
	}
	for (int i = num->mant - 1; i >= 0; --i)
	{
		res_arr[pos] = num->mant_data[i];
		pos++;
	}
	for (int i = 0; i < num->exp; ++i)
	{
		res_arr[pos] = num->exp_data[i];
		pos++;
	}

	return res_arr;
}

BigFloat from_mul_arr(unsigned int* num_arr, int mantissa)
{
	BigFloat res;
	res.exp_data = (unsigned int*)malloc(sizeof(int) * max_length);
	res.mant_data = (unsigned int*)malloc(sizeof(int) * max_length);
	for (int i = 0; i < 2 * max_length; ++i)
	{
		res.exp_data[i] = res.mant_data[i] = 0;
	}
	int len = 2 * max_length;
	for (int i = 2 * max_length - 1; i >= 0; --i)
	{
		if (num_arr[i] == 0)
		{
			len--;
		}
		else
		{
			break;
		}
	}
	res.mant = mantissa;
	res.exp = len - mantissa;
	int pos = 0;
	for (int i = res.mant - 1; i >= 0; --i)
	{
		res.mant_data[i] = num_arr[pos];
		pos++;
	}
	while (res.mant_data[res.mant - 1] == 0)
		res.mant--;
	for (int i = 0; i < res.exp; ++i)
	{
		res.exp_data[i] = num_arr[pos];
		pos++;
	}
	if (res.exp <= 0)
	{
		res.exp = 1;
	}
	if (res.mant <= 0)
	{
		res.mant = 1;
	}
	return res;
}


BigFloat abs_multiply(BigFloat* first, BigFloat* second)
{
	BigFloat res;
	res.is_positive = true;
	unsigned int* f = to_mul_arr(first);
	unsigned int* s = to_mul_arr(second);
	unsigned int* res_arr = malloc(sizeof(int) * 2 * max_length);
	for (int i = 0; i < 2 * max_length; ++i)
	{
		res_arr[i] = 0;
	}
	long long leftover = 0;
	for (int i = 0; i < 2 * max_length; ++i)
	{
		for (int j = 0; j < 2 * max_length; ++j)
		{
			if (i + j < 2 * max_length)
			{
				long long r = (long long)res_arr[i + j] + (long long)f[i] * (long long)s[j] + leftover;
				leftover = r / int_modulo;
				res_arr[i + j] = (unsigned int)(r % int_modulo);
			}

		}
	}
	res = from_mul_arr(res_arr, first->mant + second->mant);
	return res;
}

BigFloat multiply(BigFloat* first, BigFloat* second)
{
	BigFloat res = abs_multiply(first, second);
	if (first->is_positive != second->is_positive)
	{
		res.is_positive = false;
	}
	return res;
}

void shift_arr_left(unsigned int* arr)
{
	for (int i = 0; i < 2 * max_length - 1; ++i)
	{
		arr[i] = arr[i + 1];
	}
	arr[2 * max_length - 1] = 0;
}

void shift_arr_right(unsigned int* arr)
{
	for (int i = 2 * max_length; i > 0; --i)
	{
		arr[i] = arr[i - 1];
	}
	arr[0] = 0;
}

int comp_add_arr(unsigned int* f, unsigned int* s)
{
	for (int i = 0; i < 2 * max_length; ++i)
	{
		if (f[i] < s[i])
		{
			return -1;
		}
		if (f[i] > s[i])
		{
			return 1;
		}
	}
	return 0;
}

void inc(unsigned int* f, int pos)
{
	long long leftover = 1;
	for (int i = max_length - pos; i >= 0; --i)
	{
		long long r = (long long)f[i] + leftover;
		f[i] = (unsigned int)(r % int_modulo);
		leftover = (r / int_modulo);
	}
}

BigFloat division(BigFloat* first, BigFloat* second)
{
	BigFloat res;
	res.exp_data = (unsigned int*)malloc(sizeof(int) * max_length);
	res.mant_data = (unsigned int*)malloc(sizeof(int) * max_length);
	res.is_positive = true;
	unsigned int* f = to_add_arr(first);
	unsigned int* s = to_add_arr(second);
	unsigned int* res_arr = malloc(sizeof(int) * 2 * max_length);
	for (int i = 0; i < 2 * max_length; ++i)
	{
		res_arr[i] = 0;
	}
	long long leftover = 0;
	unsigned int* null_arr = (unsigned int*)malloc(sizeof(int) * 2 * max_length);
	for (int i = 0; i < 2 * max_length; ++i)
	{
		null_arr[i] = 0;
	}
	int pos = 1;
	if (comp_add_arr(s, null_arr) == 0)
	{
		printf("division by 0\n");
		return setExp("0", 0);
	}
	while (comp_add_arr(f, null_arr) != 0)
	{
		while (comp_add_arr(f, s) != -1)
		{
			long long leftover = 0;
			for (int i = 2 * max_length - 1; i >= 0; --i)
			{
				long long r = (long long)f[i] - (long long)s[i] - leftover;
				if (r < 0)
				{
					r += int_modulo;
					leftover = 1;
				}
				else
				{
					leftover = 0;
				}
				f[i] = r % int_modulo;
			}
			if (pos + max_length <= 0)
			{
				break;
			}
			inc(res_arr, pos);
		}
		if (pos + max_length <= 0)
		{
			break;
		}
		shift_arr_left(f);
		pos--;
	}
	return from_add_arr(res_arr);
}

void add_int(unsigned int* arr, int* len, int incr)
{
	long long leftover = incr;
	for (int i = 0; i < *len; ++i)
	{
		long long r = (long long)arr[i] + leftover;
		arr[i] = r % int_modulo;
		leftover = r / int_modulo;
	}
	if (leftover != 0)
	{
		arr[*len++] = leftover;
	}
}

void mul_10(unsigned int* arr, int* len)
{
	long long leftover = 0;
	for (int i = 0; i < *len; ++i)
	{
		long long r = (long long)arr[i] * (long long)10 + leftover;
		arr[i] = r % int_modulo;
		leftover = r / int_modulo;
	}
	if (leftover != 0)
	{
		arr[(*len)++] = leftover;
	}
}

BigFloat input(char* num)
{
	int pos = 0;
	BigFloat res;
	int len = strlen(num);
	if (num[0] == '-')
	{
		res.is_positive = false;
		pos++;
	}
	else
	{
		res.is_positive = true;
	}
	res.exp = 1;
	res.exp_data = malloc(sizeof(int) * max_length);
	for (int i = 0; i < max_length; ++i)
	{
		res.exp_data[i] = 0;
	}
	bool f = true;
	while (num[pos] != '.')
	{
		if (!f)
		{
			mul_10(res.exp_data, &res.exp);
		}
		else
		{
			f = false;
		}
		add_int(res.exp_data, &res.exp, num[pos] - '0');
		pos++;
	}
	int dot_pos = pos;
	pos++;
	f = true;
	res.mant = 1;
	res.mant_data = malloc(sizeof(int) * max_length);
	for (int i = 0; i < max_length; ++i)
	{
		res.mant_data[i] = 0;
	}
	while (pos != len)
	{
		if (!f)
		{
			mul_10(res.mant_data, &res.mant);
		}
		else
		{
			f = false;
		}
		add_int(res.mant_data, &res.mant, num[pos] - '0');
		pos++;
	}
	for (int i = 0; i < res.mant / 2; ++i)
	{
		unsigned c = res.mant_data[i];
		res.mant_data[i] = res.mant_data[res.mant - 1 - i];
		res.mant_data[res.mant - 1 - i] = c;
	}
	return res;
}

char* to_string(BigFloat* a)
{
	char* num = malloc(sizeof(char) * 11 * max_length);
	int pos = 0;
	if (!a->is_positive)
	{
		num[pos++] = '-';
	}
	for (int i = 0; i < a->exp; ++i)
	{
		unsigned int c = a->exp_data[i];
		do
		{
			num[pos++] = c % 10 + '0';
			c /= 10;
		} while (c);
	}

	for (int i = num[0] == '-' ? 1 : 0; pos - i > i; ++i)
	{
		char c = num[i];
		num[i] = num[pos - 1 - i];
		num[pos - 1 - i] = c;
	}
	int point_pos = pos;
	num[pos++] = '.';
	for (int i = a->mant - 1; i >= 0; --i)
	{
		unsigned int c = a->mant_data[i];
		int beg_pos = pos;

		do {
			num[pos++] = c % 10 + '0';
			c /= 10;
		} while (c);
		for (int j = 0; beg_pos + j < pos - 1 - j; ++j)
		{
			int c = num[beg_pos + j];
			num[beg_pos + j] = num[pos - 1 - j];
			num[pos - 1 - j] = c;
		}
	}
	for (int i = 0; point_pos + 1 + i < pos - i - 1; ++i)
	{
		char c = num[point_pos + 1 + i];
		num[point_pos + 1 + i] = num[pos - 1 - i];
		num[pos - 1 - i] = c;
	}
	num[pos] = '\0';
	return num;
}

BigFloat linear(BigFloat a, BigFloat b)
{
	b.is_positive = !b.is_positive;
	BigFloat res = division(&b, &a);
	b.is_positive = !b.is_positive;
	return res;
}

char* getExp(BigFloat* a)
{
	char* num = malloc(sizeof(char) * 11 * max_length);
	int pos = 0;
	if (!a->is_positive)
	{
		num[pos++] = '-';
	}
	for (int i = 0; i < a->exp; ++i)
	{
		unsigned int c = a->exp_data[i];
		do
		{
			num[pos++] = c % 10 + '0';
			c /= 10;
		} while (c);
	}

	for (int i = num[0] == '-' ? 1 : 0; pos - i > i; ++i)
	{
		char c = num[i];
		num[i] = num[pos - 1 - i];
		num[pos - 1 - i] = c;
	}
	int point_pos = pos;
	num[pos] = '\0';
	return num;
}
char* getMant(BigFloat* a)
{
	char* num = malloc(sizeof(char) * 11 * max_length);
	int pos = 0;
	for (int i = a->mant - 1; i >= 0; --i)
	{
		unsigned int c = a->mant_data[i];
		int beg_pos = pos;

		do {
			num[pos++] = c % 10 + '0';
			c /= 10;
		} while (c);
		for (int j = 0; beg_pos + j < pos - 1 - j; ++j)
		{
			int c = num[beg_pos + j];
			num[beg_pos + j] = num[pos - 1 - j];
			num[pos - 1 - j] = c;
		}
	}
	for (int i = 0; i < pos - i - 1; ++i)
	{
		char c = num[i];
		num[i] = num[pos - 1 - i];
		num[pos - 1 - i] = c;
	}
	num[pos] = '\0';
	return num;
}
BigFloat setExp(char* a, size_t n)
{
	int pos = 0;
	BigFloat res;
	res.is_positive = true;
	res.exp = 1;
	res.exp_data = malloc(sizeof(int) * max_length);
	for (int i = 0; i < max_length; ++i)
	{
		res.exp_data[i] = 0;
	}
	bool f = true;
	while (pos < n)
	{
		if (!f)
		{
			mul_10(res.exp_data, &res.exp);
		}
		else
		{
			f = false;
		}
		add_int(res.exp_data, &res.exp, a[pos] - '0');
		pos++;
	}
	int dot_pos = pos;
	pos++;
	f = true;
	res.mant = 1;
	res.mant_data = malloc(sizeof(int) * max_length);
	for (int i = 0; i < max_length; ++i)
	{
		res.mant_data[i] = 0;
	}
	return res;
}
BigFloat setMant(char* a, size_t n)
{
	int pos = 0;
	BigFloat res;
	int len = n;
	res.is_positive = true;
	res.exp = 1;
	res.exp_data = malloc(sizeof(int) * max_length);
	for (int i = 0; i < max_length; ++i)
	{
		res.exp_data[i] = 0;
	}
	bool f = true;
	f = true;
	res.mant = 1;
	res.mant_data = malloc(sizeof(int) * max_length);
	for (int i = 0; i < max_length; ++i)
	{
		res.mant_data[i] = 0;
	}
	while (pos != len)
	{
		if (!f)
		{
			mul_10(res.mant_data, &res.mant);
		}
		else
		{
			f = false;
		}
		add_int(res.mant_data, &res.mant, a[pos] - '0');
		pos++;
	}
	for (int i = 0; i < res.mant / 2; ++i)
	{
		unsigned c = res.mant_data[i];
		res.mant_data[i] = res.mant_data[res.mant - 1 - i];
		res.mant_data[res.mant - 1 - i] = c;
	}
	return res;
}

int main()
{
	char* a1="12", b1="0";
	//gets(&a1);
	//gets(&b1);

	BigFloat a = setExp(a1, 2);
	BigFloat b = setMant(b1, 0);
	BigFloat c = add(&a, &b);
	printf("%s.%s\n", getExp(&a), getMant(&a));
	printf("%s + %s = %s\n", to_string(&a), to_string(&b), to_string(&c));
	c = sub(&a, &b);
	printf("%s - %s = %s\n", to_string(&a), to_string(&b), to_string(&c));
	//free_float(&c);
	a = input("123.23");
	b = input("0.334432");
	c = multiply(&a, &b);
	printf("%s * %s = %s\n", to_string(&a), to_string(&b), to_string(&c));
	//free_float(&c);
	c = division(&a, &b);
	printf("%s / %s = %s\n", to_string(&a), to_string(&b), to_string(&c));
	system("pause");

}