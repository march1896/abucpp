#ifndef __CORE_MAYBE_H__
#define __CORE_MAYBE_H__

#include "Class.hxx"

namespace abu { namespace Data {

template<class T> struct Just                { typedef T         Origin; };
                  struct Nothing             { typedef Undefined Origin; };

template<class T> struct isJust              { typedef Undefined Eval; };
template<class T> struct isJust<Just<T>>     { typedef True      Eval; };
template<>        struct isJust<Nothing>     { typedef False     Eval; };

template<class T> struct isNothing           { typedef Undefined Eval; };
template<class T> struct isNothing<Just<T>>  { typedef False     Eval; };
template<>        struct isNothing<Nothing>  { typedef True      Eval; };

template<class T> struct fromJust            { typedef Undefined Eval; };
template<class T> struct fromJust<Just<T>>   { typedef T         Eval; };
template<>        struct fromJust<Nothing>   { typedef Undefined Eval; };

// class Eq
template<>                 struct equal<Nothing, Nothing> { typedef True        Eval; };
template<class A>          struct equal<Just<A>, Nothing> { typedef False       Eval; }; 
template<class B>          struct equal<Nothing, Just<B>> { typedef False       Eval; }; 
template<class A, class B> struct equal<Just<A>, Just<B>> { typedef equal<A, B> Eval; };

// class Ordering
template<>                 struct less<Nothing, Nothing>     { typedef False    Eval; };
template<class A>          struct less<Nothing, Just<A>>     { typedef True     Eval; };
template<class A>          struct less<Just<A>, Nothing>     { typedef False    Eval; };
template<class A, class B> struct less<Just<A>, Just<B>>     { typedef False    Eval; };
template<>                 struct greater<Nothing, Nothing>  { typedef False    Eval; };
template<class A>          struct greater<Nothing, Just<A>>  { typedef False    Eval; };
template<class A>          struct greater<Just<A>, Nothing>  { typedef True     Eval; };
template<class A, class B> struct greater<Just<A>, Just<B>>  { typedef False    Eval; };

// class Functor
template<template<typename> class Func>           struct fmap<Func, Nothing> { typedef Nothing                      Eval; };
template<template<typename> class Func, class T>  struct fmap<Func, Just<T>> { typedef Just<typename Func<T>::Eval> Eval; };

// class Monad
template<class T>                                 struct maybe_lift          { typedef Just<T>                      Eval; };

template<template<typename> class Cont>           struct bind<Nothing, Cont> { typedef Nothing                      Eval; };
template<class T, template<typename> class Cont>  struct bind<Just<T>, Cont> { typedef Cont<T>                      Eval; };

}; // namespace Data
}; // namespace abu

#endif // __CORE_MAYBE_H__
