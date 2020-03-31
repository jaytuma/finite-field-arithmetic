#define ERROR "\n\t\e[1;31mError\e[0m:"

#define DEBUG 1

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