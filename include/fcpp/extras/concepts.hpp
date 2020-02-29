#pragma once
#include <concepts>

namespace std{
	template <class Derived, class Base>
	concept not_derived_from = !derived_from <Derived, Base>; 

	template <class A, class B>
	concept not_same_as = !same_as <A, B>; 
	}
