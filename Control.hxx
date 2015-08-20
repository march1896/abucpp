#ifndef __CORE_CONTROL_H__
#define __CORE_CONTROL_H__

// Control Flow
namespace abu {

struct Undefined;

namespace Data {
    template<bool> struct toBoolean;
    typedef toBoolean<true>  True;
    typedef toBoolean<false> False;
    template<class> struct Not;
};
using namespace Data;

// if C then T else E
template<class C, class T, class E> struct If;
template<class T, class E>          struct If<True,  T, E> { typedef T Eval; };
template<class T, class E>          struct If<False, T, E> { typedef E Eval; };

// Apply a lambda calculus definition of boolean.
template<class C, class T, class E> struct _If { typedef typename C::template Apply<T, E>::Eval Eval; };

//
template<class ...Args> struct Cond;
template<class Test, class Statement, class ...Others> class Cond<Test, Statement, Others...> {
    typedef typename If<Test, Statement, Cond<Others...>>::Eval Eval;
};
template<class Default> class Cond<Default> { typedef Default Eval; };
template<>              class Cond<>        { typedef Undefined Eval; };

}; // namespace abu

#endif // __CORE_CONTROL_H__
