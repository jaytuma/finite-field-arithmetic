#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>

#define INITIAL_MEMORY 2

#define ERROR "\n\t\e[1;31mError\e[0m:"

#define DEBUG 1

/*the following macro should not be here but in a different file*/
#ifdef DEBUG
	#define MY_ASSERT(b, s) 								\
			if(!(b))										\
			{												\
				fprintf(stderr, "%s %s!\n", ERROR, s);	\
				fflush(stderr);								\
			}										
#else
	#define MY_ASSERT(b,s);
#endif


// - - vector data structure - - //
/* holds an dynamically allocated vector of mpz

/* the data structure need to be initialized through a pointer
 * from that moment on the vector is a pointer to _vec_t and 
 * values can be modified without passing a pointer.
 */	

typedef struct _vec_mpz_t
{
	int len;
	int size;
	mpz_t* data;
} *vec_mpz_t;

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

MCR_vec_mpz_append(vec_mpz_append, mpz_t, mpz_set);
MCR_vec_mpz_append(vec_mpz_append_ui, unsigned long int, mpz_set_ui);
MCR_vec_mpz_append(vec_mpz_append_si, signed long int, mpz_set_si);

/*
void vec_mpz_append(vec_mpz_t vec, mpz_t num)
{
	//check to verify that there is enough space
	_vec_mpz_increase(vec);
	mpz_init(vec -> data[vec -> len]);
	mpz_set(vec -> data[vec -> len], num);
	vec -> len ++;
}
/* Given a vector and a number, put it at the
 * end of the array
 */

void vec_mpz_get(mpz_t out, vec_mpz_t vec, int i)
{
	//sanity check in debug mode
	MY_ASSERT(i >= 0, "accessing vec_mpz with negative index");
	MY_ASSERT(i < vec -> len, "accessing vec_mpz out of boundary");
	
	mpz_set( out, (vec -> data)[i] );
}
/* Given a vector and an index i in the boundary
 * of the vector return the mpz in place i
 */

void vec_mpz_set(vec_mpz_t vec, int i, mpz_t num)
{
	//sanity check in debug mode
	MY_ASSERT(i < 0, "accessing vec_mpz with negative index!");
	MY_ASSERT(i >= vec -> len, "accessing vec_mpz out of boundary!");
	
	mpz_set( (vec -> data)[i], num);
}
/* Given a vector and an index i in the boundary
 * set the vector value in place i to the mpz in input
 */

void vec_mpz_set_ui(vec_mpz_t vec, int i, unsigned int num)
{
	MY_ASSERT(i >= 0, "accessing vec_mpz with negative index");
	MY_ASSERT(i < vec -> len, "accessing vec_mpz out of boundary");
	
	mpz_set_ui( (vec -> data)[i], num);
}

void vec_mpz_pop(mpz_t out, vec_mpz_t vec, int i)
{
	MY_ASSERT(i >= 0, "accessing vec_mpz with negative index");
	MY_ASSERT(i < vec -> len, "accessing vec_mpz out of boundary");
	
	//in case one don't want to save the output data
	if(out != NULL)
	{
		vec_mpz_get(out, vec, i);
	}
	
	//Todo, Warning, bad implementation, too expensive.
	int j;
	for(j = i; j < (vec -> len) - 1; j++)
	{
			
			//gmp_printf("\tj = %d,\tvec[%d] = %Zd,\tvec[%d] = %Zd\n", j, j, vec -> data[j], j+1, vec -> data[j+1]);
			
		mpz_set(vec -> data[j], vec -> data[j+1]);
	}
	mpz_clear(vec -> data[vec -> len - 1]);
	
	//length decrease by one
	vec -> len --;
	_vec_mpz_decrease(vec);
}
/* 
 * 
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
	char str[1024];
	vec_mpz_string(str, vec);
	printf("vec = %s\nlen = %d,\tsize = %d\n\n", str, vec -> len, vec -> size);
}

int main()
{
	char* str = malloc(sizeof(char)*1024);
	vec_mpz_t vec;
	mpz_t tmp;
	signed long int i, j;
	
	vec_mpz_init(&vec);
	mpz_init(tmp);	
	
	for(i = 0; i < 11; i++)
	{
		mpz_set_ui(tmp, i);
		vec_mpz_append_si(vec, tmp);
	}
	
	vec_mpz_print(vec);
}