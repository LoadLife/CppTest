#include <iostream>
#include "gtest/gtest.h"
#include "stack.h"
using namespace std;
void test_stack();

class Template_Test : public ::testing::Test{
private:

protected:
    virtual void SetUpTestCase();
    virtual void TearDownTestCase();
};

void Template_Test::SetUpTestCase() {

}
void Template_Test::TearDownTestCase(){

} 

void test_stack() {
  Stack<int> s;
  s.push(3);
  EXPECT_EQ(1,1);
  int b;
  s.pop(b);
  cout << "b: " << b <<endl;
  cout << "stack's size : " << s.size() << endl;
  cout << __FILE__<< endl;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
}