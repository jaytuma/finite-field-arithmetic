#include "lib_vec.h"

// - - vector data structure - - //
/* holds a dynamically allocated vector of mpz

/* the data structure need to be initialized through a pointer
 * from that moment on the vector is a pointer to _vec_t and 
 * values can be modified without passing a pointer.
 */	

void vec_mpz_init(vec_mpz_t* vec_pt)
{
	*vec_pt = malloc(sizeof(**vec_pt));
	vec_mpz_t vec = *vec_pt;
	
	//parameter allocation
	vec -> data = malloc( INITIAL_MEMORY*sizeof(mpz_t) );
	vec -> len = 0;
	vec -> size = INITIAL_MEMORY;
}

void vec_mpz_clear(vec_mpz_t* vec_pt)
{
	vec_mpz_t vec = *vec_pt;
	
	//parameter deallocation
	free(vec -> data);
	
	//struct deallocation
	free(vec);
}

void _vec_mpz_increase(vec_mpz_t vec)
{
	//reallocate more memory if needed
	if(vec -> len == vec -> size)
	{
		vec -> size *= 2;
		vec -> data = realloc( vec -> data, (vec -> size) * sizeof(mpz_t) );		
	}
}
/* INTERNAL_USE: increase the size of a vector if
 * there is not enough allocated space
 */

void _vec_mpz_decrease(vec_mpz_t vec)
{
	//deallocate memory once not needed anymore
	if((vec -> len < (vec -> size)/4) && (vec -> size >= 4))
	{
		vec -> size /= 2;
		vec -> data = realloc( vec -> data, vec -> size * sizeof(mpz_t));
	}
}
/* INTERNAL_USE: decrease the size of a vector if
 * there is too much space allocated
 */

//specific function

#define MCR_vec_mpz_append(MCR_func_name, MCR_type, MCR_mpz_set)		\
	void MCR_func_name(vec_mpz_t vec, MCR_type num)						\
	{																	\
		_vec_mpz_increase(vec);											\
		mpz_init(vec -> data[vec -> len]);								\
		MCR_mpz_set(vec -> data[vec -> len], num);						\
		vec -> len ++;													\
	}
/* Given a vector and a number, put it at the
 * end of the array
 */

	//to append mpz_t
MCR_vec_mpz_append(vec_mpz_append, 		mpz_t, 				mpz_set);
	//to append unsigned integer
MCR_vec_mpz_append(vec_mpz_append_ui,	unsigned long int, 	mpz_set_ui);
	//to append signed integer
MCR_vec_mpz_append(vec_mpz_append_si, 	signed long int, 	mpz_set_si);


void vec_mpz_get(mpz_t out, vec_mpz_t vec, int i)
{
	//sanity check in debug mode
	MY_ASSERT(i >= 0, "accessing in vec_mpz_get vec_mpz with negative index");
	MY_ASSERT(i < vec -> len, "accessing in vec_mpz_get vec_mpz out of boundary");
	
	mpz_set( out, (vec -> data)[i] );
}
/* Given a vector and an index i in the boundary
 * of the vector return the mpz in place i
 */

#define MCR_vec_mpz_set(MCR_func_name, MCR_type, MCR_mpz_set)								\
	void MCR_func_name(vec_mpz_t vec, int i, MCR_type num)									\
	{																						\
		MY_ASSERT(i >= 0, "accessing in MCR_func_name vec_mpz with negative index!");		\
		MY_ASSERT(i < vec -> len, "accessing in MCR_func_name vec_mpz out of boundary!");	\
																							\
		MCR_mpz_set( (vec -> data)[i], num);												\
	}
/* Given a vector and an index i in the boundary
 * set the vector value in place i to the mpz in input
 */

	//to set a mpz_t
MCR_vec_mpz_set(vec_mpz_set,	mpz_t,				mpz_set);
	//to set an unsigned integer
MCR_vec_mpz_set(vec_mpz_set_ui,	unsigned long int,	mpz_set_ui);
	//to set an unsigned integer
MCR_vec_mpz_set(vec_mpz_set_si, signed long int,	mpz_set_si);


