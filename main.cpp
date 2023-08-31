#include <iostream>
#include <zero_threadpool.h>

void func0()
{
    std::cout << "func0()" << std::endl;
}

void func1(int a)
{
    std::cout << "func1 int =" << a << std::endl;
}

void func2(int a, string b)
{
    cout << "func2() a=" << a << ", b=" << b<< endl;
}


void test1()
{
    ZERO_ThreadPool threadpool;
    threadpool.init(4);    
    threadpool.start();
    threadpool.exec(func1, 10);
    threadpool.exec(func2, 20, "gexn"); 
    threadpool.waitForAllDone(); 
    threadpool.stop();
}

int func1_future(int a)
{
    cout << "func1() a=" << a << endl;
    return a;
}

string func2_future(int a, string b)
{
    cout << "func1() a=" << a << ", b=" << b<< endl;
    return b;
}

void test2() 
{
    ZERO_ThreadPool threadpool;
    threadpool.init(1);
    threadpool.start(); 
    std::future<decltype (func1_future(0))> result1 = threadpool.exec(func1_future, 10);
    std::future<string> result2 = threadpool.exec(func2_future, 20, "gexn");

    std::cout << "result1: " << result1.get() << std::endl;
    std::cout << "result2: " << result2.get() << std::endl;
    threadpool.waitForAllDone();
    threadpool.stop();
}

class Test
{
    public:
        int test(int i)
        {
            cout << _name << ", i = " << i << endl;
            return i;
        }
        void setName(string name)
        {
            _name = name;
        }
        string _name;
};

void test3() 
{
    ZERO_ThreadPool threadpool;
    threadpool.init(1);
    threadpool.start(); 
    Test t1;
    Test t2;
    t1.setName("Test1");
    t2.setName("Test2");
    auto f1 = threadpool.exec(std::bind(&Test::test, &t1, std::placeholders::_1), 10);
    auto f2 = threadpool.exec(std::bind(&Test::test, &t2, std::placeholders::_1), 20);
    threadpool.waitForAllDone();
    cout << "t1 " << f1.get() << endl;
    cout << "t2 " << f2.get() << endl;
}

void func2_1(int a, int b)
{
    cout << "func2_1 a + b = " << a+b << endl;
}

int func2_1(string a, string b)
{
    cout << "func2_1 a + b = " << a << b<< endl;
    return 0;
}
void test4() 
{
    ZERO_ThreadPool threadpool;
    threadpool.init(1);
    threadpool.start();
    threadpool.exec((void(*)(int, int))func2_1, 10, 20);   
    threadpool.exec((int(*)(string, string))func2_1, "gexn", " gexianann");
    threadpool.waitForAllDone(); 
    threadpool.stop();
}

int main()
{
    test4();
    cout << "main finish!" << endl;
    return 0;
}

