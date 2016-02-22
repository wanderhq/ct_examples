#ifndef CT_INTEGRAL_CONSTANT_H_INCLUDED_
#define CT_INTEGRAL_CONSTANT_H_INCLUDED_

namespace ct
{

// integral_constant
template <typename Tp, Tp v>
struct integral_constant
{
    static  Tp const                   value = v;
    typedef Tp                         value_type;
    typedef integral_constant<Tp, v>   type;
};

template <typename Tp, Tp v>
Tp const integral_constant<Tp, v>::value;

typedef integral_constant<bool, true>   true_type;
typedef integral_constant<bool, false>  false_type;

} // ct

#endif // CT_INTEGRAL_CONSTANT_H_INCLUDED_
