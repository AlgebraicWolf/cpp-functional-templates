#include <cstdio>
#include "Nat.hpp"

int main() {
  using three = Succ<Succ<Succ<Z>>>;
  using six = Succ<Succ<Succ<three>>>;

  printf("%u + %u = %u\n", NatToUnsignedInt<three>::value, NatToUnsignedInt<six>::value, NatToUnsignedInt<typename Add<three, six>::result>::value);
  printf("%u * %u = %u\n", NatToUnsignedInt<three>::value, NatToUnsignedInt<six>::value, NatToUnsignedInt<typename Mul<three, six>::result>::value);

  printf("3 + 6 == 6 + 3 -- %s\n", Eq<Add<three, six>::result, Add<six, three>::result>::value ? "correct" : "incorrect");
  printf("3 == 6 -- %s\n", Eq<three, six>::value ? "correct" : "incorrect");
  printf("3 <= 6 -- %s\n", LE<three, six>::value ? "correct" : "incorrect");
  printf("6 <= 3 -- %s\n", LE<six, three>::value ? "correct" : "incorrect");
}