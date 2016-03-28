#ifndef CT_TYPE_LIST_DETAIL_OPERATIONS_H_INCLUDED
#define CT_TYPE_LIST_DETAIL_OPERATIONS_H_INCLUDED

#include <cstddef>

#include "type_list.h"

namespace ct
{
/*!
  tl_get_index metafunction
  in:
      T, that we're looking for
      ListT, our type_list
  out:
      position of T in ListT, starting from zero
 */

template <typename ListT, typename T>
struct tl_get_index;

#if __cplusplus >= 201103L // C++11
template <typename ...Args, typename T>
struct tl_get_index<type_list<T, Args...>, T>
{
    enum { value = 0 };
};
template <typename Arg, typename ...Args, typename T>
struct tl_get_index<type_list<Arg, Args...>, T>
{
    enum { value = 1 + tl_get_index<type_list<Args...>, T>::value };
};

#else  //C++98
template <typename TailT, typename T>
struct tl_get_index<type_list<T, TailT>, T>
{
    enum { value = 0 };
};
template <typename HeadT, typename TailT, typename T>
struct tl_get_index<type_list<HeadT, TailT>, T>
{
    enum { value = 1 + tl_get_index<TailT, T>::value };
};

#endif // __cplusplus

/*!
  tl_get_type metafunction
  in:
      Idx, index of type that we're looking for
      ListT, our type_list
  out:
      type at given position
 */
template <typename ListT, std::size_t Idx>
struct tl_get_type;

#if __cplusplus >= 201103L // C++11
template <typename T, typename ...Args>
struct tl_get_type<type_list<T, Args...>, 0>
{
    using type = T;
};
template <typename T, typename ...Args, std::size_t Idx>
struct tl_get_type<type_list<T, Args...>, Idx>
{
    using type = typename tl_get_type<type_list<Args...>, Idx - 1>::type;
};

#else  //C++98
template <typename HeadT, typename TailT>
struct tl_get_type<type_list<HeadT, TailT>, 0>
{
    typedef HeadT type;
};
template <typename HeadT, typename TailT, std::size_t Idx>
struct tl_get_type<type_list<HeadT, TailT>, Idx>
{
    typedef typename tl_get_type<TailT, Idx - 1>::type type;
};

#endif // __cplusplus

/*!
  tl_contains metafunction
  in:
      T, that we're looking for
      ListT, our type_list
  out:
      boolean value, true if ListT contains T, otherwise - false
 */
template <typename ListT, typename T>
struct tl_contains;

#if __cplusplus >= 201103L // C++11
template <typename ...Args, typename T>
struct tl_contains<type_list<Args...>, T>
{
    enum { value = false };
};
template <typename ...Args, typename T>
struct tl_contains<type_list<T, Args...>, T>
{
    enum { value = true  };
};
template <typename Arg, typename ...Args, typename T>
struct tl_contains<type_list<Arg, Args...>, T>
{
    enum { value = tl_contains<type_list<Args...>, T>::value };
};

#else  //C++98
template <typename T>
struct tl_contains<null_list, T>
{
    enum { value = false };
};
template <typename TailT, typename T>
struct tl_contains<type_list<T, TailT>, T>
{
    enum { value = true  };
};
template <typename HeadT, typename TailT, typename T>
struct tl_contains<type_list<HeadT, TailT>, T>
{
    enum { value = tl_contains<TailT, T>::value };
};

#endif // __cplusplus

/*!

 */
template <typename ListT, typename T>
struct tl_add;

#if __cplusplus >= 201103L // C++11
template <typename ...Args, typename T>
struct tl_add<type_list<Args...>, T>
{
    using type = type_list<Args..., T>;
};

#else  //C++98
template <>
struct tl_add<null_list, null_list>
{
    typedef null_list type;
};
template <typename T>
struct tl_add<null_list, T>
{
    typedef type_list<T> type;
};
template <typename HeadT, typename TailT>
struct tl_add<type_list<HeadT, TailT>, null_list>
{
    typedef type_list<HeadT, TailT> type;
};
template <typename T, typename HeadT, typename TailT>
struct tl_add<type_list<HeadT, TailT>, T>
{
    typedef type_list<HeadT, typename tl_add<TailT, T>::type> type;
};

#endif // __cplusplus

namespace detail_impl
{
    template <typename ListT, typename T, bool Pred>
    struct tl_add_unique__;
} // namespace detail_impl

/*!


Detail:
    Digital-Mars C++ and G++ 2.95.x does not support complicated default
    template parameters
 */
template <typename ListT, typename T>
struct tl_add_unique
    : detail_impl::tl_add_unique__<ListT, T, tl_contains<ListT, T>::value>
{ };

namespace detail_impl
{
#if __cplusplus >= 201103L // C++11
    template <typename ...Args, typename T>
    struct tl_add_unique__<type_list<Args...>, T, true>
    {
        using type = type_list<Args...>;
    };
    template <typename ...Args, typename T>
    struct tl_add_unique__<type_list<Args...>, T, false>
        : tl_add<type_list<Args...>, T>
    { };

#else  //C++98
    template <typename HeadT, typename TailT, typename T>
    struct tl_add_unique__<type_list<HeadT, TailT>, T, true>
    {
        typedef type_list<HeadT, TailT> type;
    };
    template <typename HeadT, typename TailT, typename T>
    struct tl_add_unique__<type_list<HeadT, TailT>, T, false>
        : tl_add<type_list<HeadT, TailT>, T>
    { };
    template <typename T>
    struct tl_add_unique__<null_list, T, false>
        : tl_add<null_list, T>
    { };

#endif // __cplusplus
} // namespace detail_impl

template <typename ListT>
struct tl_size;

#if __cplusplus >= 201103L // C++11
template <typename ...Args>
struct tl_size<type_list<Args...> >
{
    enum { value = sizeof...(Args) };
};

#else  //C++98
template <typename HeadT, typename TailT>
struct tl_size< type_list<HeadT, TailT> >
{
    enum { value = 1 + tl_size<TailT>::value };
};
template <>
struct tl_size<null_list>
{
    enum { value = 0 };
};

#endif // __cplusplus

} // namespace ct

#endif // CT_TYPE_LIST_DETAIL_OPERATIONS_H_INCLUDED
