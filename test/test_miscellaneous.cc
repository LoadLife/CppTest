#include <array>
#include <random>
#include <utility>
#include "gtest/gtest.h"
/* ------------------summary--------------
 1.when function alloc memory, may throw exception.
 ----------------------------------------- */

// 1.test constexpr
constexpr unsigned fibonacci(unsigned i) {
  return ( i <= 1u ) ? i : (fibonacci(i - 1) + fibonacci(i - 2));
}
TEST(Miscellaneous, const_expr) {
  // fib(6) calc when compile
  char char_arr[fibonacci(6)] {};
  // sizeof(x) calc when compile
  std::cout << "compile output: " << sizeof(char_arr) << std::endl; 
  std::default_random_engine e;
  std::uniform_int_distribution<unsigned> d(1, 20);
  std::cout << "runtime output: " << fibonacci(d(e)) << std::endl;
  // error, runtime value can't used to init a std::array
  // std::array<unsigned, fibonacci(d(e)) test_arr; 
}

// 2.test decltype
TEST(Miscellaneous, decltype) {
  int a[] = {1, 2, 3};
  // decltype(*a) return int&
  decltype(*a) b = a[0]; 
  a[0] = 4;
  ASSERT_EQ(b, 4);
}

uint64_t calc_align(uint64_t n, uint64_t align) {
  return ((n + align -1) & (~(align - 1)));
}
// TEST_ALIGN
TEST(Miscellaneous, align) {
  std::cout << calc_align(17,4) << std::endl;
}