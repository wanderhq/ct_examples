#include <iostream>
#include <ct/type_traits.h>

//#2
namespace detail
{
    template <typename Tp>
    struct is_function_helper
    {
    private:
        template <typename F>
        static ct::yes_type check_is_function( ... );
        template <typename F>
        static ct::no_type  check_is_function( F (*)[1] );

    public:
        enum
        {
            value = !ct::is_class<Tp>::value
                 && !ct::is_void<Tp>::value
                 && !ct::is_reference<Tp>::value
                 && !ct::is_array<Tp>::value
                 && (sizeof( check_is_function<Tp>(0) ) - 1)
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
              << (bool)::is_function<Type>::value << std::endl

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
