#include <iostream>

using namespace std;

void f(int x){
    cout << sizeof(x) << " -> " << x << endl;
}

void f(string x){
    cout << sizeof(x) << " -> " << x << endl;
}

template <class ABC>
void s(ABC x){
    cout << "--- " << sizeof(x) << " -> " << x << endl;
}

int main(){
    std::cout << "Hello, World!" << std::endl;
    f(10);
    f("abc");

    s(5.5);
    s(10);
    s("qwerty");

    return 0;
}