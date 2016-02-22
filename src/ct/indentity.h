#ifndef CT_INDENTITY_H_INCLUDED_
#define CT_INDENTITY_H_INCLUDED_

namespace ct
{

template <typename Tp>
struct identity
{
    typedef Tp type;
};

} // ct

#endif // CT_INDENTITY_H_INCLUDED_
