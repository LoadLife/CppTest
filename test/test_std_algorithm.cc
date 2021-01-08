#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include "gtest/gtest.h"

// std::all_of
TEST(Algorithm, all_of) {
  std::array<int, 4> test_arr{1, 3, 5, 7};
  if (std::all_of(test_arr.begin(), test_arr.end(), [](int i){ return i%2; })) {
    std::cout << "all are odd number" << std::endl;
  }
}

// std::any_of
TEST(Algorithm, any_of) {
  std::array<int,4> test_arr{2, 4, 6, 8};
  if (!std::any_of(test_arr.begin(), test_arr.end(), [](int i){ return i%2; })) {
    std::cout << "all are even number" << std::endl;
  }

  test_arr[2] = 1;
  if (std::any_of(test_arr.begin(), test_arr.end(), [](int i){ return i%2; })) {
    std::cout << "odd number is existed" << std::endl;
  }
}

// std::none_of
TEST(Algorithm, none_of) {
  std::array<int,4> test_arr{2, 4, 6, 8};
  if (std::none_of(test_arr.begin(), test_arr.end(), [](int i){ return i%2; })) {
    std::cout << "all are even number" << std::endl;
  }

  test_arr[2] = 1;
  if (!std::none_of(test_arr.begin(), test_arr.end(), [](int i){ return i%2; })) {
    std::cout << "odd number is existed" << std::endl;
  }
}

// std::for_each
TEST(Algorithm, for_each) {
  std::vector<int> test_vec{1, 2, 3, 4};
  std::cout << "test_vec's elements: " << std::endl;
  std::for_each(test_vec.begin(), test_vec.end(), [](int i){ std::cout<< i << ", "; });
  std::cout << std::endl;
}

// std::find
TEST(Algorithm, find) {
  std::default_random_engine e;
  std::uniform_int_distribution<unsigned> distrib(1,20);
  std::vector<int> test_vec;
  for(auto i = 0 ;i != 30; i++){
    test_vec.push_back(distrib(e));
  }
  auto iter = std::find(test_vec.begin(), test_vec.end(), test_vec[20]);
  std::cout << "find *iter:" << *iter << std::endl;
  ASSERT_EQ(*iter, test_vec[iter - test_vec.begin()]);
  iter = std::find(test_vec.begin(), test_vec.end(), 21);
  ASSERT_EQ(iter, test_vec.end());
  std::cout << "can't find unexisted number" << std::endl;
}

// std::find_if
TEST(Algorithm, find_if) {
  std::default_random_engine e;
  std::uniform_int_distribution<unsigned> distrib(0, 100);
  std::vector<int> test_vec;
  for(auto i = 0; i != 100; i++) {
    test_vec.push_back(distrib(e));
  }
  e.seed(300);
  auto iter = std::find_if(test_vec.begin(), test_vec.end(), [](int i){ return i%2; });
  if(iter != test_vec.end()){
    std::cout << "find num : " << *iter << std::endl;
    ASSERT_EQ(*iter % 2, 1);
  }
}

// std::find_if_not
TEST(Algorithm, find_if_not) {
  std::default_random_engine e;
  std::uniform_int_distribution<unsigned> distrib(0, 100);
  std::vector<int> test_vec;
  for(auto i = 0; i != 100; i++) {
    test_vec.push_back(distrib(e));
  }
  e.seed(300);
  auto iter = std::find_if_not(test_vec.begin(), test_vec.end(), [](int i){ return i%2; });
  if(iter != test_vec.end()){
    std::cout << "find num : " << *iter << std::endl;
    ASSERT_EQ(*iter % 2, 0);
  }
}

// std::find_end
TEST(Algorithm, find_end) {
  std::vector src_vec{1, 2, 3, 6, 1, 2, 3, 2, 3, 4};
  std::array<int, 3> comp_array{1, 2, 3};
  auto iter = std::find_end(src_vec.begin(), src_vec.end(), comp_array.begin(), comp_array.end());
  ASSERT_EQ(iter - src_vec.begin(), 4);
  iter = std::find_end(src_vec.begin(), src_vec.end(), comp_array.begin(), comp_array.end(), [](int i, int j){ return --i == j; }); 
  ASSERT_EQ(iter - src_vec.begin(), 7);
}

