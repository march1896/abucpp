#ifndef __CORE_CLASS_H__
#define __CORE_CLASS_H__

#include "Control.hxx"

namespace abu { namespace Data {

template<bool> struct toBoolean;
typedef toBoolean<true>  True;
typedef toBoolean<false> False;
template<class> struct Not;

// class Equal
template<class A, class B> struct equal         { typedef Undefined Eval; };
template<class A, class B> struct unequal       { typedef typename Not<typename equal<A, B>::Eval>::Eval   Eval; };

// class Ord
template<class A, class B> struct less          { typedef Undefined Eval; };
template<class A, class B> struct greater       { typedef Undefined Eval; };
template<class A, class B> struct lessequal     { typedef typename Not<typename greater<A, B>::Eval>::Eval Eval; };
template<class A, class B> struct greaterequal  { typedef typename Not<typename less<A, B>::Eval>::Eval    Eval; };

enum Order { LT, EQ, GT };
template<Order T> struct toOrdering { static Order const Value = T; };
typedef toOrdering<LT> LessThan;
typedef toOrdering<EQ> EqualTo ;
typedef toOrdering<GT> GreaterThan;
template<class A, class B> struct compare { 
    typedef typename _If<typename less<A,B>::Eval, 
            LessThan, 
            typename _If<typename equal<A,B>::Eval, EqualTo, GreaterThan>::Eval>::Eval
                Eval;
};

// class Functor
template<template<typename> class Func, class F> struct fmap  { typedef Undefined Eval; };

// class Monad
// TODO: lift/return could not be defined, because C++ could not choose polymorphism by return value.
template<class Ma, template<typename> class Cont> struct bind { typedef Undefined Eval; };

}; // namespace Data
}; // namespace abu

#endif //  __CORE_CLASS_H__
