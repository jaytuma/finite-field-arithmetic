#include "../ffa.h"

#ifdef DEBUG
	#define MY_ASSERT(b, s) 								\
			if(!(b))										\
			{												\
				fprintf(stderr, "%s %s!\n", ERROR, s);	\
				fflush(stderr);								\
			}										
#else
	#define MY_ASSERT(b,s);
#endif

#define MCR_COMMENT(txt)  