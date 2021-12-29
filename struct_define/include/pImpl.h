#ifndef PIMPL_H_
#define PIMPL_H_
#include<memory>

class pImpl_unique_ptr {
public:
  pImpl_unique_ptr();
  // because of manually generate the deconstructor, form the 'rule of three', we should also
  // add copy && move operation
  pImpl_unique_ptr(const pImpl_unique_ptr& tmp);
  pImpl_unique_ptr& operator=(const pImpl_unique_ptr& tmp);

  pImpl_unique_ptr(pImpl_unique_ptr&& tmp);
  pImpl_unique_ptr& operator=(pImpl_unique_ptr&& tmp);
  bool validate();
  // due to struct element is a 'incomplete type' and element_ is a unique_ptr with a internal 
  // deleter(include a static assert prevent from 'incomplete type'), the implicit inline decoter
  // of pImpl invokes element_'s deleter will cause the static  assert fail, so use explicit decoter
  ~pImpl_unique_ptr();
private:
  struct element;
  std::unique_ptr<element> element_;
};

class pImpl_shared_ptr {
public:
  pImpl_shared_ptr();
  bool validate();
private:
  struct element;
  // because of shared_ptr has no internal deleter(deleter in control block)
  std::shared_ptr<element> element_;
};
#endif