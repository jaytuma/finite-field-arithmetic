#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include "macro.h"

/* - - - iterable object - - - */
/* A circular list doubly linked with starting position and
 * and a moving reading position. circularity make 4 pointer
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

void itr_node_mpz_clear(itr_node_mpz_t* node_pt)
{
	itr_node_mpz_t node = *node_pt;
	mpz_clear(node -> data);
	
	free(node);
}

void itr_mpz_init(itr_mpz_t* list_pt)
{
	*list_pt = malloc(sizeof(**list_pt));
	itr_mpz_t list = *list_pt;
	
	list -> len = 0;
	list -> first = NULL;
	list -> seek = NULL;
}
	
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

void itr_mpz_restart(itr_mpz_t list)
{
	list -> seek = list -> first;	
}

void itr_mpz_get(mpz_t out, itr_mpz_t list)
{
	mpz_set(out, (list -> seek) -> data);
}

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

	//IO function
void itr_node_mpz_string(char* out, itr_node_mpz_t node)
{
	gmp_sprintf(out, "(%d, %Zd)", node -> key, node -> data);
}

void itr_mpz_string(char* out, itr_mpz_t list)
{
	//this function should not move the seek
	char str[1024];
	out[0] = '\0';
	
	itr_node_mpz_t node;
	node = list -> first;
	
	int i;
	if(list -> len > 10)
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

/*
int main()
{
	char str[65536];
	itr_mpz_t list;
	itr_mpz_init(&list);
	
	mpz_t tmp;
	mpz_init(tmp);
	
	int i;
	for(i = 0; i < 25; i++)
	{
		mpz_set_ui(tmp, i);
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
*/










