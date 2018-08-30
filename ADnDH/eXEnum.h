
#ifdef XENUM_BEGIN
#undef XENUM_BEGIN
#endif

#ifdef XENUM_LINE
#undef XENUM_LINE
#endif

#ifdef XENUM_END
#undef XENUM_END
#endif

#ifdef XENUM_IMPLEMENT_MODE
	#define XENUM_BEGIN(Name) char* s##Name[] = {
	#define XENUM_LINE(Call,String,Value) String,
	#define XENUM_END(Name) };
#else
	#define XENUM_BEGIN(Name) typedef enum {
	#define XENUM_LINE(Call,String,Value) Call = Value,
	#define XENUM_END(Name) } e##Name ; extern char* s##Name[];
#endif

#ifdef XENUM_IMPLEMENT_MODE
	#undef XENUM_IMPLEMENT_MODE
#endif


