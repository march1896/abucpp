#ifndef __CORE_LIST_H__
#define __CORE_LIST_H__

#include "Class.hxx"
#include "Boolean.hxx"
#include "Integer.hxx" 
#include "Control.hxx"

namespace abu { namespace Data {

/// Exception declaraction.
struct Undefined;
struct TypeError;
struct NotListType;

/// List definition.
                           struct Nihil { typedef Undefined Eval; };
template<class E, class T> struct Cons  { typedef Undefined Eval; };

/// List construction.
template<class ...T>          struct List;
template<class H, class ...T> struct List<H, T...> { typedef Cons<H, typename List<T...>::Eval> Eval; };
template<>                    struct List<>        { typedef Nihil Eval; };

// The range is limited by the 'recursive template instantiation'
template<class S, class E> struct range       { typedef Cons<S, typename range<typename inc<S>::Eval, E>::Eval> Eval; };
template<class E>          struct range<E, E> { typedef Cons<E, Nihil> Eval; };

// The below definition does not work.
// The control flow requires lazy evaluation, which seems is not supported by templatei
// wiki: control structures of Lazy evaluation.(http://en.wikipedia.org/wiki/Lazy_evaluation)
// another example: http://aszt.inf.elte.hu/~gsd/halado_cpp/ch06s04.html(last line)
// TODO: is there away to do lazy evaluation in templates?
template<class S, class E> struct _range {
    typedef typename _If< typename greater<S, E>::Eval,
            Nihil,
            Cons<S, typename range<typename inc<S>::Eval, E>::Eval> >::Eval Eval;
};

// Class Eq
template<>        struct equal<Nihil, Nihil> { typedef True Eval; };
template<class A> struct equal<A, Nihil>     { typedef False Eval; };
template<class B> struct equal<Nihil, B>     { typedef False Eval; };
template<class AH, class AT, class BH, class BT> struct equal<Cons<AH, AT>, Cons<BH, BT>> { 
    typedef typename And<
        typename equal<AH, BH>::Eval, 
        typename equal<AT, BT>::Eval
            >::Eval Eval;
};

// Class Ording
template<>        struct less<Nihil, Nihil> { typedef False Eval; };
template<class A> struct less<Nihil, A>     { typedef True Eval; };
template<class A> struct less<A, Nihil>     { typedef False Eval; };
template<class AH, class AT, class BH, class BT> struct less<Cons<AH, AT>, Cons<BH, BT>> { 
    typedef typename If<typename less<AH, BH>::Eval,
            True,
            typename If<typename greater<AH, BH>::Eval, False, typename less<AT, BT>::Eval>::Eval
                >::Eval Eval;
};
template<>        struct greater<Nihil, Nihil> { typedef False Eval; };
template<class A> struct greater<Nihil, A>     { typedef False Eval; };
template<class A> struct greater<A, Nihil>     { typedef True Eval; };
template<class AH, class AT, class BH, class BT> struct greater<Cons<AH, AT>, Cons<BH, BT>> { 
    typedef typename If<typename greater<AH, BH>::Eval,
            True,
            typename If<typename less<AH, BH>::Eval, False, typename greater<AT, BT>::Eval>::Eval
                >::Eval Eval;
};


/// Basic functions.
// head :: [a] -> a
template<class L>          struct head                 { typedef NotListType Eval; };
template<>                 struct head<Nihil>          { typedef Undefined   Eval; };
template<class E, class T> struct head<Cons<E,T>>      { typedef E           Eval; };

// tail :: [a] -> a
template<class L>          struct tail                 { typedef NotListType Eval; };
template<>                 struct tail<Nihil>          { typedef Undefined   Eval; };
template<class E, class T> struct tail<Cons<E,T>>      { typedef T           Eval; };

// last :: [a] -> [a]
template<class L>          struct last                 { typedef NotListType Eval; };
template<>                 struct last<Nihil>          { typedef Undefined   Eval; };
template<class E>          struct last<Cons<E,Nihil>>  { typedef E           Eval; };
template<class E, class T> struct last<Cons<E,T>>      { typedef typename last<T>::Eval Eval; };

// init :: [a] -> [a]
template<class L>          struct init                 { typedef NotListType Eval; };
template<>                 struct init<Nihil>          { typedef Undefined   Eval; };
template<class E>          struct init<Cons<E,Nihil>>  { typedef Nihil       Eval; };
template<class E, class T> struct init<Cons<E,T>>      { typedef Cons<E, typename init<T>::Eval> Eval; };

// empty :: [a] -> Bool
template<class L>          struct empty                { typedef NotListType Eval; };
template<>                 struct empty<Nihil>         { typedef True        Eval; };
template<class E, class T> struct empty<Cons<E,T>>     { typedef False       Eval; };

// length :: [a] -> Int
template<class L>          struct length               { typedef NotListType  Eval; };
template<>                 struct length<Nihil>        { typedef toInteger<0> Eval; };
template<class E, class T> struct length<Cons<E,T>>    { typedef typename inc<typename length<T>::Eval>::Eval Eval; };

// (++) :: [a] -> [a] -> [a]
template<class A, class B> struct append               { typedef NotListType Eval; };
template<class B>          struct append<Nihil, B>     { typedef B           Eval; };
template<class HA, class TA, class LB> struct append<Cons<HA, TA>, LB> {
    typedef Cons<HA, typename append<TA, LB>::Eval> Eval;
};

/// List transformations.
// reverse :: [a] -> [a]
template<class L>          struct reverse              { typedef NotListType Eval; };
template<>                 struct reverse<Nihil>       { typedef Nihil       Eval; };
template<class H, class T> struct reverse<Cons<H,T>>   { 
    typedef Cons<H, T> L; 
    typedef Cons<typename last<L>::Eval, typename reverse<typename init<L>::Eval>::Eval> Eval; 
    //& typedef typename append<typename reverse<typename tail<T>::Eval>::Eval, Cons<H, Nihil>>::Eval Eval;
};

// map :: (a->b) -> [a] -> [b]
template<template<typename> class F, class L>             struct map                  { typedef TypeError Eval; };
template<template<typename> class F>                      struct map<F, Nihil>        { typedef Nihil     Eval; };
template<template<typename> class F, class HA, class TA>  struct map<F, Cons<HA, TA>> {
    typedef Cons<typename F<HA>::Eval, typename map<F, TA>::Eval> Eval; 
};

// intersperse :: a -> [a] -> [a]
template<class S, class L> struct intersperse                         { typedef TypeError      Eval; };
template<class S>          struct intersperse<S, Nihil>               { typedef Nihil          Eval; };
template<class S, class H> struct intersperse<S, Cons<H, Nihil>>      { typedef Cons<H, Nihil> Eval; };
template<class S, class H, class T> struct intersperse<S, Cons<H, T>> {
    typedef Cons<H, Cons<S, typename intersperse<S, T>::Eval>> Eval; 
};

// foldl :: (b -> a -> b) -> b -> [a] -> b
template<template<typename, typename> class F, class I, class L> struct foldl              { typedef Undefined Eval; };
template<template<typename, typename> class F, class I>          struct foldl<F, I, Nihil> { typedef I         Eval; };
template<template<typename, typename> class F, class I, class H, class T>
struct foldl<F, I, Cons<H, T>> {
    //template<class B, class L>          struct go                    { typedef Undefined Eval; };
    //template<class B>                   struct go<B, Nihil>          { typedef B         Eval; };
    //template<class B, class HL, class TL> struct go<B, Cons<HL, TL>> {
        //typedef typename go<typename F<B, HL>::Eval, TL>::Eval Eval;
    //};
    //typedef Cons<H, T> L;
    //typedef typename go<I, L>::Eval Eval;
    typedef typename foldl<F, typename F<I, H>::Eval, T>::Eval Eval;
};

// foldr :: (a -> b -> b) -> b -> [a] -> b
template<template<typename, typename> class F, class I, class L> struct foldr              { typedef Undefined Eval; };
template<template<typename, typename> class F, class I>          struct foldr<F, I, Nihil> { typedef I         Eval; };
template<template<typename, typename> class F, class I, class H, class T>
struct foldr<F, I, Cons<H, T>> {
    typedef typename F<H, typename foldr<F, I, T>::Eval>::Eval Eval;
};

/// List lambda to C++ value $$$ which makes me crazy.
template<class T> struct CListNode {
    T const Element;
    CListNode<T> const * Next;
    constexpr CListNode(T const &vl, CListNode<T> const * nt) : Element(vl), Next(nt) {};
};
template<class L> struct toList { typedef Undefined Eval; };
template<>        struct toList<Nihil> { static constexpr CListNode<void*> const * const pNode = nullptr; };
template<class H> struct toList<Cons<H, Nihil>> {
    typedef decltype(H::Value) eleType;
    static constexpr CListNode<eleType> const Node = CListNode<eleType>(H::Value, nullptr);
    static constexpr CListNode<eleType> const * const pNode = &Node;
};
template<class H> constexpr CListNode<decltype(H::Value)> const toList<Cons<H, Nihil>>::Node;

template<class H, class T> struct toList<Cons<H, T>> {
    typedef decltype(H::Value) eleType;
    static constexpr CListNode<eleType> const Node = CListNode<eleType>(H::Value, &toList<T>::Node);
    static constexpr CListNode<eleType> const * const pNode = &Node;
};
template<class H, class T> constexpr CListNode<decltype(H::Value)> const toList<Cons<H, T>>::Node;

// The List could take any thing, it's more like a scheme like then a haskell list.
// When the list consists different kinds of elements, it does not make sense to convert it to a C array.
// NOTE: during the implementation, it seems there is no way to do it totally at compile time.
#include <cstddef>
#include <type_traits>
template<class L> struct toArray {
    typedef decltype(head<L>::Eval::Value) EleType;

    static constexpr std::size_t const Length = length<L>::Eval::Value;
    static constexpr EleType const Addr[Length] = {};

    //struct Constructor {
        //constexpr Constructor() {
            //Assign(toList<L>::pNode, Addr);
        //}
        //static constexpr bool Assign(CListNode<EleType> const * node, EleType const * ele) {
            //*(EleType *)(ele) = node->Element;
            //return node == nullptr ? true : Assign(node->Next, ele+1); 
        //}
        //static constexpr EleType* const Head = Addr;
    //};

    template<EleType* m>                   struct _Nihil { static constexpr EleType const * const pEle = (EleType const *)m; };
    template<class H, class T>             struct _Cons;
    template<class H, EleType* m>          struct _Cons<H, _Nihil<m>>  { 
        static EleType const * const pEle = (EleType const*)((char*)m - sizeof(H::Value)); };
    template<class H, class TH, class TT>  struct _Cons<H, _Cons<TH, TT>> { 
        static EleType const * const pEle = (EleType const*)((char*)_Cons<TH, TT>::pEle - sizeof(H::Value)); };

    template<class X, EleType* m>          struct _copy                     {};
    template<class A, EleType* m>          struct _copy<Cons<A, Nihil>, m>  { typedef _Cons<A, _Nihil<m>> Eval; };
    template<class A, class B, EleType* m> struct _copy<Cons<A, B>, m>      { typedef _Cons<A, typename _copy<B, m>::Eval> Eval; };

    template<class X>                      struct _Nil                      { typedef Undefined Eval; };
    template<class A, EleType* m>          struct _Nil<_Cons<A, _Nihil<m>>> { typedef _Nihil<m> Eval; };
    template<class A, class B>             struct _Nil<_Cons<A, B>>         { typedef typename _Nil<B>::Eval Eval; };

    template<class X>             struct Assigner { };
    template<class A, EleType* m> struct Assigner<_Cons<A, _Nihil<m>>> {
        Assigner() { *_Cons<A, _Nihil<m>>::pEle = A::Value; };
    };
    template<class A, class B>    struct Assigner<_Cons<A, B>> {
        Assigner() {
            *_Cons<A, B>::pEle = A::Value;
            Assigner<_Cons<A, B>>(); 
        };
    };

    typedef typename _copy<L, Addr>::Eval _List;

    typedef Assigner<_List> RuntimeEvaluate;
    // static Assigner<_List> Value = Assigner<_List>();

    typedef typename _Nil<_List>::Eval Eval;

    // static constexpr Constructor Eval = Constructor();
};
template<class L> std::size_t const toArray<L>::Length;
template<class L> typename toArray<L>::EleType const toArray<L>::Addr[toArray<L>::Length];
/*
template<class T> struct CArrayLinker {
    T * pElement;
    CArrayLinker<T> const * Next;
    constexpr CArrayLinker(T const * pE, T const &rV, CArrayLinker<T> const *nt) : pElement(pE), Next(nt) {
        // std::remove_const<T const*>(pElement)[0] = rV;
    };
};
template<class L, class EleType, EleType* Addr> struct toArrayCons                       { static constexpr CArrayLinker<EleType> const * const pNode = nullptr; }; // TODO
template<class H, class EleType, EleType* Addr> struct toArrayCons<Cons<H, Nihil>, EleType, Addr> {
    static constexpr CArrayLinker<EleType> const Node = CArrayLinker<EleType>(Addr, H::Value, nullptr);
    static constexpr CArrayLinker<EleType> const * const pNode = &Node;
};
template<class H, class EleType, EleType* Addr> 
constexpr CArrayLinker<EleType> const toArrayCons<Cons<H, Nihil>, EleType, Addr>::Node;

template<class H, class T, class EleType, EleType* Addr> struct toArrayCons<Cons<H,T>, EleType, Addr> {
    static constexpr EleType const * const NextAddr = Addr + 1;
    static constexpr CArrayLinker<EleType> const Node = CArrayLinker<EleType>(Addr, H::Value, toArrayCons<T, EleType, NextAddr>::pNode);
    static constexpr CArrayLinker<EleType> const * const pNode = &Node;
};
template<class H, class T, class EleType, EleType* Addr> 
constexpr CArrayLinker<EleType> const toArrayCons<Cons<H,T>, EleType, Addr>::Node;

template<class L>          struct toArray { typedef Undefined Eval; };
template<>                 struct toArray<Nihil> { static constexpr void * Value = 0; };
template<class H, class T> struct toArray<Cons<H, T>> {
    typedef Cons<H, T> theList;
    typedef decltype(H::Value) EleType;

    static constexpr std::size_t const Length = length<theList>::Eval::Value;
    static constexpr EleType const Addr[Length] = {};

    // static constexpr toArrayCons<theList, EleType, Content> Array = ;
    static constexpr CArrayLinker<EleType> const Node = CArrayLinker<EleType>(Addr, H::Value, toArrayCons<theList, EleType, Addr>::pNode);
    static constexpr CArrayLinker<EleType> const * const pNode = &Node;
};
template<class H, class T> constexpr std::size_t const toArray<Cons<H, T>>::Length;
template<class H, class T> constexpr decltype(H::Value) const toArray<Cons<H, T>>::Addr[toArray<Cons<H, T>>::Length];
template<class H, class T> constexpr CArrayLinker<decltype(H::Value)> const toArray<Cons<H, T>>::Node;
*/

}; // namespace Data
}; // namespace abu
#endif // __CORE_LIST_H__
