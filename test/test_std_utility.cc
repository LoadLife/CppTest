#include <utility>
#include "gtest/gtest.h"
// 2.test std::move [utility]
TEST(Utility, move) {
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
