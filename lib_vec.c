#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>

#define ERROR "\n\t\e[1;31mError\e[0m:"

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
	vec -> data = malloc( 2*sizeof(mpz_t) );
	vec -> len = 0;
	vec -> size = 2;
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

void _vec_mpz_decrease(vec_mpz_t vec)
{
	//deallocate memory once not needed anymore
	if(vec -> len < (vec -> size)/2)
	{
		vec -> size /= 2;
		vec -> data = realloc( vec -> data, vec -> size * sizeof(mpz_t));
	}
}
	
void vec_mpz_append(vec_mpz_t vec, mpz_t num)
{
	//check to verify that there is enough space
	_vec_mpz_increase(vec);

	mpz_init(vec -> data[vec -> len]);
	mpz_set(vec -> data[vec -> len], num);
	vec -> len ++;
}

void vec_mpz_get(mpz_t out, vec_mpz_t vec, int i)
{
	//TODO - this code should only be executed in debug mode
	//sanity check
	if(i < 0)
	{
		fprintf(stderr, "%s, accessing vec_mpz with negative index!", ERROR );	
	}
	
	if(i >= vec -> len)
	{
		fprintf(stderr, "%s, accessing vec_mpz out of boundary!", ERROR );	
	}
	
	mpz_set( out, (vec -> data)[i] );
}

// - - - debug use only - - -

void vec_mpz_string(char** out, vec_mpz_t vec)
{
	int i;
	char str[1024];
	char buff[4096];
	mpz_t tmp;
	mpz_init(tmp);	
		
	*out = "[";
	
	strcat(buff, "[");
	
	
	if(vec -> len > 0)
	{
		//first element
		vec_mpz_get(tmp, vec, i);
		gmp_sprintf(str, "%Zd", tmp);
		strcat(buff, str);
		
		
		for(i = 1; i < (vec -> len); i++)
		{
			vec_mpz_get(tmp, vec, i);
			gmp_sprintf(str, ", %Zd", tmp);
			strcat(buff, str);
		}
	}
	
	strcat(buff, "]");
	mpz_clear(tmp);
	*out = buff;
}

int main()
{
	printf("ok\n");
	
	vec_mpz_t vec;
	vec_mpz_init(&vec);
	
	mpz_t tmp;
	mpz_init(tmp);
	
	/*
	mpz_set_ui(tmp, 1);	

	vec_mpz_append(vec, tmp);
	
	mpz_set_ui(tmp, 65);
	vec_mpz_append(vec, tmp);
	
	mpz_set_ui(tmp, 59);
	vec_mpz_append(vec, tmp);
	
	printf("fin qui ok\n");
	fflush(stdout);
	
	*/
	char* str;
	vec_mpz_string(&str, vec);
	printf("vec = %s\n", str);
	
	//vec_mpz_clear(&vec);
}