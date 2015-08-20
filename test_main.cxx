#include <typeinfo>
#include <iostream>
#include <type_traits>

#include <cassert>

#include "Base.hxx"

using namespace abu;
using namespace abu::Data;

static char const * CompareString(Order ord) {
    switch (ord) {
        case LT: return "LessThan";
        case EQ: return "EqualTo";
        case GT: return "GreaterThan";
    }

    return NULL;
}

void testBool() {
    printf("\n****** test Boolean ********\n");
    // typename interact with value.
    static_assert(True::Value == true, "True::Value == true");
    static_assert(False::Value == false, "False::Value == false");

    // class Eq
    static_assert(equal<True, True>::Eval::Value, "True == True");
    static_assert(! equal<True, False>::Eval::Value, "True == False");
    static_assert(! equal<False, True>::Eval::Value, "False == True");
    static_assert(equal<False, False>::Eval::Value, "False == False");
    static_assert(! unequal<True, True>::Eval::Value, "True != True");
    static_assert(unequal<True, False>::Eval::Value, "True != False");
    static_assert(unequal<False, True>::Eval::Value, "False != True");
    static_assert(! unequal<False, False>::Eval::Value, "False != False\n");

    // class Ording
    static_assert(greater<True, False>::Eval::Value, "True > False");
    static_assert(! less<True, False>::Eval::Value, "True < False");
    static_assert(greaterequal<True, False>::Eval::Value, "True >= False");
    static_assert(! lessequal<True, False>::Eval::Value, "True <= False");

    // operators
    static_assert(! Not<True>::Eval::Value, "Not True");
    static_assert(Not<False>::Eval::Value, "Not False");

    static_assert(And<True, True>::Eval::Value, "True & True");
    static_assert(! And<True, False>::Eval::Value, "True & False");
    static_assert(! And<False, False>::Eval::Value, "False & True");
    static_assert(! And<False, False>::Eval::Value, "False & False");

    static_assert(Or<True, True>::Eval::Value, "True | True");
    static_assert(Or<True, False>::Eval::Value, "True | False");
    static_assert(Or<False, True>::Eval::Value, "False | True");
    static_assert(! Or<False, False>::Eval::Value, "False | False");
}

void testInteger() {
    printf("\n****** test Integer ********\n");
    // typename interact with value.
    typedef toInteger<0> _0;
    typedef toInteger<1> _1;
    typedef toInteger<2> _2;
    typedef toInteger<3> _3;
    typedef toInteger<4> _4;
    typedef toInteger<5> _5;

    static_assert(_3::Value == 3, "_3");
    static_assert(_4::Value == 4, "_4");

    // class Eq
    static_assert(equal<_0, _0>::Eval::Value, "_0 equals _0");
    static_assert(! equal<_0, _1>::Eval::Value, "_0 equals _1");
    static_assert(equal<toInteger<-1>, toInteger<-1>>::Eval::Value, "_-1 equals _-1 ");
    static_assert(! equal<toInteger<-1>, _1>::Eval::Value, "_-1 equals _1 ");
    static_assert(! unequal<_0, _0>::Eval::Value, "_0 unequal _0 ");
    static_assert(unequal<_0, _1>::Eval::Value, "_0 unequal _1 ");
    static_assert(! unequal<toInteger<-1>, toInteger<-1>>::Eval::Value, "_-1 unequal _-1 ");
    static_assert(unequal<toInteger<-1>, _1>::Eval::Value, "_-1 unequal _1");

    // class Ording
    static_assert(less<_0, _1>::Eval::Value, "_0 < _1");
    static_assert(! greater<_0, _1>::Eval::Value, "_0 > _1");
    static_assert(lessequal<toInteger<-1>, toInteger<-1>>::Eval::Value, "_-1 <= _-1");
    static_assert(greaterequal<_1, _1>::Eval::Value, "_1 >= _1");
    static_assert(compare<_3, _4>::Eval::Value == LT, "compare<_3, _4>");
    static_assert(compare<_3, _3>::Eval::Value == EQ, "compare<_3, _3>");
    static_assert(compare<_4, _3>::Eval::Value == GT, "compare<_4, _3>\n");

    // operators
    static_assert(inc<_0>::Eval::Value == 1, "_0 ++");
    static_assert(inc<inc<_0>::Eval>::Eval::Value == 2, "(_0 ++) ++");
    static_assert(dec<_0>::Eval::Value == -1, "_0 --");
    static_assert(dec<dec<_0>::Eval>::Eval::Value == -2, "(_0 --) --");
    static_assert(inc<dec<_0>::Eval>::Eval::Value == 0, "(_0 --) ++");
    static_assert(dec<inc<_0>::Eval>::Eval::Value == 0, "(_0 ++) --");

    static_assert(add<_3, _4>::Eval::Value == 7, "_3 + _4");
    static_assert(add<_2, add<_3, _4>::Eval>::Eval::Value == 9, "_2 + (_3 + _4)");
    static_assert(minus<_3, _4>::Eval::Value == -1, "_3 - _4");
    static_assert(minus<_2, minus<_3, _4>::Eval>::Eval::Value == 3, "_2 - (_3 - _4)");
    static_assert(mul<_3, _4>::Eval::Value == 12, "_3 * _4");
    static_assert(divid<_3, _4>::Eval::Value == 0, "_3 / _4");
    static_assert(divid<toInteger<8>, _4>::Eval::Value == 2, "_8 / _4");
}

