#pragma once
#include <ranges>
#include <iterator>
#include <tuple>

namespace std::ranges
	{

	template <input_or_output_iterator...Its>
		requires  (  (input_iterator <Its> && ...)
		          || (!input_iterator <Its> && ...))
	struct zip_iterator
		{

		using iterator_category = 
			 conditional_t   < (bidirectional_iterator <Its> && ...)
			                , bidirectional_iterator_tag
			,conditional_t < (forward_iterator <Its> && ...)
			 		 , forward_iterator_tag
			,conditional_t < (input_iterator <Its> && ...)
					 , input_iterator_tag
			 		 , output_iterator_tag
			       	 	>>>;
			 
		using value_type = std::tuple < iter_reference_t <Its> ...>;
		using reference = value_type;
		using difference_type = std::common_type_t < iter_difference_t <Its> ...>;

		std::tuple <Its...> value;

		zip_iterator() = default;

		constexpr
		zip_iterator(Its...args)
			:value {std::move(args)...}
			{}

		reference
		operator *() const
			{
			return _deref(std::index_sequence_for <Its...> {});
			}

		zip_iterator&
		operator++()
			{
			_for_each([](auto&...its){(++its,...);});
			return *this;
			}

		zip_iterator
		operator++(int)
			{
			auto old = *this;
			operator++();
			return old;
			}

		zip_iterator&
		operator--() 
			requires (bidirectional_iterator<Its> && ...)
			{
			_for_each([](auto&...its){(--its,...);});
			return *this;
			}

		zip_iterator
		operator--(int) 
			requires (bidirectional_iterator<Its> && ...)
			{
			auto old = *this;
			operator--();
			return old;
			}

		zip_iterator&
		operator += (difference_type i) 
			requires ((random_access_iterator<Its> && ...))
			{
			_for_each ([i] (auto&...its) { ((its += i),...); });
			return *this;
			}

		zip_iterator&
		operator -= (difference_type i) 
			requires (random_access_iterator<Its> && ...)
			{
			_for_each ([i] (auto&...its) { ((its -= i),...); });
			return *this;
			}

		auto
		operator [] (difference_type i) const 
			requires (random_access_iterator<Its> && ...)
			{
			return _deref(std::index_sequence_for <Its...> {}, i);
			}

		friend zip_iterator
		operator + (zip_iterator self, difference_type i) 
			requires (random_access_iterator <Its> && ...)
			{
			self += i;
			return self;
			}

		friend zip_iterator
		operator + (difference_type i, zip_iterator self) 
			requires (random_access_iterator <Its> && ...)
			{
			return self+i;
			}

		friend zip_iterator
		operator - (zip_iterator self, difference_type i) 
			requires (random_access_iterator <Its> && ...)
			{
			self -= i;
			return self;
			}


		private:
		template <class F, size_t...Is>
		void
		_for_each(const F& f,std::index_sequence <Is...>)
			{
			return f( get<Is>(value)...);
			}
		template <class F>
		void
		_for_each(const F& f)
			{
			return _for_each (f,std::index_sequence_for<Its...> {});
			}

		template <size_t...Is>
		reference
		_deref (std::index_sequence <Is...>) const
			{
			return reference {* get <Is> (value) ...};
			}

		template <size_t...Is>
		auto
		_deref (std::index_sequence <Is...>, difference_type i) const
			{
			return reference {get <Is> (value) [i] ...};
			}

		bool
		operator == (const zip_iterator&) const
			= default;

		auto
		operator <=> (const zip_iterator&) const
			= default;

		};

	template <class...Its>
	zip_iterator (Its...) -> zip_iterator <Its...>;


	struct zip_sentinel
		{

		template <class...Ts>
		friend auto
		operator == (zip_sentinel, const zip_iterator <Ts...> & it)
			-> bool
			{
			return get<0>(it.value) == default_sentinel_t{};
			}

		template <class...Ts>
		friend auto
		operator - (zip_sentinel, const zip_iterator <Ts...> & it)
			{
			return get<0>(it.value).count();
			}

		template <class...Ts>
		friend auto
		operator - (const zip_iterator <Ts...> & it, zip_sentinel)
			{
			return - (zip_sentinel{} - it);
			}

		};

	template <class It, class St>
	struct zip_view
		: subrange <It,St>
		{
		zip_view() = default;
		zip_view (It i, St s)
			: subrange <It,St> (i,s)
			{}
		};

	template <class It, class St>
	zip_view(It,St) -> zip_view <It,St>;

	struct zip_t
		{

		explicit 
		zip_t() =default;

		template <range R, range...Rs>
		constexpr auto
		operator () (R&& r, Rs&&...rs) const
			{
			using std::ranges::size; 
			using std::ranges::begin, std::ranges::end;
			using std::ranges::min;
			if constexpr ( (sized_range <R> && ... && sized_range <Rs>) ){
				auto min_size = min ({size(r), size(rs)...});
				auto ct = counted_iterator {begin(r), min_size};
				auto it = zip_iterator {ct, begin(rs)...};
				auto v = zip_view {it,zip_sentinel{}};
				return v;
				}
			else {
				auto it = zip_iterator {begin(r), begin(rs)...};
				auto e = zip_iterator {end(r), end(rs)...};
				auto v = zip_view {it, e};
				return v;
				}
			}
		};

	inline constexpr auto zip = zip_t{};

	}
