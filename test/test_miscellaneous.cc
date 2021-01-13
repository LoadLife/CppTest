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

// 2.test std::move [utility]
TEST(Miscellaneous, move) {
  // [sec1]: for the basic type, the move constructor's behaviour is unexplicit
  int a = 3;
  int b(std::move(a));
  EXPECT_NE(&a, &b);

  /* [sec2]: in this test, when rVal s_tmp is used in expresssion, it(s_tmp) will decay to a lVal(same to its source s_a), 
     so this sample call copy constructor */
  std::string s_a = "hello world";
  std::string&& s_tmp = std::move(s_a);
  std::string s_b(s_tmp);
  EXPECT_EQ(s_a, s_b); // just the string value's equal

  /* [sec3]: even our operation invoke the move constructor, s_d is not the symbol of previous s_c's memory yet. 
    s_d is another object just copy the s_c.data() pointor, and set the s_c.data() = nullptr,this action 
    occur in s_d's move constructor */
  std::string s_c = "hello world";
  std::string s_d(std::move(s_c));
  EXPECT_NE(&s_c, &s_d); 
  EXPECT_NE(s_c, s_d); // the string value's nonequal
  
  /* [sec4]: a unique_ptr sample */
  auto u_a = std::make_unique<int>(3);
  auto u_b(std::move(u_a)); 
  EXPECT_NE(&u_a, &u_b);
  EXPECT_EQ(u_a.get(), nullptr); 
}