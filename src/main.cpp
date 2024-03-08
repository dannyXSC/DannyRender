#include <iostream>
#include <memory>
using namespace std;
class A
{
public:
    A() { cout << "A()" << endl; }
    A(int a) : A() { cout << "A(int a)" << endl; }
};

int main()
{
    A t1(1);
}