void vec_mpz_pop(mpz_t out, vec_mpz_t vec, int i)
{
	MY_ASSERT(i >= 0, "accessing in vec_mpz_pop vec_mpz with negative index");
	MY_ASSERT(i < vec -> len, "accessing in vec_mpz_pop vec_mpz out of boundary");
	
	//in case one don't want to save the output data
	if(out != NULL)
	{
		vec_mpz_get(out, vec, i);
	}
	
	//Todo, Warning, bad implementation, too expensive.
	int j;
	for(j = i; j < (vec -> len) - 1; j++)
	{	
		mpz_swap(vec -> data[j], vec -> data[j+1]);
	}
	mpz_clear(vec -> data[vec -> len - 1]);
	
	//length decrease by one
	vec -> len --;
	_vec_mpz_decrease(vec);
}
/*	Given a vector and an index, it write on out the value
 * 	held in position i, remove the i-th entry and shift the other
 *	entries
 */

#define MCR_vec_mpz_insert(MRC_func_name, MCR_type, MCR_mpz_set)								\
	void MRC_func_name(vec_mpz_t vec, int i, MCR_type num)										\
	{																							\
		MY_ASSERT(i >= 0, "accessing in MRC_func_name vec_mpz with negative index");			\
		MY_ASSERT(i <= vec -> len, "accessing in MRC_func_name vec_mpz out of boundary");		\
																								\
		vec -> len ++;																			\
		_vec_mpz_increase(vec);																	\
		mpz_init(vec -> data[(vec -> len) - 1]);												\
																								\
		int j;																					\
		for(j = (vec -> len) - 2; j >= i; j--)													\
		{																						\
			mpz_swap(vec -> data[j+1], vec -> data[j]);											\
		}																						\
																								\
		MCR_mpz_set(vec -> data[i], num);														\
	}																							

MCR_vec_mpz_insert(vec_mpz_insert,		mpz_t,				mpz_set);
MCR_vec_mpz_insert(vec_mpz_insert_ui,	unsigned long int,	mpz_set_ui);
MCR_vec_mpz_insert(vec_mpz_insert_si,	signed long int,	mpz_set_si);

/*  Given a vector, index i, mpz_t inset the element num at
 *  position i, incrementing by one the position of elements
 *  that wera at a position grater that i
 */

int vec_mpz_count(vec_mpz_t vec, mpz_t num)
{
	int i, cnt = 0;
	for(i = 0; i < vec -> len; i++)
	{
		if(mpz_cmp(vec -> data[i], num) == 0)
		{
			cnt ++;
		}
	}
	
	return cnt;
}
/* Count the number of elements equal to the given mpz num
 * occurring in vec
 */

int vec_mpz_index(vec_mpz_t vec, mpz_t num)
{
	int i;
	while((mpz_cmp(vec -> data[i], num) != 0) && (i < vec -> len))
	{
		i++;	
	}
	
	if(mpz_cmp(vec -> data[i], num) == 0)
	{
		return i;	
	}
	else
	{
		return -1;
	}
}
/* look for the first occurrence of num in vec. If there is no
 * occurrence will return a negative value (-1)
 */
	
void vec_mpz_reverse(vec_mpz_t vec)
{
	int i, n = vec -> len;
	for(i = 0; i < n/2; i++);
	{
		mpz_swap(vec -> data[i], vec -> data[n - i - 1]);	
	}
}
/* reverse the order of the vector
 */

	
	
// - - - debug use only - - -

void vec_mpz_string(char* out, vec_mpz_t vec)
{
	int i;
	mpz_t tmp;
	mpz_init(tmp);
	char str[1024];
	
	out[0] = '[';
	out[1] = '\0';

	if(vec -> len > 0)
	{
		//first element
		vec_mpz_get(tmp, vec, 0);
		gmp_sprintf(str, "%Zd", tmp);
		strcat(out, str);
		
		for(i = 1; i < (vec -> len); i++)
		{
			vec_mpz_get(tmp, vec, i);
			gmp_sprintf(str, ", %Zd", tmp);
			strcat(out, str);
		}
	}
	
	strcat(out, "]");
	mpz_clear(tmp);
}

void vec_mpz_print(vec_mpz_t vec)
{
	char str[65536];
	vec_mpz_string(str, vec);
	printf("vec = %s\nlen = %d,\tsize = %d\n\n", str, vec -> len, vec -> size);
}

int vec_mpz_typecheck(vec_mpz_t vec)
{
	return 1;
}

// - - - no main - - - //