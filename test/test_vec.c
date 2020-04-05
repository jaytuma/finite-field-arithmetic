#include <time.h>
#include "../src/lib_vec.h"
#include "test_main.h"																	

//testing function

int __vec_mpz_pag(int len)
{
	vec_mpz_t vec;
	vec_mpz_init(&vec);
	mpz_t tmp;
	mpz_init(tmp);
	
	int i, j;
	for(i = 0; i < len; i++)
	{
		j = rand();
		vec_mpz_append_ui(vec, j);
		vec_mpz_get(tmp, vec, i);
		if(mpz_cmp_ui(tmp, j) != 0)
		{
			return 0;
		}
	}
	
	if(vec_mpz_len(vec) != len)
	{
		return 0;	
	}
	
	for(i = 0; i < len; i++)
	{
		j = rand() % (len - i);
		vec_mpz_pop(NULL, vec, j);
	}
	
	if(vec_mpz_len(vec) != 0)
	{
		return 0;	
	}
	
	mpz_clear(tmp);
	vec_mpz_clear(&vec);
	return 1;
}
/* test for pop append_ui and get. Suggested len = 100000
 * suggested iter_num = 100
 */

int __vec_mpz_sig(int len)
{
	vec_mpz_t vec;
	vec_mpz_init(&vec);
	mpz_t tmp;
	mpz_init(tmp);
	
	int i, j;
	for(i = 0; i < len; i++)
	{
		j = rand();
		mpz_set_ui(tmp, j);
		vec_mpz_insert(vec, i, tmp);
		vec_mpz_get(tmp, vec, i);
		if(mpz_cmp_ui(tmp, j) != 0)
		{
			return 0;	
		}
		vec_mpz_set_ui(vec, i, i*i);
	}
	
	for(i = 0; i < len; i++)
	{
		vec_mpz_get(tmp, vec, i);
		if(mpz_cmp_ui(tmp, i*i) != 0)
		{
			return 0;
		}
	}
	
	mpz_clear(tmp);
	vec_mpz_clear(&vec);
	return 1;
}
/* test for set, insert, get
 */

int __vec_mpz_osia(signed int len)
{
	vec_mpz_t vec;
	vec_mpz_init(&vec);
	
	mpz_t tmp;
	mpz_init(tmp);
	
	signed int i;
	for(i = 0; i < len; i++)
	{	
		vec_mpz_insert_si(vec, 2*i, 2*(i-len));		
		vec_mpz_append_si(vec, (signed int) 2*(i-len)+1);
	}
	
	for(i = 0; i < len; i++)
	{
		vec_mpz_insert_ui(vec, 2*(len + i), 2*i);
		vec_mpz_append_ui(vec, 2*i + 1);
	}

	for(i = 0; i < 2*len; i++)
	{
		if(mpz_cmp_si( vec_mpz_access(vec, i) ,  i - 2*len) != 0)
		{
			return 0;	
		}
	}
	
	for(i = 0; i < 2*len; i++)
	{
		vec_mpz_get(tmp, vec, 2*len + i);
		if(mpz_cmp_ui(tmp, i) != 0)
		{
			return 0;	
		}
	}
	
	mpz_clear(tmp);
	vec_mpz_clear(&vec);
	return 1;
}
/* test for insert_ui, insert_si
 * 		append_ui, insert_ui;
 */

int __vec_mpz_cnt(int len)
{	
	vec_mpz_t vec;
	vec_mpz_init(&vec);
	
	int i, cnt = 0, a = rand()%100, b = a, p = rand()%10;
	for(i = 0; i < len; i++)
	{
		if(rand()%10 < p)
		{
			vec_mpz_append_ui(vec, a);
			cnt += 1;
		}
		else
		{
			do
			{
				b = rand()%100;	
			}
			while(b == a);

			vec_mpz_append_ui(vec, b);
		}
	}
	
	mpz_t tmp;
	mpz_init(tmp);
	
	mpz_set_ui(tmp, a);
	b = (cnt == vec_mpz_count(vec, tmp));
		
	vec_mpz_clear(&vec);
	mpz_clear(tmp);
	return b;
}
/* test for the count function
 */ 


/* test the creation and destruction of long vector
 */


int main()
{
	srand(time(0));
	
	MCR_test_iteration_display(__vec_mpz_pag(5000), 100,\
							   "pop/append/get test:", "", stdout);

	MCR_test_iteration_display(__vec_mpz_sig(100000), 100,\
							   "set/insert/get test:", "", stdout);
	
	MCR_test_iteration_display(__vec_mpz_cnt(100000), 100,\
							   "count test:\t", "", stdout);
	
	MCR_test_oneshot_display(__vec_mpz_osia(500000), \
							   "generic ins/app test:", "", stdout);
	
	
	
}



















		