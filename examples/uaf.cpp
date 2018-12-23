#include <iostream>

using namespace std;

class Test
{
  private:
    int a;
    

  public:
    Test(int num) { this->a = num; };
    ~Test() {};
    void foo(void);
};

void Test::foo(void)
{
  cout << this->a << endl;
}

int main()
{
  // alloc a new object
  Test *ptr = new Test(1234);
  Test *old = ptr;

  ptr->foo();

  // free the object
  delete ptr;
  // use after free
  ptr->foo();

  // old pointer point to the new object
  ptr = new Test(4321);
  old->foo();
}