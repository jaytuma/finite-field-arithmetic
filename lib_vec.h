#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "macro.h"

#define INITIAL_MEMORY 2

typedef struct _vec_mpz_t
{
	int len;
	int size;
	mpz_t* data;
} *vec_mpz_t;

	//memory managment	

void vec_mpz_init(vec_mpz_t* vec_pt);

void vec_mpz_clear(vec_mpz_t* vec_pt);

void _vec_mpz_increase(vec_mpz_t vec);

void _vec_mpz_decrease(vec_mpz_t vec);

	//main functions

int vec_mpz_len(vec_mpz_t vec);

	/*from macro expansion*/
void vec_mpz_append(vec_mpz_t vec, mpz_t);
void vec_mpz_append_ui(vec_mpz_t vec, unsigned long int num);
void vec_mpz_append_si(vec_mpz_t vec, signed long int num);
	/* - - - - - - - - - -*/

void vec_mpz_get(mpz_t out, vec_mpz_t vec, int i);

	/*from macro expansion*/
void vec_mpz_set(vec_mpz_t vec, int i, mpz_t num);
void vec_mpz_set_ui(vec_mpz_t, int i, unsigned long int num);
void vec_mpz_set_si(vec_mpz_t vec, int i, signed long int num);
	/* - - - - - - - - - -*/

void vec_mpz_pop(mpz_t out, vec_mpz_t vec, int i);
	
	/*from macro expansion*/
void vec_mpz_insert(vec_mpz_t vec, int i, mpz_t num);
void vec_mpz_insert_ui(vec_mpz_t vec, int i, unsigned long int num);
void vec_mpz_insert_si(vec_mpz_t vec, int i, signed long int num);
	/* - - - - - - - - - -*/

int vec_mpz_count(vec_mpz_t vec, mpz_t num);

int vec_mpz_index(vec_mpz_t vec, mpz_t num);
	
void vec_mpz_reverse(vec_mpz_t vec);

	//IO function

void vec_mpz_string(char* out, vec_mpz_t vec);

void vec_mpz_print(vec_mpz_t vec);
	
	//Debug Use Only

int _vec_mpz_typecheck(vec_mpz_t vec);
int _int_typecheck(int i);

	// - - - - M a c r o z - - - - //

	/*vec_mpz_len(vec_mpz_t) - with type check*/
#ifdef DEBUG
#define vec_mpz_len(MCR_vec)												\
	((_vec_mpz_typecheck(MCR_vec)) ? (MCR_vec -> len) : (0) )							
#else
#define vec_mpz_len(MCR_vec)												\
	(MCR_vec -> len)
#endif
/* Return the lenght of the current vector (not the size!)
 *  	now it's a macro for optimization, and in debug mode
 *		will still return a warning if wrong input type.
 *		
 *		the general idea for this complier-time type checking
 *		is to pass the arguments to a function that returns 1
 *		therefore compiler will check the input type and if
 *		correct, the ternary operator returns the first option
 */

	/*vec_mpz_access(vec_mpz_t, int) - with type check*/
#ifdef DEBUG
#define vec_mpz_access(MCR_vec, MCR_i)															\
	((_vec_mpz_typecheck(MCR_vec) * _int_typecheck(MCR_i)) ? (MCR_vec -> data[MCR_i]) : (0))	
#else
#define vec_mpz_access(MCR_vec, MCR_i)															\
	(MCR_vec -> data[MCR_i])
#endif
/* Return an mpz_t. Never use this function in asignment (get
 * 		and set should be used instead). The purpose of this macro
 *		is to access elements when they are needed as argument of
 *		a function without the need to copy them first.
 */





