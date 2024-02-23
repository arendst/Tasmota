#include <stdio.h>

#include <timer.h>

class Base {
public:
  virtual void function1() {
    printf("Base says hello\n");
  };
  virtual void function2() {};
};

class D1: public Base {
public:
  virtual void function1() override {
    printf("D1 says hello\n");
  };
};

class D2: public Base {
public:
  virtual void function1() override {
    printf("D2 says hello\n");
  };
};

int main() {
  volatile uint8_t test_branch = 0;

  D1 d1class;
  D2 d2class;
  Base *pClass;
  while (1) {
    if (test_branch % 2) {
      pClass = &d1class;
      pClass->function1();
    } else {
      pClass = &d2class;
      pClass->function1();
    }
    test_branch++;

    delay_ms(1000);
  }

  return 0;
}
