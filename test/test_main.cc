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
---------------------------------------------------------------------- */

//----------------------Test Case------------------------------------
// stack_test 
TEST(Template_Test, test_stack) {
  Stack<int> stack;
  int ele = 3;
  stack.push(3);
  int tmp;
  tmp = stack.top();
  stack.pop();
  EXPECT_EQ(ele, tmp);
}

// strut mem memory align test
template<typename T1, typename T2, typename T3>
struct t_s {
  T1 a;
  T2 b;
  T3 c;
};
/*  default align mode
  1. 当前元素偏移量是当前元素大小的整数倍
  2. 整个struct的大小是最大元素的整数倍
  3. 当struct中有其它struct时候，内部struct的偏移量必须为其中最大元素大小的整数倍
*/
TEST(Align_Test, struct_test) {
  t_s<char, int, double> s_1;
  EXPECT_NE(sizeof(s_1), 13); 
  EXPECT_EQ(sizeof(s_1), 16);

  t_s<int, double, char> s_2;
  EXPECT_NE(sizeof(s_2), 13);
  EXPECT_EQ(sizeof(s_2), 24);
}

/* function template test : 
     this is function template override, 
     function template local specialization properly looks like ->

     template<typename T1>
     int func<int,T1>(int, T1){
     }
*/
template<typename T1, typename T2>
T1 func(T1 a, T2 b){
  a = b;
  return a; 
}
template<typename T1>
int func(int a, T1 b){
  a = static_cast<int>(b);
  return a;
}

TEST(Specialization, override){
  double a = 3.14;
  int b = 5;
  EXPECT_EQ(func(a, 3.15), 3.15);
  EXPECT_EQ(func(1, 2.5), 2); 
}

// -----------------main-----------------------
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}