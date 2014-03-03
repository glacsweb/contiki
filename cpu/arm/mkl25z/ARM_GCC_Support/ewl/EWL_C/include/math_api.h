/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/04/27 13:22:29 $
 * $Revision: 1.5 $
 */

#ifndef _EWL_MATH_API_H
#define	_EWL_MATH_API_H

#include <ewl_misra_types.h>

#if _GCCPORT_
	#include <ewl_math_api_gcc.h>
#else

#if _EWL_DOUBLE_SIZE == 32

typedef union
{
	double   d;
	int32_t  sw;
	uint32_t uw;
} *_ewl_hilo_ptr;

_MISRA_EXCEPTION_MATHAPI_MACROS()
#define __HI(x)  ((_ewl_hilo_ptr)&x)->sw
#define __LO(x)  ((_ewl_hilo_ptr)&x)->sw
#define __UHI(x) ((_ewl_hilo_ptr)&x)->uw
#define __ULO(x) ((_ewl_hilo_ptr)&x)->uw

#elif _EWL_DOUBLE_SIZE == 64

typedef union
{
	double d;
	struct { int32_t  lsw, hsw; } s;
	struct { uint32_t luw, huw; } u;
} *_ewl_hilo_ptr;

#if _EWL_LITTLE_ENDIAN
	_MISRA_EXCEPTION_MATHAPI_MACROS()
	#define __HI(x)  ((_ewl_hilo_ptr)&x)->s.hsw
	#define __LO(x)  ((_ewl_hilo_ptr)&x)->s.lsw
	#define __UHI(x) ((_ewl_hilo_ptr)&x)->u.huw
	#define __ULO(x) ((_ewl_hilo_ptr)&x)->u.luw
#else
	_MISRA_EXCEPTION_MATHAPI_MACROS()
	#define __LO(x)  ((_ewl_hilo_ptr)&x)->s.hsw
	#define __HI(x)  ((_ewl_hilo_ptr)&x)->s.lsw
	#define __ULO(x) ((_ewl_hilo_ptr)&x)->u.huw
	#define __UHI(x) ((_ewl_hilo_ptr)&x)->u.luw
#endif /* _EWL_LITTLE_ENDIAN */

#else
#error
#endif

_EWL_BEGIN_EXTERN_C

#if _EWL_FLOATING_POINT
	#if _EWL_C99
		#if __has_intrinsic(__builtin_fma)
			extern double __builtin_fma(double, double, double) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
		#endif

		#if __has_intrinsic(__builtin_fmaf)
			extern float __builtin_fmaf(float, float, float) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
		#endif

		#if __has_intrinsic(__builtin_round)
			extern short __builtin_round(long) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
		#endif


		double _EWL_MATH_CDECL __ieee754_lgamma_r(double, int32_t *);


	#endif /* _EWL_C99 */
#endif /* _EWL_FLOATING_POINT */

	#if __has_intrinsic(__builtin___count_bits32)
		extern unsigned int __builtin___count_bits32(unsigned long) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#else
		uint32_t _EWL_MATH_CDECL __ewl_generic_count_bits32(uint32_t) _EWL_CANT_THROW;
	#endif

	#if __has_intrinsic(__builtin___count_leading_zero32)
		extern unsigned int __builtin___count_leading_zero32(unsigned int) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#else
		uint32_t _EWL_MATH_CDECL __ewl_generic_count_leading_zero32(uint32_t) _EWL_CANT_THROW;
	#endif

	#if __has_intrinsic(__builtin___count_trailing_zero32)
		extern unsigned int __builtin___count_trailing_zero32(unsigned int) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___count_leading_one32)
		extern unsigned int __builtin___count_leading_one32(unsigned long) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___count_trailing_one32)
		extern unsigned int __builtin___count_trailing_one32(unsigned long) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___rotate_left32)
		extern unsigned int __builtin___rotate_left32(unsigned int, int) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___rotate_right32)
		extern unsigned int __builtin___rotate_right32(unsigned int, int) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#endif

#if _EWL_LONGLONG

	#if __has_intrinsic(__builtin___count_bits64)
		extern unsigned int __builtin___count_bits64(uint64_t) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#else
		uint32_t _EWL_MATH_CDECL __ewl_generic_count_bits64(uint64_t) _EWL_CANT_THROW;
	#endif

	#if __has_intrinsic(__builtin___count_leading_zero64)
		extern unsigned int __builtin___count_leading_zero64(uint64_t) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#else
		uint_t _EWL_MATH_CDECL __ewl_generic_count_leading_zero64(uint64_t) _EWL_CANT_THROW;
	#endif

	#if __has_intrinsic(__builtin___count_trailing_zero64)
		extern unsigned int __builtin___count_trailing_zero64(uint64_t) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#else
		uint_t _EWL_MATH_CDECL __ewl_generic_count_trailing_zero64(uint64_t) _EWL_CANT_THROW;
	#endif

	#if __has_intrinsic(__builtin___count_leading_one64)
		extern unsigned int __builtin___count_leading_one64(uint64_t) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___count_trailing_one64)
		extern unsigned int __builtin___count_trailing_one64(uint64_t) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___rotate_left64)
		extern uint64_t __builtin___rotate_left64(uint64_t, int) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___rotate_right64)
		extern uint64_t __builtin___rotate_right64(uint64_t, int) _EWL_CANT_THROW _EWL_ATTRIBUTE_CONST;
	#endif

