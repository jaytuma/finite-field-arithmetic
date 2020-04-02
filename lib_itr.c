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

void itr_node_mpz_init(itr_node_mpz_t* node_pt)
{
	*node_pt = malloc(sizeof(**node_pt));
	itr_node_mpz_t node = *node_pt;
	
	node -> key = 0;
	node -> prev = NULL;
	node -> next = NULL;
	
	mpz_init(node -> data);
}
/* Inizialize a node for the iterator
 * object
 */

void itr_node_mpz_clear(itr_node_mpz_t* node_pt)
{
	itr_node_mpz_t node = *node_pt;
	mpz_clear(node -> data);
	
	free(node);
}
/* Clear the memory space occupied by a node
 */ 

void itr_mpz_init(itr_mpz_t* list_pt)
{
	*list_pt = malloc(sizeof(**list_pt));
	itr_mpz_t list = *list_pt;
	
	list -> len = 0;
	list -> first = NULL;
	list -> seek = NULL;
}
/* Inizialize a iterator list
 */
	
void itr_mpz_clear(itr_mpz_t* list_pt)
{
	itr_node_mpz_t node = (*list_pt) -> first;
	itr_node_mpz_t node_tmp = NULL;

	int i;
	for(i = 0; i < (*list_pt) -> len; i++)
	{
		node_tmp = (node -> next);
		itr_node_mpz_clear(&node);

		node = node_tmp;
	}
	
	free(*list_pt);
}
/* Clearthe memory space occupied by a iterator
 * list and all its associated nodes
 */

	//specific function

void itr_mpz_next(itr_mpz_t list)
{
	MY_ASSERT((list -> len > 0 && list -> seek != NULL) || 
			  (list -> len == 0 && list -> seek == NULL),
			  "in iter_mpz object, seek and len disagree");
	
	if(list -> len != 0)
	{
		(list -> seek) = (list -> seek) -> next;	
	}
}
/* move the seek pointer one node
 * forward
 */

void itr_mpz_prev(itr_mpz_t list)
{
	MY_ASSERT((list -> len > 0 && list -> seek != NULL) || 
			  (list -> len == 0 && list -> seek == NULL),
			  "in iter_mpz object, seek and len disagree");
	
	if(list -> len != 0)
	{
		(list -> seek) = (list -> seek) -> prev;	
	}	
}
/* move the seek pointer one node
 * behind
 */

void itr_mpz_restart(itr_mpz_t list)
{
	list -> seek = list -> first;	
}
/* move the seek pointer to the start
 * position
 */

void itr_mpz_get(mpz_t out, itr_mpz_t list)
{
	MY_ASSERT(list -> len > 0, "trying to get data from an empty list");
	MY_ASSERT(list -> first != NULL, "trying to get data from list with NULL initial data pointer");
	MY_ASSERT(list -> seek != NULL, "trying to get data from list with NULL seek data pointer");
	
	mpz_set(out, (list -> seek) -> data);
}
/* set out to be equal to the value stored
 * in the node pointed by the seek pointer.
 *
 *	WARNING: requires non-empty iterator
 */

void itr_mpz_get_key(int* key_pt, itr_mpz_t list)
{
	MY_ASSERT(list -> len > 0, "trying to get data from an empty list");
	MY_ASSERT(list -> first != NULL, "trying to get data from list with NULL initial data pointer");
	MY_ASSERT(list -> seek != NULL, "trying to get data from list with NULL seek data pointer");
	
	*key_pt = (list -> seek) -> key;
}
/* set the integer pointed by key_pt equal
 * to the key of the node currently pointed
 * by seek.
 *
 *	WARNING: requires non-empty iterator
 */

void itr_mpz_set(itr_mpz_t list, mpz_t num)
{
	MY_ASSERT(list -> len > 0, "trying to set data of an empty list");
	MY_ASSERT(list -> first != NULL, "trying to set data of a list with NULL initial data pointer");
	MY_ASSERT(list -> seek != NULL, "trying to set data of a list with NULL seek data pointer");
	
	mpz_set((list -> seek) -> data, num);
}
/* set the value of the node currently pointed by
 * seek equal to num.
 *
 * 	WARNING: requires non-empty iterator
 */

void itr_mpz_set_key(itr_mpz_t list, int new_key)
{
	MY_ASSERT(list -> len > 0, "trying to set data of an empty list");
	MY_ASSERT(list -> first != NULL, "trying to set data of a list with NULL initial data pointer");
	MY_ASSERT(list -> seek != NULL, "trying to set data of a list with NULL seek data pointer");
	
	(list -> seek) -> key = new_key;
}
/* set the value of the node currently pointed by
 * seek equal to num.
 *
 * 	WARNING: requires non-empty iterator
 */

