#ifndef stdarg_h_
#define stdarg_h_

#define va_start(v,l)	__builtin_va_start((v),l)
#define va_end			__builtin_va_end
#define va_arg			__builtin_va_arg
#define va_copy(d,s)	__builtin_va_copy((d),(s))
#define	va_list			__builtin_va_list

#endif // stdarg_h_
