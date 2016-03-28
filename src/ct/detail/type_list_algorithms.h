#ifndef CT_TYPE_LIST_DETAIL_ALGORITHMS_H_INCLUDED
#define CT_TYPE_LIST_DETAIL_ALGORITHMS_H_INCLUDED

#include "type_list.h"
#include "type_list_operations.h"

#include "../declval.h"

namespace ct
{
namespace detail_impl
{
    template <typename ListT, typename DestLisT>
    struct tl_generate_unique__;
}

/*!
   tl_generate_unique metafunction
   in:
      ListT, our type_list with possible duplicates
   out:
      type_list without duplicates
 */
template <typename ListT>
struct tl_generate_unique
    : detail_impl::tl_generate_unique__<ListT, make_list<>::type>
{ };

namespace detail_impl
{
#if __cplusplus >= 201103L // C++11
    template <typename Arg, typename ...Args, typename DestLisT>
    struct tl_generate_unique__<type_list<Arg, Args...>, DestLisT>
        : tl_generate_unique__<type_list<Args...>, typename tl_add_unique<Arg, DestLisT>::type>
    { };
    template <typename Arg, typename DestLisT>
    struct tl_generate_unique__<type_list<Arg>, DestLisT>
        : tl_add_unique<Arg, DestLisT>
    { };
    template <typename DestListT>
    struct tl_generate_unique__<type_list<>, DestListT>
    {
        using type = DestListT;
    };

#else  // C++98
    template <typename HeadT, typename TailT, typename DestLisT>
    struct tl_generate_unique__<type_list<HeadT, TailT>, DestLisT>
        : tl_generate_unique__<TailT, typename tl_add_unique<DestLisT, HeadT>::type>
    { };
    template <typename HeadT, typename DestLisT>
    struct tl_generate_unique__<type_list<HeadT>, DestLisT>
        : tl_add_unique<DestLisT, HeadT>
    { };
    template <typename DestListT>
    struct tl_generate_unique__<null_list, DestListT>
    {
        typedef DestListT type;
    };

#endif
} // namespace detail_impl

namespace detail_impl
{
    template <typename L, size_t Idx = 1>
    struct overload_generator__;

#if __cplusplus >= 201103L // C++11
    template <typename Arg, typename ...Args, size_t Idx>
    struct overload_generator__<type_list<Arg, Args...>, Idx>
        : overload_generator__<type_list<Args...>, Idx + 1>
    {
        using overload_generator__<type_list<Args...>, Idx + 1>::check;

        static char (&check(Arg const &))[Idx];
    };

    template <typename Arg, size_t Idx>
    struct overload_generator__<type_list<Arg>, Idx>
    {
        static char (&check(Arg const &))[Idx];
    };

#else  // C++98
    template <typename HeadT, typename TailT, size_t Idx>
    struct overload_generator__<type_list<HeadT, TailT>, Idx>
        : overload_generator__<TailT, Idx + 1>
    {
        typedef overload_generator__<TailT, Idx + 1> base_type;

        using base_type::check;
        static char (&check(HeadT const &))[Idx];
    };

    template <typename HeadT, size_t Idx>
    struct overload_generator__<type_list<HeadT>, Idx>
    {
        static char (&check(HeadT const &))[Idx];
    };

#endif
} // namespace detail_impl

template <typename ListT, typename T>
struct tl_find_best_match
{
public:
    enum
    {
        value = sizeof(detail_impl::overload_generator__<ListT>::check(ct::declval<T>())) - 1
    };

    typedef typename ct::tl_get_type<ListT, value>::type type;
};

} // namespace ct

#endif // CT_TYPE_LIST_DETAIL_ALGORITHMS_H_INCLUDED
