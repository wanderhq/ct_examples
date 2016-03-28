#ifndef CT_TYPE_LIST_TUPLE_HPP_INCLUDED_
#define CT_TYPE_LIST_TUPLE_HPP_INCLUDED_

#include "enable_if.h"
#include "type_traits.h"
#include "int_to_type.h"
#include "indices_cpp11.h"

namespace ct
{

template <typename T, size_t I, typename Dummy = void>
struct tl_tuple_element;

template <typename T, size_t I>
struct tl_tuple_element<T, I, typename enable_if<is_class<T>::value>::type>
    : T
{
    T & get()
    { return *this; }
    T const & get() const
    { return *this; }
};

template <typename T, size_t I>
struct tl_tuple_element<T, I, typename enable_if<!is_class<T>::value>::type>
{
    T & get()
    { return m_value; }
    T const & get() const
    { return m_value; }

    T m_value;
};

#if __cplusplus >= 201103L // C++11

template <typename ListT, typename I>
struct tl_tuple_gen;

template <typename ...Args, size_t ...Indexes>
struct tl_tuple_gen<type_list<Args...>, indices<Indexes...>>
    : public tl_tuple_element<Args, Indexes>...
{ };

template <typename ListT>
struct tl_tuple_generator
    : private tl_tuple_gen<ListT, typename make_indices<tl_size<ListT>::value>::type>
{
    template <size_t I>
    typename tl_get_type<ListT, I>::type & get()
    {
        typedef typename tl_get_type<ListT, I>::type element_type;
        return tl_tuple_element<element_type, I>::get();
    }
    template <size_t I>
    typename tl_get_type<ListT, I>::type const & get() const
    {
        typedef typename tl_get_type<ListT, I>::type element_type;
        return tl_tuple_element<element_type, I>::get();
    }
};

#else

template <typename ListT, size_t I>
struct tl_tuple_gen;

template <typename ListT, size_t I>
struct tl_tuple_gen
    : tl_tuple_gen<ListT, I - 1>
    , private tl_tuple_element<typename ct::tl_get_type<ListT, I>::type, I>
{
    typedef typename ct::tl_get_type<ListT, I>::type element_type;

    element_type & get()
    { return tl_tuple_element<element_type, I>::get(); }
    element_type const & get() const
    { return tl_tuple_element<element_type, I>::get(); }
};

template <typename ListT>
struct tl_tuple_gen<ListT, 0>
    : private tl_tuple_element<typename ct::tl_get_type<ListT, 0>::type, 0>
{
    typedef typename ct::tl_get_type<ListT, 0>::type element_type;

    element_type & get()
    { return tl_tuple_element<element_type, 0>::get(); }
    element_type const & get() const
    { return tl_tuple_element<element_type, 0>::get(); }
};

template <typename ListT>
struct tl_tuple_generator
    : private tl_tuple_gen<ListT, tl_size<ListT>::value - 1>
{
    template <size_t I>
    typename ct::tl_get_type<ListT, I>::type & get()
    {
        return tl_tuple_gen<ListT, I>::get();
    }
    template <size_t I>
    typename ct::tl_get_type<ListT, I>::type const & get() const
    {
        return tl_tuple_gen<ListT, I>::get();
    }
};

#endif

} // namespace ct

#endif // CT_TYPE_LIST_TUPLE_HPP_INCLUDED_
