#ifndef SETUP_H_
#define SETUP_H_

#define PERVERTION
#define DEBUG_MODE
#define FUNCTION_IGNORE_GLOBAL_ERRORS
//#define DEBUG_MALLOC


#ifdef DEBUG_MODE
	#define debug(...) printf(__VA_ARGS__)
#else
	#define debug(...)
#endif

#ifdef PERVERTION
#define MAXBONUSTABLEVALUE 20
#else
#define MAXBONUSTABLEVALUE 30
#endif



#endif /* SETUP_H_ */
