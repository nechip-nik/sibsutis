#include <iostream>

int foo(int x);
int bar(int x);
class test
    {
    public:
        test(){
            std::cout << "Hello constructor" ;
        };
        ~test()
        {
            std::cout << "Hello destructor" ;
        };
    };
int main(int argc, char** argv){
    std::cout << foo(argc);
}

int foo(int x){
    int y;

    try
    {
        y = bar(x);
    }
    catch(int e)
    {
        y = e;
    }
    return y;
    
}
int bar(int x)
{
    test a, b;
    
    
    if (x==1)
    {
        throw 1;
    }
    if (x== 2)
    {
        throw 3.14;
    }
    return x;
}