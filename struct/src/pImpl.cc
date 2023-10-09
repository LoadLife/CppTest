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

bool pImpl_unique_ptr::validate() {
  return true;
}

pImpl_unique_ptr::~pImpl_unique_ptr() = default;

//==================================================================================

struct pImpl_shared_ptr::element {
  int a {1};
  float b {1.0f};
};

pImpl_shared_ptr::pImpl_shared_ptr():
  element_(std::make_shared<element>())
{}

bool pImpl_shared_ptr::validate() {
  return true;
}


