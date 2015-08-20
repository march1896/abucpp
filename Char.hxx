#ifndef __CORE_CHARACTER_H__
#define __CORE_CHARACTER_H__

namespace abu {
namespace Data {

template<char x>           struct toChar   { static const char Value = x; };

// class Eq
template<char A, char B>   struct equal<toChar<A>, toChar<B>>    { typedef toBoolean<A==B> Eval; };

// class Ord
template<char A, char B>   struct less<toChar<A>, toChar<B>>     { typedef toBoolean<(A<B)> Eval; };
template<char A, char B>   struct greater<toChar<A>, toChar<B>>  { typedef toBoolean<(A>B)> Eval; };

}
// namespace Data
}; // namespace abu
#endif // __CORE_CHARACTER_H__