// std::find_first_of
// [!] find the iter of src_vec that iter's value equal to compare_array's first value
TEST(Algorithm, find_first_of) {
  std::vector src_vec{1, 2, 4, 6, 1, 2, 3, 2, 3, 4};
  std::array<int, 3> comp_array{1, 2, 3};
  auto iter = std::find_first_of(src_vec.begin(), src_vec.end(), comp_array.begin(), comp_array.end());
  ASSERT_EQ(iter - src_vec.begin(), 0);
  iter = std::find_first_of(src_vec.begin(), src_vec.end(), comp_array.begin(), comp_array.end(), [](int i, int j){ return --i == j; }); 
  ASSERT_EQ(iter - src_vec.begin(), 1);
}

// std::adjacent_find
TEST(Algorithm, adjacent_find) {
  std::vector test_vec{1, 2, 3, 4, 4, 7, 9};
  auto iter = std::adjacent_find(test_vec.begin(), test_vec.end());
  ASSERT_EQ(iter - test_vec.begin(), 3);
  iter = std::adjacent_find(test_vec.begin(), test_vec.end(), [](int i, int j){ return i == j - 3; });
  ASSERT_EQ(iter - test_vec.begin(), 4);
}

// std::count && std::count_if
TEST(Algorithm, count_and_count_if) {
  std::vector test_vec{1, 1, 1, 2, 3, 3, 4, 5, 6};
  auto count = std::count(test_vec.begin(), test_vec.end(), 1);
  ASSERT_EQ(count, 3);
  count = std::count_if(test_vec.begin(), test_vec.end(), [](int i){ return i%2; });
  ASSERT_EQ(count, 6);
}

// std::mismatch's return: pair
TEST(Algorithm, mismatch_and_equal) { 
  std::vector<int> src {1, 2, 3, 4};
  std::vector<int> ref {1, 2, 4, 6};
  auto iter = std::mismatch(src.begin(), src.end(), ref.begin());
  ASSERT_EQ(iter.first - src.begin(), 2);
  ASSERT_EQ(iter.second - ref.begin(), 2);

  ASSERT_FALSE(std::equal(src.begin(), src.end(), ref.begin()));
  ASSERT_TRUE(std::equal(src.begin(), src.end(), ref.begin(), [](int i, int j){ return j - i <= 2; }));
}

// std::is_permutation : compare if the two containers' element are all same even they are in different order
TEST(Algorithm, is_permutation) {
  std::array<int, 4> src{1, 2, 3, 4};
  std::array<int, 4> ref{2, 1, 3, 4};
  ASSERT_TRUE(std::is_permutation(src.begin(), src.end(), ref.begin()));
  ASSERT_FALSE(std::is_permutation(src.begin(), src.end(), ref.begin(), [](int i, int j){ return i - j > 3; }));
}

// std::serach
TEST(Algorithm, serach) {
  std::array<int, 8> src{1, 2, 3, 4, 5, 6, 7, 8};
  std::array<int, 4> ref{3, 4, 5, 6};
  auto iter = std::search(src.begin(), src.end(), ref.begin(), ref.end());
  ASSERT_EQ(iter - src.begin(), 2);
  iter = std::search(src.begin(), src.end(), ref.begin(), ref.end(), [](int i, int j){ return j - i <= 2; }); 
  ASSERT_EQ(iter, src.begin());
}

// std::serach_n
TEST(Algorithm, serach_n) {
  std::array<int, 8> src{1, 2, 3, 4, 4, 6, 7, 8};
  auto iter = std::search_n(src.begin(), src.end(), 2, 4);
  ASSERT_EQ(iter - src.begin(), 3);
  iter = std::search_n(src.begin(), src.end(), 2, 4, [](int i, int j){ return j - i <= 3; });
  ASSERT_EQ(iter, src.begin());
}

