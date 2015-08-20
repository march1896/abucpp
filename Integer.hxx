#ifndef __CORE_INTEGER_H__
#define __CORE_INTEGER_H__

#include "Class.hxx"

namespace abu { namespace Data {

template<int x>            struct toInteger   { static const int Value = x; };
template<int x>            struct I           { static const int Value = x; };

template<class A, class B> struct add                               { typedef Undefined      Eval; };
template<int A, int B>     struct add<toInteger<A>, toInteger<B>>   { typedef toInteger<A+B> Eval; };

template<class A, class B> struct minus                             { typedef Undefined      Eval; };
template<int A, int B>     struct minus<toInteger<A>, toInteger<B>> { typedef toInteger<A-B> Eval; };

template<class A, class B> struct mul                               { typedef Undefined      Eval; };
template<int A, int B>     struct mul<toInteger<A>, toInteger<B>>   { typedef toInteger<A*B> Eval; };

template<class A, class B> struct divid                             { typedef Undefined      Eval; };
template<int A, int B>     struct divid<toInteger<A>, toInteger<B>> { typedef toInteger<A/B> Eval; };

template<class T>          struct inc                               { typedef Undefined      Eval; };
template<int A>            struct inc<toInteger<A>>                 { typedef toInteger<A+1> Eval; };

template<class T>          struct dec                               { typedef Undefined      Eval; };
template<int A>            struct dec<toInteger<A>>                 { typedef toInteger<A-1> Eval; };

// class Eq
template<int A, int B>     struct equal<toInteger<A>, toInteger<B>> { typedef toBoolean<A==B> Eval; };

// class Ord
template<int A, int B>     struct less<toInteger<A>, toInteger<B>>    { typedef toBoolean<(A<B)> Eval; };
template<int A, int B>     struct greater<toInteger<A>, toInteger<B>> { typedef toBoolean<(A>B)> Eval; };

}; // namespace Data
}; // namespace abu
#endif // __CORE_INTEGER_H__
