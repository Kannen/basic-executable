#pragma once
#include <fcpp/c++support.hpp>
#include <fcpp/types.hpp>
#include <fcpp/array.hpp>


namespace std
	{


	template <class T>
	concept visitable = requires (const T& cv)
	        {
		{remove_reference_t <T> :: size} -> convertible_to <size_t>;
		get <0> (cv);
		{index (cv)} -> convertible_to <size_t>;
		};


	template <class Arg>
	struct do_not_visit 
		: tuple<Arg>
		{
		friend constexpr auto
		index(const do_not_visit&) -> ssize_t
			{ return 0; }
		};

	template <class Arg>
	do_not_visit(Arg&&) -> do_not_visit<Arg&&>;


	namespace _impl
		{

		template<size_t N, size_t...Ns>
		struct visiting_all_combinations
			{

			template<class, size_t = sizeof...(Ns)> 
			struct by_i;
	
			template<size_t...Is, size_t R>
			struct by_i <index_sequence <Is...>, R> {

				using sub = visiting_all_combinations <Ns...>;
	            
				template<class F, size_t...Js>
				static constexpr auto
				make_tbl(){
					return array{ sub :: template make_tbl <F,Js...,Is> () ...};
					};

				template<class F,size_t...Js>
				using return_type = common_reference_t <typename sub :: template return_type <F, Js...,Is>...>;

				};

			template<size_t...Is>
			struct by_i <index_sequence <Is...>, 0> {
	            
				template<class F,size_t...Js>
				static constexpr auto
				make_tbl(){
					return array{ F::template get_specialization <Js...,Is> () ...};
					}

				template<class F,size_t...Js>
				using return_type = common_reference_t < typename F :: template return_type <Js...,Is>...>;

				};

	
			template<class F,size_t...Js>
			static constexpr auto
			make_tbl(){
				return by_i <make_index_sequence <N>> :: template make_tbl <F,Js...> ();
				}

			template<class F,size_t...Js>
			using return_type = by_i <make_index_sequence <N>> :: template return_type<F, Js...>;
	
			};
	
	
		template<class R, class F, class...Abstracts>
		struct vis_specializer{
			
			template<size_t...Is>
			static constexpr auto 
			get_specialization(){
				return specialization<Is...>;
				}
			
			template<size_t...Is>
			static constexpr R
			specialization(F f, Abstracts...args){
				return forward <F> (f) (get<Is>(args)...);
			  	}

			};

		template<class F, class...Abstracts>
		struct vis_get_ret{

			template<size_t...Is>
			using return_type = decltype(declval<F>() (get<Is>(declval<Abstracts>())...));
			
			};

		template<class...Abstracts>
		using visiting_all = visiting_all_combinations < remove_reference_t <Abstracts> ::size ... >;
	
		template<class R,class F, class...Abstracts>
		inline constexpr auto 
		visitor_table = visiting_all <Abstracts...> 
		                :: template make_tbl < vis_specializer <R, F, Abstracts...>>();
	
		template <class T, class Arg>
		[[gnu::always_inline]] constexpr auto
		index_vis_tbl(const T& vtbl, const Arg& arg){
			return vtbl[index(arg)];	
			}

		template <class T, class Arg, class...Args>
		[[gnu::always_inline]] constexpr auto
		index_vis_tbl(const T& vtbl, const Arg& arg, const Args&...args){
			return index_vis_tbl (vtbl [index(arg)], args...);	
			}

                template <class T>
		using vis_norm_arg = conditional_t <visitable <T> 
				, T
				, do_not_visit <T>
				>;
		template <class T>
		constexpr decltype(auto)
		vis_cast (T& v){
			if constexpr (visitable <T>)
				return forward <T> (v);
			else
				return do_not_visit <T&&> {forward <T> (v)};
			}

		}


	template<class F, class...Args>
	using visitor_return_type = _impl::visiting_all<Args...>::template return_type< _impl::vis_get_ret <F,Args...>>;


	template<class R, class Fv, class...Argsv, class F, class...Args>
	constexpr decltype(auto) 
	do_visit(F&& f, Args&&...args){
		using namespace _impl;
		return index_vis_tbl (visitor_table <R, Fv, Argsv...>, args...) 
			(forward <F> (f), forward <Args> (args)...);
		}


	template<class R, class F, class...Args>
	constexpr decltype(auto)
	visit(F&& f, Args&&...args){
		using namespace _impl;
		return do_visit <R, F&&, vis_norm_arg<Args&&>...> (forward <F> (f), vis_cast <Args&&> (args)...);
		}

	template<class F, class...Args>
	constexpr decltype(auto)
	visit(F&& f, Args&&...args){
		using namespace _impl;
		using ret_t = visitor_return_type <F, vis_norm_arg <Args&&>...>;
		return visit <ret_t> (forward<F>(f), forward<Args>(args)...);
		}


	}

