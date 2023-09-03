#ifndef HELLO_H
#define HELLO_H

#include <iostream>
#include <string>

class Hello {
 public:
  void sayHello();

 private:
  std::string s = "hello";
};

#endif  // HELLO_H