#ifndef CT_LOGICAL_H_INCLUDED_
#define CT_LOGICAL_H_INCLUDED_

#include "integral_constant.h"

namespace ct
{

// Conditional (IF)
template <bool Cond, typename ThenTp, typename ElseTp>
struct conditional
{
    typedef ThenTp type;
};

template <typename ThenTp, typename ElseTp>
struct conditional<false, ThenTp, ElseTp>
{
    typedef ElseTp type;
};

template <bool Cond, typename ThenTp, typename ElseTp>
struct if_
    : conditional<Cond, ThenTp, ElseTp>::type
{ };

// OR
template <typename Tp1, typename Tp2>
struct or_
    : conditional<Tp1::value, Tp1, Tp2>::type
{ };

// AND
template <typename Tp1, typename Tp2>
struct and_
    : conditional<Tp1::value, Tp2, Tp1>::type
{ };

// NOT
template <typename Tp>
struct not_
    : integral_constant<bool, !Tp::value>
{ };

} // ct

#endif // CT_LOGICAL_H_INCLUDED_
