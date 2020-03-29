#include <stdlib.h>
#include <stdio.h>

// - - vector data structure - - //
/* everything is implicilty a pointer */

typedef struct __vec_t
{
	int len;
	int size;
	int* data;
} _vec_t, *vec_t;

void vec_init(vec_t* vec_pt)
{
	*vec_pt = malloc(sizeof(**vec_pt));
	vec_t vec = *vec_pt;
	
	//parameter allocation
	vec -> data = malloc( 2*sizeof(int) );
	vec -> len = 0;
	vec -> size = 2;
}

void vec_clear(vec_t* vec_pt)
{
	vec_t vec = *vec_pt;
	
	//parameter deallocation
	free(vec -> data);
	
	//struct deallocation
	free(vec);
}

int main()
{
	vec_t vec;
	vec_init(&vec);
	vec_clear(&vec);
	
	printf("all ok man! \n");
}