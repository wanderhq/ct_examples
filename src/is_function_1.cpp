#include <iostream>
#include <ct/type_traits.h>
#include <ct/enable_if.h>

//#1
namespace detail
{
    template <typename F>
    struct may_add_ptr
    {
        template <typename P>
        static ct::yes_type may_add_ptr_check(P *);
        template <typename P>
        static ct::no_type  may_add_ptr_check(...);

        enum
        {
            value = sizeof( may_add_ptr_check<F>(0) ) - 1
        };
    };

    template <typename Tp>
    struct is_function_helper
    {
        template <typename F>
        static typename ct::enable_if<
            may_add_ptr<F>::value == 1
          , void (*)(typename ct::remove_reference<F>::type *)
        >::type declfunc();

        template <typename F>
        static typename ct::enable_if<
            may_add_ptr<F>::value == 0
          , void (*)(typename ct::remove_reference<F>::type  )
        >::type declfunc();

        template <typename F>
        static ct::yes_type is_function_check( void(*)( F ) );

        template <typename F>
        static ct::no_type  is_function_check( ... );

        enum
        {
            value = sizeof( is_function_check<Tp>( declfunc<Tp>() ) ) - 1
        };
    };
} // detail

template <typename Tp>
struct is_function
    : ct::integral_constant<bool, detail::is_function_helper<Tp>::value>
{ };


struct S { virtual void f() = 0; };

#define TEST_IS_FUNCTION(Type, R) \
    std::cout << ((::is_function<Type>::value == R) ? "[SUCCESS]" : "[FAILED]") \
              << " Test is_function<" #Type "> (should be [" #R "]):" \
              << std::boolalpha \
              << ::is_function<Type>::value << std::endl

int main()
{
    typedef void f1() const;
    typedef void f2() volatile;
    typedef void f3() const volatile;

    TEST_IS_FUNCTION(void(int), true);
    TEST_IS_FUNCTION(void(), true);
    TEST_IS_FUNCTION(f1, true);
    TEST_IS_FUNCTION(void(*)(int), false);
    TEST_IS_FUNCTION(void(&)(int), false);
    TEST_IS_FUNCTION(f2, true);
    TEST_IS_FUNCTION(f3, true);
    TEST_IS_FUNCTION(void(S::*)(), false);
    TEST_IS_FUNCTION(void(S::*)() const, false);
    TEST_IS_FUNCTION(S, false);
    TEST_IS_FUNCTION(int, false);
    TEST_IS_FUNCTION(int *, false);
    TEST_IS_FUNCTION(int [], false);
    TEST_IS_FUNCTION(int [2], false);
    TEST_IS_FUNCTION(int **, false);
    TEST_IS_FUNCTION(double, false);
    TEST_IS_FUNCTION(int *[], false);
    TEST_IS_FUNCTION(int &, false);
    TEST_IS_FUNCTION(int const &, false);
    TEST_IS_FUNCTION(void(...), true);
    TEST_IS_FUNCTION(int S::*, false);
    TEST_IS_FUNCTION(void, false);
    TEST_IS_FUNCTION(void const, false);
}
