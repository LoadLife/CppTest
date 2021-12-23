#include <deque>
#include <iostream>
#include <memory>
#include <type_traits>
#include <variant>
#include "gtest/gtest.h"
#include "stack.h"
#include "gtest/gtest.h"
using namespace std;

//----------------------Test Case------------------------------------
// stack<int> test 
TEST(T_Class, test_stack) {
  Stack<int> stack;
  int ele = 3;
  stack.push(3);
  int tmp;
  tmp = stack.top();
  stack.pop();
  ASSERT_EQ(ele, tmp);
}

// stack<string> specialization test
TEST(T_Class, test_stack_string) {
  try {
    Stack<string> stack;
    stack.pop();
  } catch(const exception& e) {
    cout << e.what() <<endl;
  }
}

/* strut member align test
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
  ASSERT_NE(sizeof(s_1), 13); 
  ASSERT_EQ(sizeof(s_1), 16);

  t_s<int, double, char> s_2;
  ASSERT_NE(sizeof(s_2), 13);
  ASSERT_EQ(sizeof(s_2), 24);
}

/* function template test  
  this is function template override, 
  function template local specialization properly looks like ->
  template<typename T1>
  int func<T1>(int, T1){
  } */
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
  ASSERT_EQ(test_override(a, b), 5.0);
  ASSERT_EQ(test_override(b, a), 3); 
}

// test Non_typed template parameters
// default non_typed template value
template<typename T1, int V1 = 1>
T1 test_default_value(T1 a, int b = V1) {
  a = a + static_cast<T1>(b);
  return a;
}
TEST(T_Function, non_typed_default_value) {
  double a = 2.0;
  ASSERT_EQ(test_default_value(a), 3.0);
}

/* Non_typed template paramters must be const integer num, enum or extern pointer,
   can't be float point now, samples below is not allowed */
/* -----------------------
  template <double D>
  void func(){
    cout <<　D << endl;
  }
  ------------------------ */ 
/* in msvc_2017 compile internal error
   extern `constexpr` char str[] = "hello world"; */
// if this is extern const char* str = "hello world" , can't be compiled[to be cleared]
extern const char str[] = "hello world"; 
template<const char* c_pointer>
const char* test_extern_pointer() {
  return c_pointer;
} 
TEST(T_Function, non_typed_extern_pointer) {
  ASSERT_EQ(test_extern_pointer<str>(),str);
}

/* template test decay 
  {"hello":char const[5] | "world!":char const[6]} they two are differnt types
   when non referenced type T, array will decay to a pointer */
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

// template's template param [not support in cpp98]
template<typename T, template<typename T2, typename T3 = std::allocator<T2>>
                     class CONTAINER>
void test_template_template_param() {
  CONTAINER<T> test_container;
  ASSERT_EQ(test_container.size(), 0);
}
TEST(T_Function, function_template_template_param) {
  test_template_template_param<int, std::vector>();
}

/* reference cuttle(引用折叠)
  X& &、[X& &&]、 X&& & → X&
  [x&& &&] -> x&& */
template <typename T1, typename T2 = int&>
bool test_cuttle(T1&& a) { 
  return std::is_same<T1, T2>::value;
}
TEST(T_Function, reference_cuttle) {
  int a = 3; int& b = a;
  auto ret = test_cuttle(b);
  ASSERT_EQ(ret, true);
}

// test std::visit
template<class... Ts> struct visitor : Ts... {using Ts::operator()...; };
template<class... Ts> visitor(Ts...) -> visitor<Ts...>;
using Var_Type = std::variant<int, double>;
TEST(template, visit) {
  int tmp = 0;
  Var_Type a = 10;
  std::visit(visitor{
    // need to accept all variant types
    [&tmp](int value) { tmp = value; },
    [](double value) { value = 3.14; } ,
  }
  , a);
  ASSERT_EQ(tmp, std::get<int>(a));
}

template<typename Container, typename Index>
auto return_ele(Container& c, Index i) 
  ->decltype(c[i])
{
  return c[i];
}
// test decltype
TEST(T_Function, decltype) {
  int a[] = {1, 2, 3};
  // decltype(*a) return int&
  decltype(*a) b = a[0]; 
  a[0] = 4;
  ASSERT_EQ(b, 4);
  ASSERT_EQ(&a[1], &return_ele(a, 1));

}

template<typename T>
void test_array_copy(T arg) {
  (void)arg;
  auto ret = std::is_same<T, const char*>();
  ASSERT_TRUE(ret);
}

template<typename T>
void test_array_ref(T& arg) {
  // T now is const char[len(arg)]
  (void)arg;
  auto ret = std::is_same<T, const char[12]>();
  ASSERT_TRUE(ret);
}
// test template array_args
TEST(T_Function, array_args){
  const char test_str[] = "hello world";
  test_array_copy(test_str);
  test_array_ref(test_str);
}

template<typename T>
void test_universal_ref(T&& ref) {
  std::cout << is_same<T, int&>() << std::endl;
}
TEST(T_Function, universal_ref_test) {
  int a = 4;
  int&& b = std::move(a);
  test_universal_ref(b);
}