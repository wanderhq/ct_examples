#ifndef TYPE_TRAITS_H_INCLUDED_
#define TYPE_TRAITS_H_INCLUDED_

#include "logical.h"
#include "declval.h"

#include "sfinae_types.h"

#include <cstddef>

namespace ct
{

// const-volatile modifications.
// remove_const
template <typename Tp>
struct remove_const
{
    typedef Tp     type;
};
template <typename Tp>
struct remove_const<Tp const>
{
    typedef Tp     type;
};

// remove_volatile
template <typename Tp>
struct remove_volatile
{
    typedef Tp     type;
};
template <typename Tp>
struct remove_volatile<Tp volatile>
{
    typedef Tp     type;
};

// remove_cv
template <typename Tp>
struct remove_cv
    : remove_const<typename remove_volatile<Tp>::type>
{ };

// add_const
template <typename Tp>
struct add_const
{
    typedef Tp const     type;
};

// add_volatile
template <typename Tp>
struct add_volatile
{
    typedef Tp volatile  type;
};

// add_cv
template <typename Tp>
struct add_cv
    : add_const<typename add_volatile<Tp>::type>
{ };

// is_void
template <typename Tp>
struct is_void
    : false_type
{ };
template <>
struct is_void<void>
    : true_type
{ };
template <>
struct is_void<void const>
    : true_type
{ };

// is_array
template <typename Tp>
struct is_array
    : false_type
{ };
template <typename Tp, std::size_t Size>
struct is_array<Tp[Size]>
    : true_type
{ };
template <typename Tp>
struct is_array<Tp[]>
    : true_type
{ };

// is_same
template <typename Tp1, typename Tp2>
struct is_same
    : false_type
{ };
template <typename Tp1>
struct is_same<Tp1, Tp1>
    : true_type
{ };

// is_class
namespace detail
{
    template <typename Tp>
    struct is_class_helper
    {
    private:
        template <typename C>
        static yes_type is_class_check(int C:: *);
        template <typename C>
        static no_type  is_class_check(...);

    public:
        enum
        {
            value = sizeof( is_class_check<Tp>(0) ) - 1
        };
    };
}  // detail
template <typename Tp>
struct is_class
    : integral_constant<bool, detail::is_class_helper<Tp>::value>
{ };

// is_pointer
namespace detail
{
    template <typename Tp>
    struct is_pointer_helper
        : false_type
    { };

    template <typename Tp>
    struct is_pointer_helper<Tp *>
        : true_type
    { };
} // detail
template <typename Tp>
struct is_pointer
    : detail::is_pointer_helper<typename remove_cv<Tp>::type>::type
{ };

// is_reference
template <typename Tp>
struct is_lvalue_reference
    : false_type
{ };
template <typename Tp>
struct is_lvalue_reference<Tp &>
    : true_type
{ };

template <typename Tp>
struct is_rvalue_reference
    : false_type
{ };
#if __cplusplus >= 201103L
template <typename Tp>
struct is_rvalue_reference<Tp &&>
    : true_type
{ };
#endif

template <typename Tp>
struct is_reference
    : or_< is_lvalue_reference<Tp>, is_rvalue_reference<Tp> >::type
{ };

// is_convertible
namespace detail
{
    template <typename Tp1, typename Tp2>
    struct is_convertible_helper
    {
    private:
        template <typename U>
        static yes_type is_convertible_check(U const &);
        template <typename U>
        static no_type  is_convertible_check(...);

    public:
        enum
        {
            value = sizeof( is_convertible_check<Tp2>(declval<Tp1>()) ) - 1
        };
    };
    template <typename Tp>
    struct is_convertible_helper<Tp, Tp>
    {
    private:
        template <typename U>
        static yes_type is_convertible_check(U);
        template <typename U>
        static no_type  is_convertible_check(...);

    public:
        enum
        {
            value = sizeof( is_convertible_check<Tp>(declval<Tp>()) ) - 1
        };
    };
} // detail
template <typename Tp1, typename Tp2>
struct is_convertible
    : integral_constant<bool, detail::is_convertible_helper<Tp1, Tp2>::value>
{ };

// reference modifications
// remove_reference
template <typename Tp>
struct remove_reference
{
    typedef Tp   type;
};
template <typename Tp>
struct remove_reference<Tp &>
{
    typedef Tp   type;
};
#if __cplusplus >= 201103L
template <typename Tp>
struct remove_reference<Tp &&>
{
    typedef Tp   type;
};
#endif

// add_reference
// add_lvalue_reference
namespace detail
{
    template <typename Tp, bool IsRefOrVoid, bool IsRvRef>
    struct add_lvalue_reference_helper
    {
        typedef Tp   type;
    };
    template <typename Tp>
    struct add_lvalue_reference_helper<Tp, false, false>
    {
        typedef Tp & type;
    };
    template<typename Tp>
    struct add_lvalue_reference_helper<Tp, true, true>
    {
        typedef typename remove_reference<Tp>::type & type;
    };
} // detail
template <typename Tp>
struct add_lvalue_reference
    : detail::add_lvalue_reference_helper<
          Tp
        , is_reference<Tp>::value || is_void<Tp>::value
        , is_rvalue_reference<Tp>::value
      >
{ };

// add_rvalue_reference
namespace detail
{
    template <typename Tp, bool IsRefOrVoid>
    struct add_rvalue_reference_helper
    {
        typedef Tp     type;
    };
#if __cplusplus >= 201103L
    template <typename Tp>
    struct add_rvalue_reference_helper<Tp, false>
    {
        typedef Tp &&  type;
    };
#endif
} // detail
template <typename Tp>
struct add_rvalue_reference
    : detail::add_rvalue_reference_helper<
          Tp
        , is_reference<Tp>::value || is_void<Tp>::value
      >
{ };

// pointer modifications
// remove_pointer
namespace detail
{
    template <typename Tp, typename Up>
    struct remove_pointer_helper
    {
        typedef Tp     type;
    };
    template <typename Tp, typename Up>
    struct remove_pointer_helper<Tp, Up *>
    {
        typedef Up     type;
    };
} // detail
template <typename Tp>
struct remove_pointer
    : detail::remove_pointer_helper<Tp, typename remove_cv<Tp>::type>
{ };

// add_pointer
template <typename Tp>
struct add_pointer
{
    typedef typename remove_reference<Tp>::type * type;
};

} // ct

#endif // TYPE_TRAITS_H_INCLUDED_
