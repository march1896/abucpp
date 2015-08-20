#ifndef __CORE_EITHER_H__
#define __CORE_EITHER_H__

#include "Class.hxx"

namespace abu { namespace Data {

/*
 * data  Either a b  =  Left a | Right b
 *   deriving (Eq, Ord, Read, Show, Typeable)
 */

template<class A> struct Left               { typedef A         Origin; };
template<class B> struct Right              { typedef B         Origin; };

template<class A> struct isLeft             { typedef Undefined Eval; };
template<class X> struct isLeft<Left<X>>    { typedef True      Eval; };
template<class X> struct isLeft<Right<X>>   { typedef False     Eval; };

template<class A> struct isRight            { typedef Undefined Eval; };
template<class X> struct isRight<Left<X>>   { typedef False     Eval; };
template<class X> struct isRight<Right<X>>  { typedef True      Eval; };

// class Eq
template<class A, class B> struct equal<Left<A>, Left<B>>                    { typedef equal<A,B> Eval; };
template<class A, class B> struct equal<Right<A>, Right<B>>                  { typedef equal<A,B> Eval; };

// class Functor
template<template<typename> class Func, class A> struct fmap<Func, Left<A>>  { typedef Left<typename Func<A>::Eval>  Eval; };
template<template<typename> class Func, class B> struct fmap<Func, Right<B>> { typedef Right<typename Func<B>::Eval> Eval; };

// class Monad
template<class T>                                 struct either_lift         { typedef Right<T>                      Eval; };
template<class T, template<typename> class Cont>  struct bind<Left<T>, Cont> { typedef Left<T>                       Eval; };
template<class T, template<typename> class Cont>  struct bind<Right<T>, Cont>{ typedef Cont<T>                       Eval; };

}; // namespace Data
}; // namepsace abu

#endif // __CORE_EITHER_H__
