/*macro to handle testing*/

#define MCR_test_iteration_display(MCR_test_and_arguments, MCR_iter_num, MCR_testname_str, MCR_error_str, MCR_fileout)\
do																			\
{																			\
	int MCR_tid_i;															\
	int MCR_tid_n = MCR_iter_num;											\
	int MCR_flag = 1;														\
																			\
	fprintf((MCR_fileout), "%s\t   0.0%%", (MCR_testname_str));				\
	fflush(MCR_fileout);													\
																			\
	for(MCR_tid_i = 0; MCR_tid_i < MCR_tid_n; MCR_tid_i++)					\
	{																		\
		if(!MCR_test_and_arguments)											\
		{																	\
			fprintf(MCR_fileout, "\b\b\b\b\b\b\e[1;31mFailed\e[0m\n");		\
			fflush(MCR_fileout);											\
			if((MCR_error_str)[0] != '\0')									\
			{																\
				fprintf(stderr, "%s %s!\n", ERROR, MCR_error_str);			\
				fflush(stderr);												\
			}																\
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

#define MCR_test_oneshot_display(MCR_test_and_arguments, MCR_testname_str, MCR_error_str, MCR_fileout)\
do																			\
{																			\
	fprintf(MCR_fileout, "%s\t   ....", (MCR_testname_str));				\
	fflush(MCR_fileout);													\
																			\
	if(MCR_test_and_arguments)												\
	{																		\
		fprintf(MCR_fileout, "\b\b\b\b\b\b\e[1;32mSucces\e[0m\n");			\
		fflush(MCR_fileout);												\
	}																		\
	else																	\
	{																		\
		fprintf(MCR_fileout, "\b\b\b\b\b\b\e[1;31mFailed\e[0m\n");			\
		fflush(MCR_fileout);												\
		if((MCR_error_str)[0] != '\0')										\
		{																	\
			fprintf(stderr, "%s %s!\n", ERROR, MCR_error_str);				\
			fflush(stderr);													\
		}																	\
	}																		\
}																			\
while(0)
