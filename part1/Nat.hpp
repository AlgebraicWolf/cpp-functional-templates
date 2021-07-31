#ifndef NAT_HPP_
#define NAT_HPP_

#include <type_traits>

// Объявляем тип...
struct NatBase {};
 
template <typename T>
concept Nat = std::is_base_of<NatBase, T>::value;
 
// ... и его конструкторы
struct Z : NatBase {}; // 0
 
template <Nat n>
struct Succ : NatBase {}; // S(n)

template<Nat a, Nat b>
struct Add {};
 
template<Nat a>
struct Add<a, Z> {
  using result = a;
};
 
template<Nat a, Nat b>
struct Add<a, Succ<b>> {
  using result = Succ<typename Add<a, b>::result>;
};

template<Nat n>
struct NatToUnsignedInt {};
 
template<>
struct NatToUnsignedInt<Z> {
  static constexpr unsigned int value = 0;
};
 
template <Nat n>
struct NatToUnsignedInt<Succ<n>> {
  static constexpr unsigned int value = 1 + NatToUnsignedInt<n>::value;
};

template <Nat a, Nat b>
struct Mul {};
 
template <Nat a>
struct Mul<a, Z> {
  using result = Z;
};
 
template <Nat a, Nat b>
struct Mul<a, Succ<b>> {
  using result = Add<typename Mul<a, b>::result, a>::result;
};

template <Nat n, Nat m>
struct Eq {
  static constexpr bool value = false; // By default numbers are not equal (this case really
                                       // triggers iff n = 0 or m = 0)
};
 
template <>
struct Eq<Z, Z> {
  static constexpr bool value = true; // Unless we are compairing two zeroes...
};
 
template <Nat n, Nat m>
struct Eq<Succ<n>, Succ<m>> {
  static constexpr bool value = Eq<n, m>::value; // For successors of n and m, they are equal
                                                 // iff n == m
};

template <Nat n, Nat m>
struct LE {};
 
template <Nat m>
struct LE<Z, m> {
  static constexpr bool value = true;
}; 
 
template <Nat n>
struct LE<Succ<n>, Z> {
  static constexpr bool value = false;
};
 
template <Nat n, Nat m>
struct LE<Succ<n>, Succ<m>> {
  static constexpr bool value = LE<n, m>::value;
};

#endif