#ifndef __CORE_BOOLEAN_H__
#define __CORE_BOOLEAN_H__

#include "Class.hxx"

namespace abu {
namespace Data {

// TODO: we should have another namespace called Boolean here, but the classes(Equal,
// Ordering, Functor) could not be partial specialized in different namespace.

struct Undefined;

template<bool x> struct toBoolean;
template<> struct toBoolean<true> {
    static bool const Value = true;
    // the lambda calculus of boolean
    template<class A, class B> struct Apply { typedef A Eval; };
};
template<> struct toBoolean<false> {
    static bool const Value = false;
    template<class A, class B> struct Apply { typedef B Eval; };
};
typedef toBoolean<true> True;
typedef toBoolean<false> False;

// class Equal
// template<class A, class B> using equal = abu::Class::equal<A, B>;
template<bool A, bool B>   struct equal<toBoolean<A>, toBoolean<B>>   { typedef toBoolean<A==B>  Eval; };

// class Ordering
template<bool A, bool B>   struct less<toBoolean<A>, toBoolean<B>>    { typedef toBoolean<(A<B)> Eval; };
template<bool A, bool B>   struct greater<toBoolean<A>, toBoolean<B>> { typedef toBoolean<(A>B)> Eval; };

// operations
template<class U, class V> struct And                                 { typedef Undefined Eval; };
template<bool A, bool B>   struct And<toBoolean<A>, toBoolean<B>>     { typedef toBoolean<A&&B>  Eval; };

template<class U, class V> struct Or                                  { typedef Undefined Eval; };
template<bool A, bool B>   struct Or<toBoolean<A>, toBoolean<B>>      { typedef toBoolean<A||B>  Eval; };

template<class A>          struct Not                                 { typedef Undefined Eval; };
template<bool A>           struct Not<toBoolean<A>>                   { typedef toBoolean<!A>  Eval; };
}; // namespace Data

}; // namespace abu

#endif // __CORE_BOOLEAN_H__
