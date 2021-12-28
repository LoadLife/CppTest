#ifndef PIMPL_H_
#define PIMPL_H_
#include<memory>

class pImpl_unique_ptr {
public:
  pImpl_unique_ptr();
  pImpl_unique_ptr(const pImpl_unique_ptr& tmp);
  pImpl_unique_ptr& operator=(const pImpl_unique_ptr& tmp);

  pImpl_unique_ptr(pImpl_unique_ptr&& tmp);
  pImpl_unique_ptr& operator=(pImpl_unique_ptr&& tmp);
  void print();

  ~pImpl_unique_ptr();
private:
  struct element;
  std::unique_ptr<element> element_;
};

#endif