#include <time.h>
#include "../src/lib_itr.h"
#include "test_main.h"

void __itr_mpz_range(itr_mpz_t list, int len)
{
	int i;
	for(i = 0; i < itr_mpz_len(list); i++)
	{
		itr_mpz_pop(list);	
	}
	
	for(i = 0; i < len; i++)
	{
		itr_mpz_append_ui(list,i,i);	
	}
}

int __itr_mpz_gasp(int len)
{
	itr_mpz_t list;
	itr_mpz_init(&list);
	
	mpz_t tmp;
	mpz_init(tmp);
	
	int i = 0;
	
	while(i < len/6)//*_append test
	{
		mpz_set_ui(tmp, i*i);
		itr_mpz_append(list, i, tmp);
		i++;
	}
	
	while(i < len/3)//*_append_ui test
	{
		itr_mpz_append_ui(list, i, i*i);
		i++;
	}
	
	itr_mpz_prev(list);
	
	while(i < len/2)//*_append_si test
	{
		itr_mpz_append_si(list,	i, -i*i);
		itr_mpz_next(list);
		itr_mpz_set_ui(list, i*i);
		i++;
	}
	
	while(i < 2*len/3)//*_insert test
	{
		mpz_set_ui(tmp, i*i);
		itr_mpz_insert(list, i, tmp);
		i++;
	}
	
	while(i < 5*len/6)//*_insert_ui test
	{
		itr_mpz_insert_ui(list, i, i*i);
		i++;
	}
	
	while(i < len)//*_insert_si test
	{
		itr_mpz_insert_si(list, i, -i*i);
		itr_mpz_set_ui(list, i*i);
		i++;
	}
	
	//check that the list is right
	for(i = 0; i < itr_mpz_len(list); i++)
	{
		itr_mpz_next(list);
		if(mpz_cmp_ui( itr_mpz_access(list), i*i) != 0)
		{
			return 0;	
		}
		
		if(i != itr_mpz_access_key(list))
		{
			return 0;	
		}
	}
	
	for(i = 0; i < itr_mpz_len(list); i++)
	{
		itr_mpz_pop(list);
		if(itr_mpz_len(list) != len - i - 1)
		{
			return 0;	
		}
	}
	
	mpz_clear(tmp);
	itr_mpz_clear(&list);
	return 1;
}

int __itr_mpz_random_walk(int op)
{
	itr_mpz_t list;
	itr_mpz_init(&list);
	
	__itr_mpz_range(list, op);
	
	int i, j, p;
	int tmp, tmp2;
	mpz_t tmpz;
	mpz_init(tmpz);
	
	srand(time(0));
	
	for(i = 0; i < op; i++)
	{
		p = rand()%4;
		
		switch(p)
		{
			case 0: //append
				tmp = itr_mpz_len(list);
				tmp2 = itr_mpz_access_key(list);
				itr_mpz_get(tmpz, list); 
				itr_mpz_append_ui(list, op + i, rand());
				
				//check that the lenght is increasing by one
				if(tmp + 1 != itr_mpz_len(list))
				{
					return 0;	
				}				
				
				//check that i'm still looking at the same node
				if(mpz_cmp(tmpz, itr_mpz_access(list)) != 0)
				{
					return 0;	
				}
				
				//same as before but with the key
				if(tmp2 != itr_mpz_access_key(list))
				{
					return 0;	
				}
				
			case 1: //insert
				tmp = itr_mpz_len(list);
				tmp2 = itr_mpz_access_key(list);
				itr_mpz_get(tmpz, list);
				
				itr_mpz_insert_ui(list, op + i, rand());
				
				//check that the length is increasing by one
				if(tmp + 1 != itr_mpz_len(list))
				{
					return 0;
				}
				
				//check that the previous node is the one before
				itr_mpz_prev(list);
				
				if(mpz_cmp(tmpz, itr_mpz_access(list)) != 0)
				{
					return 0;	
				}
				
				if(tmp2 != itr_mpz_access_key(list))
				{
					return 0;	
				}
				
			case 2: //pop
				tmp = itr_mpz_len(list);
				itr_mpz_pop(list);
				
				//check that the lenght is correctly decreasing
				if((tmp > 0) && (tmp - 1 != itr_mpz_len(list)))
				{
					return 0;	
				}
				
			case 3:
				tmp = rand();
				itr_mpz_set_ui(list, tmp);
				
				//check that the inserted value is right
				if(mpz_cmp_ui(itr_mpz_access(list), tmp) != 0)
				{
					return 0;	
				}
		}
		
		p = rand()%(100);
		for(j = 0; j < p; j++)
		{
			itr_mpz_prev(list);	
		}
	}
	
	mpz_clear(tmpz);
	itr_mpz_clear(&list);
	return 1;
}

int __int_mpz_turn_around(int len)
{
	itr_mpz_t list;
	itr_mpz_init(&list);
	
	srand(time(0));
	__itr_mpz_range(list, len);
	
	int i, l;
	//test for the next function
	itr_mpz_restart(list);
	l = rand()%len;
	for(i = 0; i < l; i++)
	{
		itr_mpz_next(list);		
	}
	
	//check
	if(l != itr_mpz_access_key(list))
	{
		return 0;
	}
	
	//test for the prev function
	itr_mpz_restart(list);
	l = rand()%(len - 1) + 1;
	for(i = 0; i < l; i++)
	{
		itr_mpz_prev(list);
	}
	
	//check
	if(len - l != itr_mpz_access_key(list))
	{
		return 0;	
	}
	
	itr_mpz_clear(&list);
	return 1;
}

int __int_mpz_reverse(int len)
{
	itr_mpz_t list;
	itr_mpz_init(&list);
	
	__itr_mpz_range(list, len);
	itr_mpz_reverse(list);
	
	int i;
	for(i = 0; i < len; i++)
	{
		itr_mpz_next(list);
		if(len - i - 1 != itr_mpz_access_key(list))
		{
			return 0;	
		}
	}
	
	return 1;
	itr_mpz_clear(&list);
}

int main()
{
	itr_mpz_t list;
	itr_mpz_init(&list);

	
	MCR_test_iteration_display(__itr_mpz_random_walk(12000), 100,
							   "random walk test:", "", stdout);
	
	MCR_test_iteration_display(__int_mpz_turn_around(100000), 100,
							   "next/prev/reset test:", "", stdout);
	
	MCR_test_oneshot_display(__itr_mpz_gasp(1000000),
							"get/app/set/ins test:", "", stdout);
	
	MCR_test_oneshot_display(__int_mpz_reverse(1000000),
							"reverse test:\t", "", stdout);
	
	
	itr_mpz_clear(&list);
}
