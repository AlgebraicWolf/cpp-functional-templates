#ifndef LIST_HPP_
#define LIST_HPP_

#include <type_traits>
#include <cstdio>

struct ListBase {};

template <typename T>
concept List = std::is_base_of<ListBase, T>::value;

struct Nil : ListBase {};

template <typename x, List xs>
struct Cons : ListBase {};


template <template <typename> typename ValuePrinter, List xs>
struct PrintAuxillary {
  static void print() {};
};

template <template <typename> typename ValuePrinter>
struct PrintAuxillary<ValuePrinter, Nil> {
  static void print() {} // do nothing
};

template <template <typename> typename ValuePrinter, typename x>
struct PrintAuxillary<ValuePrinter, Cons<x, Nil>> {
  static void print() {
    ValuePrinter<x>::print(); // just print the value
  }
};

template <template <typename> typename ValuePrinter, typename x, List xs>
struct PrintAuxillary<ValuePrinter, Cons<x, xs>> {
  static void print() {
    ValuePrinter<x>::print(); // Print the value
    printf(", "); // Add separator
    PrintAuxillary<ValuePrinter, xs>::print(); // Recursively print the rest of the list
  }
};

template <template <typename> typename ValuePrinter, List xs>
struct Printer {
  static void print() {
    printf("[");
    PrintAuxillary<ValuePrinter, xs>::print();
    printf("]");
  }
};

// Function Fmap, turns list into functor (indeed, one can verify that functor laws hold)

template <template <typename> typename f, List xs>
struct Fmap {};

template <template <typename> typename f>
struct Fmap<f, Nil> {
  using result = Nil;
};

template <template <typename> typename f, typename x, List xs>
struct Fmap<f, Cons<x, xs>> {
  using result = Cons<typename f<x>::result, typename Fmap<f, xs>::result>;
};

// Concat
// template <List xs, List ys>
// struct Concat {};

// template <List ys>
// struct Concat<Nil, ys> {
//   using result = ys;
// };

// template <typename x, List xs, List ys>
// struct Concat<Cons<x, xs>, ys> {
//   using result = Cons<x, typename Concat<xs, ys>::result>;
// };

// Folds 

// Right fold: turns [a0, a1, a2, ..., an] -> f a0 (f a1 (f a2 (... f an init)))
template <template <typename, typename> typename f, typename init, List xs>
struct Foldr {};

template <template <typename, typename> typename f, typename init>
struct Foldr<f, init, Nil> {
  using result = init;
};

template <template <typename, typename> typename f, typename init, typename x, List xs>
struct Foldr<f, init, Cons<x, xs>> {
  using result = f<x, typename Foldr<f, init, xs>::result>::result;
};

// Left fold: turns [a0, a1, a2, ..., an] -> f (...f (f (init a0) a1) a2...) an

template <template <typename, typename> typename f, typename init, List xs>
struct Foldl {};

template <template <typename, typename> typename f, typename init>
struct Foldl<f, init, Nil> {
  using result = init;
};

template <template <typename, typename> typename f, typename init, typename x, List xs>
struct Foldl<f, init, Cons<x, xs>> {
  using result = Foldl<f, typename f<init, x>::result, xs>::result;
};

// Append -- Wrapping list constructor `Cons` into a function
template <typename x, List xs>
struct Append {
  using result = Cons<x, xs>;
};

// Concat
template <List xs, List ys>
struct Concat {
  using result = Foldr<Append, ys, xs>::result;
};

// Flatten : [[a]] -> [a] -- take list of lists and concatenate them together
template <List xs>
struct Flatten {
  using result = Foldr<Concat, Nil, xs>::result;
};

// Fmap using folds
template <template <typename> typename f, typename x, List xs>
struct ApplyAppend {
  using result = Cons<typename f<x>::result, xs>;
};

template <template <typename> typename f, List xs>
struct FmapFoldr {
  template <typename y, List ys>
  struct PartiallyApplied {
    using result = ApplyAppend<f, y, ys>::result;
  };

  using result = Foldr<PartiallyApplied, Nil, xs>::result;
};

// Monadic interface on list
// pure : a -> [a]
template <typename x> 
struct ListPure {
  using result = Cons<x, Nil>;
};

// bind : [a] -> (a -> [b]) -> [b]
// Basically, it is FlattenMap
template <List xs, template <typename> typename f> 
struct ListBind {
  using result = Flatten<typename Fmap<f, xs>::result>::result;
};

#endif