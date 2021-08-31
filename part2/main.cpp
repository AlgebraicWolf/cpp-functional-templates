#include "List.hpp"
#include "../part1/Nat.hpp"

template <Nat N>
struct NatPrinter {
  static void print() {
    printf("%u", NatToUnsignedInt<N>::value);
  }
};

template <Nat N>
struct Mul2 {
  using result = Mul<N, Succ<Succ<Z>>>::result;
};

template <Nat N>
struct Mul3 {
  using result = Mul<N, Succ<Succ<Succ<Z>>>>::result;
};

template <List xs>
struct Mul2Each {
  using result = Fmap<Mul2, xs>::result; 
};

template <List xs>
struct Mul3Each {
  using result = Fmap<Mul3, xs>::result; 
};

template <Nat n> 
struct kleisliExample {
  using result = Cons<typename Mul2<n>::result, Cons<typename Mul3<n>::result, Nil>>;
};

int main() {
  using l0 = Cons<Z, Cons<Succ<Z>, Cons<Succ<Succ<Z>>, Nil>>>;
  // using l1 = Cons<Succ<Succ<Succ<Z>>>, Cons<Succ<Succ<Succ<Succ<Z>>>>, Cons<Succ<Succ<Succ<Succ<Succ<Z>>>>>, Nil>>>;
  // using l1 = ListBind<l0, kleisliExample>::result;
  // using l2 = ListBind<l1, kleisliExample>::result;
  // using l3 = ListBind<l2, kleisliExample>::result; 

  Printer<NatPrinter, FmapFoldr<Mul2, l0>::result>::print();

  // Printer<NatPrinter, l0>::print();
  // printf("\n");
  // Printer<NatPrinter, l1>::print();
  // printf("\n");
  // Printer<NatPrinter, l2>::print();
  // printf("\n");
  // Printer<NatPrinter, l3>::print();
  // printf("\n");
}