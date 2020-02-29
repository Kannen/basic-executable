#pragma once

namespace std{

template <class Derived, class From = Derived>
struct self_interface
	{
	protected:

	[[gnu::always_inline]] const Derived &
	self() const &
		{
		static_assert (std::is_base_of_v <self_interface, Derived>);
		static_assert (std::is_base_of_v <self_interface, From>);
		static_assert (std::is_base_of_v <From, Derived>);
		return static_cast <const Derived&> (*this);
		}

	[[gnu::always_inline]] Derived &
	self() &
		{
		static_assert (std::is_base_of_v <self_interface, Derived>);
		static_assert (std::is_base_of_v <self_interface, From>);
		static_assert (std::is_base_of_v <From, Derived>);
		return static_cast <Derived &> (*this);
		}
	[[gnu::always_inline]] Derived &&
	self() && 
		{
		static_assert (std::is_base_of_v <self_interface, Derived>);
		static_assert (std::is_base_of_v <self_interface, From>);
		static_assert (std::is_base_of_v <From, Derived>);
		return static_cast <Derived &&> (*this);
		}

	[[gnu::always_inline]] const Derived &&
	self() const && 
		{
		static_assert (std::is_base_of_v <self_interface, Derived>);
		static_assert (std::is_base_of_v <self_interface, From>);
		static_assert (std::is_base_of_v <From, Derived>);
		return static_cast <const Derived &&> (*this);
		}

	auto
	operator == (const self_interface &) const
		-> bool  = default;

	auto
	operator <=> (const self_interface &) const
		= default;

	};

}
