#include <array>
#include <random>
#include <utility>
#include "gtest/gtest.h"
/* --------------------------------
  1.when function alloc memory, may throw exception.
  -------------------------------- */

// 1.test constexpr
constexpr unsigned fibonacci(unsigned i) {
  return ( i <= 1u ) ? i : (fibonacci(i - 1) + fibonacci(i - 2));
}
TEST(Miscellaneous, const_expr) {
  char char_arr[fibonacci(6)] {};  // fib(6) calc when compile
  std::cout << "compile output: " << sizeof(char_arr) << std::endl; // sizeof(x) calc when compile
  std::default_random_engine e;
  std::uniform_int_distribution<unsigned> d(1, 20);
  std::cout << "runtime output: " << fibonacci(d(e)) << std::endl;
  // std::array<unsigned, fibonacci(d(e)) test_arr; // error, runtime value can't used to init a std::array
}

// 2.test decltype
TEST(Miscellaneous, decltype) {
  int a[] = {1, 2, 3};
  decltype(*a) b = a[0]; // decltype(*a) return int&
  a[0] = 4;
  ASSERT_EQ(b, 4);
}
