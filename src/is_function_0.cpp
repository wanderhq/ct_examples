#include <iostream>
#include <ct/type_traits.h>

//#0
namespace detail
{
    template <typename F>
    struct func_ptr
    {
        typedef void(* type)(F);
    };

    template <typename T, bool IsAllowed>
    struct is_function_helper
    {
        template <typename F>
        static typename func_ptr<F>::type declfunc();

        template <typename F>
        static void (* gen( void(*)(F *) ) )( F   );
        template <typename F>
        static void (* gen( void(*)(F  ) ) )( F * );

        template <typename F>
        static char (& check_is_function( F ) )
        [
            ct::is_same<void(*)( T ), F>::value + 1
        ];
        enum
        {
            value = sizeof(
                        check_is_function(
                            gen(
                                declfunc<typename ct::remove_reference<T>::type>()
                            )
                        )
                    ) - 1
        };
    };

    template <typename F>
    struct is_function_helper<F, true>
        : ct::false_type
    { };
} // detail

template <typename F>
struct is_function
    : ct::integral_constant<bool
        , detail::is_function_helper<
              F
            , ct::is_pointer<F>::value
           || ct::is_class<F>::value
           || ct::is_void<F>::value
          >::value
      >
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
