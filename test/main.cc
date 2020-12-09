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
void Template_Test::TearDownTestCase() {

} 
TEST_F(Template_Test,test_stack) {
  Stack<int> stack;
  int ele = 3;
  stack.push(3);
  int tmp;
  s.pop(tmp);
  EXPECT_EQ(ele, tmp);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
}