void itr_mpz_append(itr_mpz_t list, int key, mpz_t num)
{
	itr_node_mpz_t node;
	itr_node_mpz_init(&node);
	
	node -> key = key;
	mpz_set(node -> data, num); //to parametrize through a macro!
	
	if(list -> len == 0)
	{
		MY_ASSERT(list -> first == NULL, "empty list with non NULL initial data pointer");
		MY_ASSERT(list -> seek == NULL, "empty list with non NULL seek data pointer");
		
		node -> prev = node;
		node -> next = node;
		
		list -> first = node;
		list -> seek = node;
		
		list -> len ++;
	}
	else
	{
		MY_ASSERT(list -> first != NULL, "non empty list with NULL initial data pointer");
		MY_ASSERT(list -> seek != NULL, "non empty list with NULL seek data pointer");
		
		/*the order is extremely importart*/
		((list -> first) -> prev) -> next = node;
		node -> prev = (list -> first) -> prev;
		node -> next = list -> first;
		(list -> first) -> prev = node;
		
		list -> len ++;
	}
}
/* Create a node at the end of the iterator's list
 * i.e. before the first element with given key and value
 */

void itr_mpz_insert(itr_mpz_t list, int key, mpz_t num)
{
	itr_node_mpz_t node;
	itr_node_mpz_init(&node);
	
	node -> key = key;
	mpz_set(node -> data, num); //to parametrize through a macro!
	
	if(list -> len == 0)
	{
		MY_ASSERT(list -> first == NULL, "empty list with non NULL initial data pointer");
		MY_ASSERT(list -> seek == NULL, "empty list with non NULL seek data pointer");
		
		node -> next = node;
		node -> prev = node;
		
		list -> first = node;
		list -> seek = node;
		
		list -> len ++;
	}
	else
	{
		MY_ASSERT(list -> first != NULL, "non empty list with NULL initial data pointer");
		MY_ASSERT(list -> seek != NULL, "non empty list with NULL seek data pointer");
		
		/*the order is extremely important*/
		((list -> seek) -> next) -> prev = node;
		node -> next = (list -> seek) -> next;
		node -> prev = list -> seek;
		(list -> seek) -> next = node;
		list -> seek = node;
		
		list -> len ++;
	}
}
/* Create a node between the one currently pointed by
 * seek and the next one. Set seek to point to the
 * newly generated node
 */

void itr_mpz_pop(itr_mpz_t list)
{
	if(list -> len > 0)
	{
		MY_ASSERT(list -> seek != NULL, "non empty list with NULL seek data pointer");
		MY_ASSERT(list -> first != NULL, "non empty list with NULL first data pointer");
		
		itr_node_mpz_t node;
		node = list -> seek;
		
		(node -> prev) -> next = node -> next;
		(node -> next) -> prev = node -> prev;
		list -> seek = node -> next;
		
		if(list -> first == node)
		{
			list -> first = node -> next;	
		}
		
		list -> len --;
		itr_node_mpz_clear(&node);
	}
}
/* Remove the node currently pointed by seek and move
 * the seek pointer to the next node
 */

void itr_mpz_reverse(itr_mpz_t list)
{
	if(list -> len > 0)
	{
		MY_ASSERT(list -> seek != NULL, "non empty list with NULL seek data pointer");
		MY_ASSERT(list -> first != NULL, "non empty list with NULL first data pointer");
		
		itr_node_mpz_t node, tmp;
		int i;
		for(i = 0; i < list -> len; i++)
		{
			tmp = node -> next;
			node -> next = node -> prev;
			node -> prev = tmp;
		}
	}
}
/* reverse the order of the list
 *
 * TODO: if this operation is interesting it can be done in constant time
 * (currently is O(n)) by adding a flag to the structure.
 *
 */

	//IO function
void itr_node_mpz_string(char* out, itr_node_mpz_t node)
{
	gmp_sprintf(out, "(%d, %Zd)", node -> key, node -> data);
}
/* writes in out a string representing the given node
 *
 *	WARNING: requires that enough memory for out is
 *	allocated before
 */

void itr_mpz_string(char* out, itr_mpz_t list)
{
	//this function should not move the seek
	char str[1024];
	out[0] = '\0';
	
	itr_node_mpz_t node;
	node = list -> first;
	
	int i;
	if(list -> len > 0)
	{
		itr_node_mpz_string(out, node);
		node = node -> next;
		
		for(i = 0; i < (list -> len) - 1; i++)
		{
			itr_node_mpz_string(str, node);
			
			strcat(out, "->");
			strcat(out, str);
			
			node = node -> next;
		}
	}
	else
	{
		strcpy(out, "()");	
	}
}
/* writes in out a string representing the given
 * iterator list
 *
 *	WARNING: requires that enough memory for out is
 *	allocated before
 */

int main()
{
	char str[65536];
	itr_mpz_t list;
	itr_mpz_init(&list);
	
	mpz_t tmp;itr_mpz_string(str, list);
	printf("list = %s\n", str);
	mpz_init(tmp);
	
	int i;
	for(i = 0; i < 6; i++)
	{
		mpz_set_ui(tmp, i*i);
		itr_mpz_insert(list, 1, tmp);
	}
	
	//itr_mpz_restart(list);
	
	for(i = 0; i < 20; i++)
	{
		itr_mpz_next(list);
	}
	
	for(i = 0; i < 10; i++)
	{
		itr_mpz_prev(list);	
	}
	
	itr_mpz_string(str, list);
	printf("list = %s\n", str);
	
	itr_mpz_get(tmp, list);
	gmp_printf("il primo elemento vale %Zd\n\n", tmp);
	
	printf("len = %d\n", list -> len);
	itr_mpz_clear(&list);
	printf("Everything is fine\n");
}











