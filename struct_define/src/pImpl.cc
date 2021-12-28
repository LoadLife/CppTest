#include "pImpl.h"
#include <iostream>

struct pImpl_unique_ptr::element {
  int a {1};
  float b {1.0f};
};

pImpl_unique_ptr::pImpl_unique_ptr():
  element_(std::make_unique<element>()) 
{ }  

pImpl_unique_ptr::pImpl_unique_ptr(const pImpl_unique_ptr& tmp):
  element_(std::make_unique<element>(*tmp.element_))
{}

pImpl_unique_ptr& pImpl_unique_ptr::operator=(const pImpl_unique_ptr& tmp) {
  *element_ = *tmp.element_;
  return *this;
}



pImpl_unique_ptr::pImpl_unique_ptr(pImpl_unique_ptr&& tmp) = default;

pImpl_unique_ptr& pImpl_unique_ptr::operator=(pImpl_unique_ptr&& tmp) = default;

void pImpl_unique_ptr::print() {
  std::cout << element_->a << std::endl;
  std::cout << element_->b << std::endl;
}

pImpl_unique_ptr::~pImpl_unique_ptr() = default;