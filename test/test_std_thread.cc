#include <thread>
#include <exception>
#include <iostream>
#include "gtest/gtest.h"

TEST(ttt, excep) {
  try {
    throw -1;
  } catch(int err_code){
    std::cout << "catch err: " << err_code << std::endl;
  } catch(std::bad_alloc alloc) {

  }
  std::cout << "finish Function" << std::endl;
}