#endif /* _EWL_LONGLONG */

_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_count_bits32(uint32_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_count_leading_zero32(uint32_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_count_trailing_zero32(uint32_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_count_leading_one32(uint32_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_count_trailing_one32(uint32_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_rotate_left32(uint32_t, int) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_rotate_right32(uint32_t, int) _EWL_CANT_THROW;

#if _EWL_LONGLONG
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_count_bits64(uint64_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_count_leading_zero64(uint64_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_count_trailing_zero64(uint64_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_count_leading_one64(uint64_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL __ewl_count_trailing_one64(uint64_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint64_t _EWL_MATH_CDECL __ewl_rotate_left64(uint64_t, int) _EWL_CANT_THROW;
_EWL_IMP_EXP_C uint64_t _EWL_MATH_CDECL __ewl_rotate_right64(uint64_t, int) _EWL_CANT_THROW;
#endif /* _EWL_LONGLONG */

#if _EWL_USE_INLINE

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_count_bits32(uint32_t x) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___count_bits32)
		return __builtin___count_bits32(x);
	#else
		return __ewl_generic_count_bits32(x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_count_leading_zero32(uint32_t x) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___count_leading_zero32)
		return __builtin___count_leading_zero32(x);
	#else
		return __ewl_generic_count_leading_zero32(x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_count_trailing_zero32(uint32_t x) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___count_trailing_zero32)
		return __builtin___count_trailing_zero32(x);
	#else
		return __ewl_count_bits32((x & -x) - 1);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_count_leading_one32(uint32_t x) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___count_leading_one32)
		return __builtin___count_leading_one32(x);
	#else
		return __ewl_count_leading_zero32(~x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_count_trailing_one32(uint32_t x) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___count_trailing_one32)
		return __builtin___count_trailing_one32(x);
	#else
		return __ewl_count_trailing_zero32(~x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_rotate_left32(uint32_t x, int n) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___rotate_left32)
		return __builtin___rotate_left32(x, n);
	#else
		n &= 0x1F;
		return (x << n) | (x >> (32-n));
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_rotate_right32(uint32_t x, int n) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___rotate_right32)
		return __builtin___rotate_right32(x, n);
	#else
		n &= 0x1F;
		return (x << (32-n)) | (x >> n);
	#endif
	}

#if _EWL_LONGLONG

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_count_bits64(uint64_t x) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___count_bits64)
		return __builtin___count_bits64(x);
	#else
		return __ewl_generic_count_bits64(x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_count_leading_zero64(uint64_t x) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___count_leading_zero64)
		return __builtin___count_leading_zero64(x);
	#else
		return __ewl_generic_count_leading_zero64(x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_count_trailing_zero64(uint64_t x) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___count_trailing_zero64)
		return __builtin___count_trailing_zero64(x);
	#else
		return __ewl_count_bits64((x & -x) - 1);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_count_leading_one64(uint64_t x) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___count_leading_one64)
		return __builtin___count_leading_one64(x);
	#else
		return __ewl_count_leading_zero64(~x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL __ewl_count_trailing_one64(uint64_t x) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___count_trailing_one64)
		return __builtin___count_trailing_one64(x);
	#else
		return __ewl_count_trailing_zero64(~x);
	#endif
	}

	_EWL_INLINE uint64_t _EWL_MATH_CDECL __ewl_rotate_left64(uint64_t x, int n) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___rotate_left64)
		return __builtin___rotate_left64(x, n);
	#else
		n &= 0x3F;
		return (x << n) | (x >> (64-n));
	#endif
	}

	_EWL_INLINE uint64_t _EWL_MATH_CDECL __ewl_rotate_right64(uint64_t x, int n) _EWL_CANT_THROW
	{
	#if __has_intrinsic(__builtin___rotate_right64)
		return __builtin___rotate_right64(x, n);
	#else
		n &= 0x3F;
		return (x << (64-n)) | (x >> n);
	#endif
	}

#endif /* _EWL_LONGLONG */
#endif /* _EWL_USE_INLINE */

_EWL_END_EXTERN_C

#endif /* _GCCPORT_ */
#endif /* _EWL_MATH_API_H */
