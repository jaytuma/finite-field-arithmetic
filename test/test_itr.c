#include "../src/lib_itr.h"

void __itr_mpz_range(itr_mpz_t list, int len)
{
	int i;
	for(i = 0; i < itr_mpz_len(list); i++)
	{
		itr_mpz_pop(list);	
	}
	
	for(i = 0; i < len; i++)
	{
		itr_mpz_append_ui(list,0,i);	
	}
	
	itr_mpz_print(list);
}

void __itr_mpz_gasp(int len)
{
	itr_mpz_t list;
	itr_mpz_init(&list);
	
	mpz_t tmp;
	mpz_init(tmp)
	
	int i = 0;
	while(i < len/6)
	{
		mpz_set_ui(tmp, i*i);
		itr_mpz_append(list, i, tmp);
		i++;
		
			itr_mpz_print(list);
	}
	
	while(i < len/3)
	{
		itr_mpz_append_ui(list, i, i*i);
		i++;
		
			itr_mpz_print(list);
	}
	
	itr_mpz_prev(list);
	
	while(i < len/2);
	{
		itr_mpz_append_si(list,	i, -i*i);
		itr_mpz_next();
		itr_mpz_set_ui(list, i*i);
		i++;
		
			itr_mpz_print(list);
	}
	
	itr_mpz_print(list);
}

int main()
{
	itr_mpz_t list;
	itr_mpz_init(&list);
	
	__itr_mpz_gasp(list, 12);
	
	itr_mpz_clear(&list);
}