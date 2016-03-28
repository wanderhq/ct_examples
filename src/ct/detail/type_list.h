#ifndef CT_TYPE_LIST_DETAIL_H_INCLUDED
#define CT_TYPE_LIST_DETAIL_H_INCLUDED

namespace ct
{

#if __cplusplus >= 201103L // C++11

template <typename ...Args>
struct type_list;

template <typename ...Args>
struct make_list
{
    using type = type_list<Args...>;
};

typedef type_list<> null_list;

#else  // C++98

struct null_list
{};

template <typename TailT, typename HeadT = null_list>
struct type_list
{
    typedef HeadT head_type;
    typedef TailT tail_type;
};

template <
    typename Arg0 = null_list, typename Arg1 = null_list
  , typename Arg2 = null_list, typename Arg3 = null_list
  , typename Arg4 = null_list, typename Arg5 = null_list
  , typename Arg6 = null_list, typename Arg7 = null_list
  , typename Arg8 = null_list, typename Arg9 = null_list
>
struct make_list
{
private:
    typedef typename make_list<
        Arg1, Arg2, Arg3
      , Arg4, Arg5, Arg6
      , Arg7, Arg8, Arg9
    >::type tail_type;

public:
    typedef type_list<Arg0, tail_type> type;
};

template <>
struct make_list<>
{
    typedef null_list type;
};

#endif

} // namespace ct

#endif // CT_TYPE_LIST_DETAIL_H_INCLUDED
