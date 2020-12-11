#include <iostream>
#include "gtest/gtest.h"
#include "stack.h"
using namespace std;
/*---------------------GTest Class Base-----------------------------
class Template_Test : public ::testing::Test {
protected:
    virtual void SetUp();
    virtual void TearDown();
};
void Template_Test::SetUp() {
}
void Template_Test::TearDown() {
}
*/

//----------------------Test Case------------------------------------
// 1.stack test 
TEST(Template_Test, test_stack) {
  Stack<int> stack;
  int ele = 3;
  stack.push(3);
  int tmp;
  tmp = stack.top();
  stack.pop();
  EXPECT_EQ(ele, tmp);
}

// 2.strut member align test
/* default align mode
  ①.当前元素偏移量是当前元素大小的整数倍
  ②.整个struct的大小是最大元素的整数倍
  ③.当struct中有其它struct时候，内部struct的偏移量必须为其中最大元素大小的整数倍
*/
template<typename T1, typename T2, typename T3>
struct t_s {
  T1 a;
  T2 b;
  T3 c;
};
TEST(Align_Test, struct_test) {
  t_s<char, int, double> s_1;
  EXPECT_NE(sizeof(s_1), 13); 
  EXPECT_EQ(sizeof(s_1), 16);

  t_s<int, double, char> s_2;
  EXPECT_NE(sizeof(s_2), 13);
  EXPECT_EQ(sizeof(s_2), 24);
}

// 3.function template test  
/* this is function template override, 
  function template local specialization properly looks like ->
  template<typename T1>
  int func<T1>(int, T1){
  }
*/
template<typename T1, typename T2>
T1 func(T1 a, T2 b) {
  a = b;
  return a; 
}
template<typename T1>
int func(int a, T1 b) {
  a = static_cast<int>(b);
  return a;
}
TEST(Specialization, override) {
  double a = 3.14;
  int b = 5;
  EXPECT_EQ(func(a, b), 5.0);
  EXPECT_EQ(func(b, a), 3); 
}

// 4.test Non_typed template parameters
// default non_typed template value
template<typename T1, int V1 = 1>
T1 func_v(T1 a, int b = V1) {
  a = a + static_cast<T1>(b);
  return a;
}
TEST(Non_typed, default_value) {
  double a = 2.0;
  EXPECT_EQ(func_v(a), 3.0);
}

// -----------------main-----------------------
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}