void testList() {
    printf("\n****** test List ********\n");

    // helper definition.
    typedef toInteger<0> _0;
    typedef toInteger<1> _1;
    typedef toInteger<2> _2;
    typedef toInteger<3> _3;
    typedef toInteger<4> _4;
    typedef toInteger<5> _5;

    // test list construct
    typedef Cons<_5, Nihil> I5I;
    typedef Cons<_4, I5I> I45I;
    typedef Cons<_3, I45I> I345I;
    typedef Cons<_2, I345I> I2345I;
    typedef Cons<_1, I2345I> I12345I;

    typedef List<_1, _2, _3, _4, _5>::Eval L12345;
    typedef List<_1, _2, _3, _4>::Eval L1234;

    typedef range<toInteger<1>, toInteger<256>>::Eval I1_256I;

    auto node = toList<L12345>::pNode;
    int value = 1;
    while (node != nullptr) {
        assert(node->Element == value);
        node = node->Next; 
        value ++;
    }

    node = toList<I12345I>::pNode;
    value = 1;
    while (node != nullptr) {
        assert(node->Element == value);
        node = node->Next; 
        value ++;
    }

    node = toList<I1_256I>::pNode;
    value = 1;
    while (node != nullptr) {
        assert(node->Element == value);
        node = node->Next; 
        value ++;
    }

    // test class Eq
    static_assert(equal<I12345I, L12345>::Eval::Value, "I12345I equals to L12345");
    static_assert(! equal<I12345I, L1234>::Eval::Value, "I12345I equals to L1234");
    
    // test class Ordering
    static_assert(! less<I2345I, I12345I>::Eval::Value, "I2345I < I12345I");
    static_assert(less<I2345I, I345I>::Eval::Value, "I2345I < I345I");
    static_assert(greater<I2345I, I12345I>::Eval::Value, "I2345I > I12345I");
    static_assert(! greater<I2345I, I345I>::Eval::Value, "I2345I > I345I");

    static_assert(less<L1234, L12345>::Eval::Value, "L1234 < L12345");
    static_assert(! equal<L1234, L12345>::Eval::Value, "L1234 == L12345");
    static_assert(! greater<L1234, L12345>::Eval::Value, "L1234 > L12345");

    static_assert(head<I12345I>::Eval::Value == 1, "head of [12345] is 3");
    static_assert(last<I12345I>::Eval::Value == 5, "last of [12345] is 5");
    
    // test function.
    // length
    static_assert(length<Nihil>::Eval::Value == 0, "length of [] is : 0");
    static_assert(length<I5I>::Eval::Value == 1, "length of [5] is : 1");
    static_assert(length<I345I>::Eval::Value == 3, "length of [345] is : 3");
    static_assert(length<append<I345I, I12345I>::Eval>::Eval::Value == 8, "length of [345] ++ [12345] is : 8");

    // head, last
    static_assert(head<I12345I>::Eval::Value == 1, "head of [12345] is : 1");
    static_assert(last<I12345I>::Eval::Value == 5, "last of [12345] is : 5");

    // init, tail
    typedef init<I12345I>::Eval init_I12345I;
    typedef tail<I12345I>::Eval tail_I12345I;
    static_assert(length<init_I12345I>::Eval::Value == 4, "length of init_I12345I is 4");
    static_assert(head<init_I12345I>::Eval::Value == 1, "head of init_I12345I is 1");
    static_assert(last<init_I12345I>::Eval::Value == 4, "last of init_I12345I is 4");

    static_assert(length<tail_I12345I>::Eval::Value == 4, "length of tail_I12345I is 4");
    static_assert(head<tail_I12345I>::Eval::Value == 2, "head of tail_I12345I is 2");
    static_assert(last<tail_I12345I>::Eval::Value == 5, "last of tail_I12345I is 5");

    typedef reverse<I12345I>::Eval reverse_I12345I;
    node = toList<reverse_I12345I>::pNode;
    value = 5;
    while (node != nullptr) {
        assert(node->Element == value);
        node = node->Next; 
        value --;
    }

    typedef map<inc,I12345I>::Eval inc_I12345I;
    node = toList<inc_I12345I>::pNode;
    value = 2;
    while (node != nullptr) {
        assert(node->Element == value);
        node = node->Next; 
        value ++;
    }

    // foldl
    typedef foldl<add, toInteger<10>, I12345I>::Eval fladd10I12345I;
    static_assert(fladd10I12345I::Value == 25, "foldl add 10 [12345] is : 25");
    typedef foldl<minus, toInteger<1>, I12345I>::Eval flminus10I12345I;
    static_assert(flminus10I12345I::Value == -14, "foldl minus 1 [12345] is : (((((-1)-2)-3)-4)-5) = -14");

    // foldr
    typedef foldr<add, toInteger<10>, I12345I>::Eval fradd10I12345I;
    static_assert(fradd10I12345I::Value == 25, "foldr add 10 [12345] is : 25");
    typedef foldr<minus, toInteger<1>, I12345I>::Eval frminus10I12345I;
    static_assert(frminus10I12345I::Value == 2, "foldr minus 1 [12345] is : 1-(2-(3-(4-(-1)))) : 2");

    // toArray
    //auto Obj = toArray<I12345I>::Eval::pEle;
    //std::cout << "sizeof Obj is : " << sizeof(Obj) << std::endl;
    //for (auto i = 0; i < 5; i ++)
        //std::cout << Obj[i] << std::endl;
    //std::cout << typeid(Obj).name() << std::endl;

}

