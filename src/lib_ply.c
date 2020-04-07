#include "lib_ply.h"

#define PT_ITERATOR_MPZ	'i'
#define PT_VECTOR_MPZ	'v'
#define PT_EMPTY		'\0'

typedef union _ply_data
{
	itr_mpz_t iz; //shorthand for Iterator of mpZs
	vec_mpz_t vz; //shorthand for Vector of mpZs
} ply_data;

typedef struct _ply_t
{
	char type;
	ply_data data;
}* ply_t;

void ply_init(ply_t* pol_pt)
{
	*pol_pt = malloc(sizeof(**pol_pt));
	ply_t pol = *pol_pt;

	//parameter allocation
	pol -> type = PT_EMPTY;
}

void ply_clear(ply_t* pol_pt)
{
	ply_t pol = *pol_pt;

	switch(pol -> type)
	{
		case PT_EMPTY:
			break;

		case PT_VECTOR_MPZ:
			vec_mpz_clear(&(pol -> data).vz);
			break;

		case PT_ITERATOR_MPZ:
			itr_mpz_clear(&(pol -> data).iz);
			break;

		default:
			MY_ASSERT(0, "in ply_clear, unknown polynomial type");
			break;
	}

	free(pol);
}

// basic functions that don't require a Field argument

signed int ply_degree(ply_t pol)
{
	switch(pol -> type)
	{
		case PT_EMPTY:
		{
			MY_ASSERT(0, "getting degree of a non initialized polynomial")
			return -1;
			break;
		}

		case PT_VECTOR_MPZ:
		{
			vec_mpz_t vec;
			vec = (pol -> data).vz;
			int out = vec_mpz_len(vec) - 1;

			while((out >= 0) && (mpz_sgn(vec_mpz_access(vec, out)) == 0 ))
			{
				out --;
			}

			return out;
			break;
		}

		case PT_ITERATOR_MPZ:
		{
			itr_mpz_t list = (pol -> data).iz;
	
			if(itr_mpz_len(list) == 0)
			{
				return -1;
			}
				int out;

				MY_ASSERT(_itr_is_reset(list), "getting degree of non resetted iterator");
				out = itr_mpz_access_key(list);
				itr_mpz_restart(list);
			return out;
			break;
		}

		default:
		{
			MY_ASSERT(0, "in ply_degree, unknown polynomial type");
			return -1;
			break;
		}
	}
}
/* return the degree of the polynomial if it is non zero
 * and -1 for the zero polynomial
 */