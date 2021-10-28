#include "include/ThreadPool.h"
#include <iostream>
#include <random>

using namespace std;

random_device rd;
mt19937 mt(rd());
uniform_int_distribution<int> dist(-500,500);

auto rnd = bind(dist,mt);
class A {    //函数必须是 static 的才能使用线程池
public:
    int Afun(int n = 0) {
		std::cout << n << "  hello, Afun !  " << std::this_thread::get_id() << std::endl;
		return n;
	}

	static std::string Bfun(int n, std::string str, char c) {
		std::cout << n << "  hello, Bfun !  "<< str.c_str() <<"  " << (int)c <<"  " << std::this_thread::get_id() << std::endl;
		return str;
	}
};
//通过睡眠模拟并行
void mult_sleep(){
    this_thread::sleep_for(chrono::milliseconds(2000 + rnd()));
}
void nopara(){
    cout << "help" << endl;
}

void multiply(int a, int b){
    mult_sleep();
    int res = a*b;
    cout << "multipy" << res << endl;
}

void multiply_output(int & out, const int a, const int b) {
    mult_sleep();
    out = a * b;
    cout << a << " * " << b << " = " << out << std::endl;
}

int multiply_return(int a, int b){
    mult_sleep();
    int res = a*b;
    cout << "mutiply_return" << res << endl;
    return res;
}


int main(int argc, char *argv[]){
    // int a = 0;
    ThreadPool pool(5);
    pool.start();

    for (int i = 1; i < 3; ++i) {
        for (int j = 1; j < 4; ++j) {
            pool.submit(multiply, i, j);
        }
    }
    typedef std::function<void()> Functor;
    A a;
    pool.submit(nopara);
    int xx = 4;
    std::future<std::string> gh = pool.submit(A::Bfun, 9998,"mult args", 123);
    Functor functor(std::bind(&A::Afun,&a,5));//终于解决了提交普通成员函数的问题
    pool.submit(functor);
    // Submit function with output parameter passed by ref
    int output_ref;
    auto future1 = pool.submit(multiply_output, std::ref(output_ref), 5, 6);

    // Wait for multiplication output to finish
    future1.get();
    std::cout << "Last operation result is equals to " << output_ref << std::endl;

    // Submit function with return parameter 
    auto future2 = pool.submit(multiply_return, 5, 3);

    // Wait for multiplication output to finish
    int res = future2.get();
    std::cout << "Last operation result is equals to " << res << std::endl;
    
    pool.stop();

    return 0;
}