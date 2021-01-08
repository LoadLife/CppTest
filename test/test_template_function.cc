#include <iostream>
#include <deque>
#include <memory>
#include "gtest/gtest.h"
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

/* 1.strut member align test
  default align mode
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
TEST(T_Function, align_struct_test) {
  t_s<char, int, double> s_1;
  EXPECT_NE(sizeof(s_1), 13); 
  EXPECT_EQ(sizeof(s_1), 16);

  t_s<int, double, char> s_2;
  EXPECT_NE(sizeof(s_2), 13);
  EXPECT_EQ(sizeof(s_2), 24);
}

/* 2.function template test  
  this is function template override, 
  function template local specialization properly looks like ->
  template<typename T1>
  int func<T1>(int, T1){
  }
*/
template<typename T1, typename T2>
T1 test_override(T1 a, T2 b) {
  a = b;
  return a; 
}
template<typename T1>
int test_override(int a, T1 b) {
  a = static_cast<int>(b);
  return a;
}
TEST(T_Function, specialization_override) {
  double a = 3.14;
  int b = 5;
  EXPECT_EQ(test_override(a, b), 5.0);
  EXPECT_EQ(test_override(b, a), 3); 
}

// 3.test Non_typed template parameters
// default non_typed template value
template<typename T1, int V1 = 1>
T1 test_default_value(T1 a, int b = V1) {
  a = a + static_cast<T1>(b);
  return a;
}
TEST(T_Function, non_typed_default_value) {
  double a = 2.0;
  EXPECT_EQ(test_default_value(a), 3.0);
}

/* 4.Non_typed template paramters must be const integer num, enum or extern pointer,
   can't be float point now, samples below is not allowed
/*-------------------------------------------------------------
  template <double D>
  void func(){
    cout <<　D << endl;
  }
---------------------------------------------------------------*/
// in msvc_2017 compile internal error
// extern constexpr char str[] = "hello world";  
extern const char str[] = "hello world";
template<const char* c_pointer>
const char* test_extern_pointer() {
  return c_pointer;
} 
TEST(T_Function, non_typed_extern_pointer) {
  EXPECT_EQ(test_extern_pointer<str>(),str);
}

/* 5.template test decay 
  {"hello":char const[5] | "world!":char const[6]} they two are differnt types
   when non referenced type T, array will decay to a pointer
*/
template<typename T>
T const& test_max_ref(T const& a, T const& b) {
  return a > b ? a : b;
}
template<typename T>
T test_max(T a, T b) {
  return a > b ? a : b;
}
TEST(T_Function, array_decay_to_pointer) {
  const char a[] = "hello";
  const char b[] = "world!";
  // test_max_ref(a, b);  compile error
  ASSERT_EQ(test_max(a, b), b);
}

// 6.template's template param [not support in cpp98]
template<typename T, template<typename T2, typename T3 = std::allocator<T2>>
                     class CONTAINER>
void test_template_template_param() {
  CONTAINER<T> test_container;
  ASSERT_EQ(test_container.size(),0);
}
TEST(T_Function,function_template_template_param){
  test_template_template_param<int,std::vector>();
}