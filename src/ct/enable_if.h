#ifndef CT_ENABLE_IF_H_INCLUDED_
#define CT_ENABLE_IF_H_INCLUDED_

namespace ct
{

template <bool Cond, typename Tp = void>
struct enable_if
{ };

template <typename Tp>
struct enable_if<true, Tp>
{
    typedef Tp type;
};

} // ct

#endif // CT_ENABLE_IF_H_INCLUDED_
