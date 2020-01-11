#include <fcpp/c++support.hpp>


using namespace std;


namespace {
  template<class A,class B,class=void> struct has_common_ref:false_type{};
  template<class A,class B> struct has_common_ref<A,B,void_t<common_reference_t<A,B>>>:true_type{};

  struct A{};
  struct B:A{};
  
  template<class T>
  struct D{
    operator T()const&;
    };
  }

static_assert(is_same_v<common_reference_t<const int&>        ,const int&>);
static_assert(is_same_v<common_reference_t<const int& ,int&, const volatile int&>        ,const volatile int&>);

static_assert(is_same_v<common_reference_t<int        ,int>        ,int>);
static_assert(is_same_v<common_reference_t<const int  ,int>        ,int>);
static_assert(is_same_v<common_type_t     <const int  ,int>        ,int>);
static_assert(is_same_v<common_reference_t<int        ,int&>       ,int>);
static_assert(is_same_v<common_reference_t<int        ,const int&> ,int>);
static_assert(is_same_v<common_reference_t<int        ,int&&>      ,int>);
static_assert(is_same_v<common_reference_t<int        ,const int&&>,int>);
static_assert(is_same_v<common_reference_t<int&       ,int&>       ,int&>);
static_assert(is_same_v<common_reference_t<int&       ,const int&> ,const int&>);
static_assert(is_same_v<common_reference_t<int&       ,int&&>      ,const int&>);
static_assert(is_same_v<common_reference_t<int&       ,const int&&>,const int&>);
static_assert(is_same_v<common_reference_t<const int& ,const int&> ,const int&>);
static_assert(is_same_v<common_reference_t<const int& ,int&&>      ,const int&>);
static_assert(is_same_v<common_reference_t<const int& ,const int&&>,const int&>);
static_assert(is_same_v<common_reference_t<int&&      ,int&&>      ,int&&>);
static_assert(is_same_v<common_reference_t<int&&      ,const int&&>,const int&&>);
static_assert(is_same_v<common_reference_t<const int&&,const int&&>,const int&&>);

static_assert(is_same_v<common_reference_t<unsigned int        ,int>        ,unsigned int>);
static_assert(is_same_v<common_reference_t<unsigned int        ,int&>       ,unsigned int>);
static_assert(is_same_v<common_reference_t<unsigned int        ,const int&> ,unsigned int>);
static_assert(is_same_v<common_reference_t<unsigned int        ,int&&>      ,unsigned int>);
static_assert(is_same_v<common_reference_t<unsigned int        ,const int&&>,unsigned int>);
static_assert(is_same_v<common_reference_t<unsigned int&       ,int&>       ,unsigned int>);
static_assert(is_same_v<common_reference_t<unsigned int&       ,const int&> ,unsigned int>);
static_assert(is_same_v<common_reference_t<unsigned int&       ,int&&>      ,unsigned int>);
static_assert(is_same_v<common_reference_t<unsigned int&       ,const int&&>,unsigned int>);
static_assert(is_same_v<common_reference_t<const unsigned int& ,const int&> ,unsigned int>);
static_assert(is_same_v<common_reference_t<const unsigned int& ,int&&>      ,unsigned int>);
static_assert(is_same_v<common_reference_t<const unsigned int& ,const int&&>,unsigned int>);
static_assert(is_same_v<common_reference_t<unsigned int&&      ,int&&>      ,unsigned int&&>);
static_assert(is_same_v<common_reference_t<unsigned int&&      ,const int&&>,unsigned int&&>);
static_assert(is_same_v<common_reference_t<const unsigned int&&,const int&&>,unsigned int>);

static_assert(is_same_v<common_reference_t<int&       ,int>        ,int>);
static_assert(is_same_v<common_reference_t<const int& ,int>        ,int>);
static_assert(is_same_v<common_reference_t<int&&      ,int>        ,int>);
static_assert(is_same_v<common_reference_t<const int&&,int>        ,int>);
static_assert(is_same_v<common_reference_t<const int& ,int&>       ,const int&>);
static_assert(is_same_v<common_reference_t<int&&      ,int&>       ,const int&>);
static_assert(is_same_v<common_reference_t<const int&&,int&>       ,const int&>);
static_assert(is_same_v<common_reference_t<const int& ,int&&>      ,const int&>);
static_assert(is_same_v<common_reference_t<const int& ,const int&&>,const int&>);
static_assert(is_same_v<common_reference_t<const int&&,int&&>      ,const int&&>);

static_assert(is_same_v<common_reference_t<A        ,B>        ,A>);
static_assert(is_same_v<common_reference_t<A        ,B&>       ,A>);
static_assert(is_same_v<common_reference_t<A        ,const B&> ,A>);
static_assert(is_same_v<common_reference_t<A        ,B&&>      ,A>);
static_assert(is_same_v<common_reference_t<A        ,const B&&>,A>);
static_assert(is_same_v<common_reference_t<A&       ,B&>       ,A&>);
static_assert(is_same_v<common_reference_t<A&       ,const B&> ,const A&>);
static_assert(is_same_v<common_reference_t<A&       ,B&&>      ,const A&>);
static_assert(is_same_v<common_reference_t<A&       ,const B&&>,const A&>);
static_assert(is_same_v<common_reference_t<const A& ,const B&> ,const A&>);
static_assert(is_same_v<common_reference_t<const A& ,B&&>      ,const A&>);
static_assert(is_same_v<common_reference_t<const A& ,const B&&>,const A&>);
static_assert(is_same_v<common_reference_t<A&&      ,B&&>      ,A&&>);
static_assert(is_same_v<common_reference_t<A&&      ,const B&&>,const A&&>);
static_assert(is_same_v<common_reference_t<const A&&,const B&&>,const A&&>);

static_assert(is_same_v<common_reference_t<B        ,A>        ,A>);
static_assert(is_same_v<common_reference_t<B&       ,A>        ,A>);
static_assert(is_same_v<common_reference_t<const B& ,A>        ,A>);
static_assert(is_same_v<common_reference_t<B&&      ,A>        ,A>);
static_assert(is_same_v<common_reference_t<const B&&,A>        ,A>);
static_assert(is_same_v<common_reference_t<B&       ,A&>       ,A&>);
static_assert(is_same_v<common_reference_t<const B& ,A&>       ,const A&>);
static_assert(is_same_v<common_reference_t<B&&      ,A&>       ,const A&>);
static_assert(is_same_v<common_reference_t<const B&&,A&>       ,const A&>);
static_assert(is_same_v<common_reference_t<const A& ,const B&> ,const A&>);
static_assert(is_same_v<common_reference_t<const A& ,B&&>      ,const A&>);
static_assert(is_same_v<common_reference_t<const A& ,const B&&>,const A&>);
static_assert(is_same_v<common_reference_t<B&&      ,A&&>      ,A&&>);
static_assert(is_same_v<common_reference_t<const B&&,A&&>      ,const A&&>);
static_assert(is_same_v<common_reference_t<const A&&,const B&&>,const A&&>);
