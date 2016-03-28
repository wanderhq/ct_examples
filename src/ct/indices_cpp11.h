#ifndef CT_INDICES_CPP11_HPP_INCLUDED
#define CT_INDICES_CPP11_HPP_INCLUDED

#include <cstddef>

namespace ct
{
#if __cplusplus >= 201103L // C++11

template <size_t ...I>
struct indices
{ };

template <size_t Max, size_t ...Indices>
struct make_indices
    : make_indices<Max - 1, Max - 1, Indices...>
{ };

template <size_t ...Indices>
struct make_indices<0, Indices...>
    : indices<Indices...>
{
    using type = indices<Indices...>;
};

#endif
} // namespace ct

#endif // CT_INDICES_CPP11_HPP_INCLUDED
