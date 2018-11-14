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
  Test *ptr = new Test(1234);
  Test *old = ptr;

  ptr->foo();
  delete ptr;
  ptr->foo();
  ptr = new Test(4321);
  old->foo();
}