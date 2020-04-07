#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include "macro.h"

/* - - - iterable object - - - */
/* A circular list doubly linked with starting point and
 * a moving reading position. circularity make 4 pointer
 * more usefull and lets us get rid of the "ending" pointer.
 * Append therefore becomes a prepend to the first element.
 */

typedef struct _itr_node_mpz_t
{
	int key; //this is an extra-field useful for polynomial
	
	struct _itr_node_mpz_t *prev;
	struct _itr_node_mpz_t *next;
	
	mpz_t data;
} *itr_node_mpz_t;

typedef struct _itr_mpz_t
{
	int len;
	
	itr_node_mpz_t first;
	itr_node_mpz_t seek;
} *itr_mpz_t;

void itr_node_mpz_init(itr_node_mpz_t* node_pt);

void itr_node_mpz_clear(itr_node_mpz_t* node_pt);

void itr_mpz_init(itr_mpz_t* list_pt);
	
void itr_mpz_clear(itr_mpz_t* list_pt);
	
	//specific function

void itr_mpz_next(itr_mpz_t list);
	
void itr_mpz_prev(itr_mpz_t list);

void itr_mpz_restart(itr_mpz_t list);

void itr_mpz_get(mpz_t out, itr_mpz_t list);

void itr_mpz_get_key(int* key_pt, itr_mpz_t list);

	/*from macro expansion*/
void itr_mpz_set(itr_mpz_t list, mpz_t num);
void itr_mpz_set_ui(itr_mpz_t list, unsigned long int num);
void itr_mpz_set_si(itr_mpz_t list, signed long int num);
	/* - - - - - - - - - -*/

void itr_mpz_set_key(itr_mpz_t list, int new_key);

	/*from Macro Expansion*/
void itr_mpz_append(itr_mpz_t list,	int key, mpz_t num);
void itr_mpz_append_ui(itr_mpz_t list, int key, unsigned long int num);
void itr_mpz_append_si(itr_mpz_t list, int key, signed long int num);
	/*- - - - - - - - - - */									
	
	/*from Macro Expansion*/
void itr_mpz_insert(itr_mpz_t list,	int key, mpz_t num);
void itr_mpz_insert_ui(itr_mpz_t list, int key, unsigned long int num);
void itr_mpz_insert_si(itr_mpz_t list, int key, signed long int num);
	/* - - - - - - - - - -*/

void itr_mpz_pop(itr_mpz_t list);

void itr_mpz_reverse(itr_mpz_t list);

	/*IO function*/

int itr_node_mpz_string_len(itr_node_mpz_t node);

void itr_node_mpz_string(char* out, itr_node_mpz_t node);

int itr_mpz_string_len(itr_mpz_t list);

void itr_mpz_string(char* out, itr_mpz_t list);

void itr_mpz_print(itr_mpz_t list);

	//debug function

int _itr_mpz_typecheck(itr_mpz_t list);

int _itr_is_reset(itr_mpz_t list);

	// - - - - M a c r o z - - - - //

#ifdef DEBUG
#define itr_mpz_len(MCR_list)											\
	((_itr_mpz_typecheck(MCR_list)) ? (MCR_list -> len) : (0))
#else
#define itr_mpz_len(MCR_list)											\
	(MCR_list -> len)
#endif
/* return the lenght of a iterator list. with compiler-time
 * type-checking
 */

#ifdef DEBUG
#define itr_mpz_access(MCR_list)										\
	((_itr_mpz_typecheck(MCR_list)) ? ((MCR_list -> seek) -> data) : (0))
#else
#define itr_mpz_access(MCR_list)										\
	((MCR_list -> seek) -> data)
#endif
/* give access to the data pointed by seek without the need
 * to get that value in a different mpz (that is useful when
 * passing the currently pointed value as a function argument)
 */

#ifdef DEBUG
#define itr_mpz_access_key(MCR_list)									\
	((_itr_mpz_typecheck(MCR_list)) ? ((MCR_list -> seek) -> key) : (0))	
#else
#define itr_mpz_access_key(MCR_list)										\
	((MCR_list -> seek) -> key)											
#endif
/* give access to the key of the node pointed by the seek pointer.
 * here because *_get_key is implicit (as nearly everything should be)
 * but is alway good to give direct access.
 */
