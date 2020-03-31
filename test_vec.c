#include <time.h>
#include "lib_vec.h"


/*macro to handle testing*/

#define MCR_test_iteration_display(MCR_test_and_arguments, MCR_iter_num, MCR_testname_str, MCR_fail_str, MCR_fileout)\
do																			\
{																			\
	int MCR_tid_i;															\
	int MCR_tid_n = MCR_iter_num;											\
	int MCR_flag = 1;														\
																			\
	fprintf(MCR_fileout, "%s:\t   0.0%%", MCR_testname_str);				\
	fflush(MCR_fileout);													\
																			\
	for(MCR_tid_i = 0; MCR_tid_i < MCR_tid_n; MCR_tid_i++)					\
	{																		\
		if(!MCR_test_and_arguments)											\
		{																	\
			fprintf(stderr, "%s %s!\n", ERROR, MCR_fail_str);				\
			fflush(stderr);													\
			MCR_flag = 0;													\
			break;															\
		}																	\
		else																\
		{																	\
			fprintf(MCR_fileout, "\b\b\b\b\b\b%5.1f%%",						\
					((100 * MCR_tid_i)/( (float) MCR_tid_n )));				\
			fflush(MCR_fileout);											\
		}																	\
	}																		\
																			\
	if(MCR_flag)															\
	{																		\
		fprintf(MCR_fileout, "\b\b\b\b\b\b\e[1;32m100.0%%\e[0m\n");			\
		fflush(MCR_fileout);												\
	}																		\
}																			\
while(0)																	

//testing function

int __vec_mpz_pag(int len)
{
	srand(time(0));
	
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
			return -1;
		}
	}
	
	if(vec_mpz_len(vec) != len)
	{
		return -1;	
	}
	
	for(i = 0; i < len; i++)
	{
		j = rand() % (len - i);
		vec_mpz_pop(NULL, vec, j);
	}
	
	if(vec_mpz_len(vec) != 0)
	{
		return -1;	
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
	srand(time(0));
	
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
			return -1;	
		}
		vec_mpz_set_ui(vec, i, i*i);
	}
	
	//printf("len = %d", vec_mpz_len(vec));
	
	for(i = 0; i < len; i++)
	{
		vec_mpz_get(tmp, vec, i);
		if(mpz_cmp_ui(tmp, i*i) != 0)
		{
			return -1;
		}
	}
	
	mpz_clear(tmp);
	vec_mpz_clear(&vec);
	return 1;
}
/* test for 
	


/* test the creation and destruction of long vector
 */


int main()
{
	//rintf("test = %d", __vec_mpz_pop_append(1000));
	MCR_test_iteration_display(__vec_mpz_pag(10000), 100,\
							   "pop/append/get test", "", stdout);

	MCR_test_iteration_display(__vec_mpz_sig(100000), 100,\
							   "set/insert/get test", "", stdout);
}



















		