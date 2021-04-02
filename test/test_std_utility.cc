#include <utility>
#include <array>
#include "gtest/gtest.h"

// test std::move [utility]
TEST(Utility, move) {
  // [sec1]: for the basic type, the move constructor's behaviour is unexplicit
  int a = 3;
  int b(std::move(a));
  ASSERT_NE(&a, &b);

  /* [sec2]: in this test, when rVal s_tmp is used in expresssion, it(s_tmp) will decay to a lVal(same to its source s_a), 
     so this sample call copy constructor */
  std::string s_a = "hello world";
  std::string&& s_tmp = std::move(s_a);
  std::string s_b(s_tmp);
  ASSERT_EQ(s_a, s_b); // just the string value's equal

  /* [sec3]: even our operation invoke the move constructor, s_d is not the symbol of previous s_c's memory yet. 
    s_d is another object just copy the s_c.data() pointor, and set the s_c.data() = nullptr,this action 
    occur in s_d's move constructor */
  std::string s_c = "hello world";
  std::string s_d(std::move(s_c));
  ASSERT_NE(&s_c, &s_d); 
  ASSERT_NE(s_c, s_d); // the string value's nonequal
  
  /* [sec4]: a unique_ptr sample */
  auto u_a = std::make_unique<int>(3);
  auto u_b(std::move(u_a)); 
  ASSERT_NE(&u_a, &u_b);
  ASSERT_EQ(u_a.get(), nullptr); 
}

// test std::swap<T&, T&> [Exchange values of two objects]
TEST(Utility, swap) {
  std::array<int, 4> src{1, 2, 3, 4};
  auto tmp_src = src;
  std::array<int, 4> dst{5, 6, 7, 8};
  auto tmp_dst = dst;
  std::swap(src, dst);
  ASSERT_EQ(src, tmp_dst);
  ASSERT_EQ(dst, tmp_src);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/* GTest Class Base
class Template_Test : public ::testing::Test {
protected:
    virtual void SetUp();
    virtual void TearDown();
};
void Template_Test::SetUp() {
}
void Template_Test::TearDown() {
} */