void testJust() {
    printf("\n****** test Just ********\n");
    typedef toInteger<1> _1;
    typedef toInteger<2> _2;
    typedef toInteger<3> _3;
    typedef toInteger<4> _4;
    typedef toInteger<5> _5;

    // test data constructor
    fromJust<Just<_1>>::Eval one;
    static_assert(one.Value == 1, "fromJust (1) is 1");

    typedef Just<_2> just_2;
    typedef fromJust<just_2>::Eval from_just_2;
    static_assert(from_just_2::Value == 2, "fromJust (2) is 2");

    // class Eq
    static_assert(equal<Nothing, Nothing>::Eval::Value, "Nothing == Nothing");
    static_assert(! equal<Nothing, just_2>::Eval::Value, "Nothing == just_2");

    // class Ordering
    static_assert(! less<Nothing, Nothing>::Eval::Value, "Nothing < Nothing");
    static_assert(less<Nothing, just_2>::Eval::Value, "Nothing < just_2");
    static_assert(! greater<Nothing, just_2>::Eval::Value, "Nothing > just_2");
    
    // test fmap
    typedef fmap<inc, just_2>::Eval fmap_inc_just_2;
    typedef fromJust<fmap_inc_just_2>::Eval fromJust_fmap_inc_just_2;
    static_assert(fromJust_fmap_inc_just_2::Value == 3, "fmap inc (2) is 3");

}

void testEither() {
}


int main() {
    testBool();
    testInteger();
    testList();
    testJust();
    return 0;
}
