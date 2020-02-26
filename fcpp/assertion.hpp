#pragma once

//	This header file provides 2 macros: `Assert` and `Assert_heavy` and the declaration of
//	a function that the user must implement: `std::dynamic_assertion_failure`
//
//	The behaviour can be configured with a macro: `ASSERTION_LEVEL`:
//
//	The macros `Assert` `Assert_heavy` test a condition which is given as their
//	first argument and may generates error or enable optimization depending
//	on ASSERTION_LEVEL:
//
//	*ASSERTION_LEVEL==2* (default if NDEBUG not defined)
//		With this assertion level all assertion are tested. When the assertion
//		can be constant evaluated the error is generated at compile time. Otherwise
//		the function `dynamic_assertion_failure` is called.
//
//	*ASSERTION_LEVEL==1* (default if NDEBUG defined)
//		With this assertion level `Assert_heavy` is not evaluated. `Assert` make its 
//		best to check that the expression is light weight and does not have side effects.
//		The condition still appears in the preprocessed source code an it is expected that 
//		the optimizer remove it: it should not appear in the resulting code.
//		Nevertheless the compiler should be able to perform optimization because it is informed
//		that an assertion failure is equivalent to undefined behaviour.((!expr) => __builtin_unreachable())
//
//	*ASSERTION_LEVEL==0*
//		No assertion are evaluated
//	   
//	The macros `Assert` and `Assert_heavy` can takes more arguments. Those extra argument
//	are supposed to be variables that are evaluated during the condition evaluation.
//	With ASSERTION_LEVEL==2, if all those value are constant expression and if the condition
//	evaluation is not a constant expression, the compiler will generate an error saying that 
//	the assertion evaluation break a constant evaluation.
//
//	**Rational**
//	assertion can occur in 3 different contexts where a compile time error may be more appropriate:
//	   - dynamic computation
//	   - constant evaluation
//	   - constexpr evaluation
//	
//	The risk of using an assertion is to break a constant evaluation: in the abscence of the 
//	assertion the code could be constant evaluated but it is not any more due the evaluation
//	of the condition.
//	
//	In constant evaluation, it could be interesting to have a compile time error.
//


#ifndef ASSERTION_LEVEL
#  ifdef NDEBUG
#    define ASSERTION_LEVEL 1
#  else
#    define ASSERTION_LEVEL 2
#  endif
#endif


#if ASSERTION_LEVEL == 2

#	define Assert(expr,...) \
		do{\
 		const bool _fcpp_assert_eval = static_cast<bool>(expr);\
	    __VA_OPT__(		if (__builtin_constant_p((__VA_ARGS__)) && !__builtin_constant_p(_fcpp_assert_eval)){\
				extern void constant_evaluation_broken [[gnu::error("assertion evaluation break constant evaluation.")]]();\
				constant_evaluation_broken();\
				} )\
		if (!_fcpp_assert_eval) [[unlikely]] {\
			if (__builtin_constant_p(_fcpp_assert_eval)){\
				extern void constant_assertion_failure [[gnu::error("assertion failure in constant expression")]]();\
				constant_assertion_failure();\
				}\
			::std::dynamic_assertion_failure (#expr, __FILE__, __LINE__);\
			}\
		}while(false)

#	define Assert_heavy(expr,...) Assert(expr __VA_OPT__(,) __VA_ARGS__)

#	define Assert_unreachable() \
			::std::dynamic_assertion_failure ("Reached unreachable assertion.", __FILE__, __LINE__)

#elif ASSERTION_LEVEL == 1

#   define Assert(expr,...) \
		do{\
		if (::std::_impl::eval_light_weight_assertion ([&] {return !static_cast<bool>(expr);})) __builtin_unreachable();\
		}while(false)

#   define Assert_heavy(expr,...) static_cast<int>(0)

#   define Assert_unreachable() \
		 	__builtin_unreachable()	

#elif ASSERTION_LEVEL == 0

#   define Assert(expr,...) static_cast<int>(0)

#   define Assert_heavy(expr,...) static_cast<int>(0)

#   define Assert_unreachable() static_cast<int>(0)

#else

#  error "ASSERTION_LEVEL should be equals to 2 (full assertion check)"\
         ", 1 (light assertion check used for optimization)"
         ", 0 (no assertion check)"

#endif


namespace std
	{ 

	void dynamic_assertion_failure (const char* expression, const char* file, int line);

	}


namespace std::_impl
	{

	template <class T>
	[[using gnu: noinline, error("assertion generates code: condition evaluation certainly has side-effects.")]] constexpr bool
	eval_light_weight_assertion (T&& expr)
		{
		return expr();
		}
	
	}
