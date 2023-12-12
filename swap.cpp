#include <bits/stdc++.h>
using namespace std;
class A {
   protected:
    int m;
};
class B : public A {
   public:
    void print(B& test) { cout << test.m << endl; }
};

class rational {
   public:
    int num;
    rational(int a) : num(a) {}
    rational operator=(const rational& rhs) {
        this->num = rhs.num;
        return *this;
    }
};
rational operator*(const rational& lhs, const rational& rhs) { return rational(lhs.num * rhs.num); }
bool operator==(const rational& lhs, const rational& rhs) { return lhs.num == rhs.num; }

int func() {
    int val = 5;
    return val;
}

class TextBlock {
   private:
    string text;

   public:
    const char& operator[](std::size_t position) const { return text[position]; }
    char& operator[](std::size_t position) { return text[position]; }
};

void test(string& s) { cout << s << endl; }

int main() {
    // B b;
    // b.print(b);

    // int t = 42;
    // int&& i = (int&&)(t);
    // cout << i << endl;
    // cout << t << endl;
    // i++;
    // cout << i << endl;
    // cout << t << endl;

    // rational a(2);
    // rational b(3);
    // cout << (a * b).num << endl;
    // rational c(5);
    // (a * b) = c; //调用的是operator((a+b),c),因此可以调用，而不像func() = a一样

    // int a = 5;
    // func() = a;
    // int&& b = func();

    // test("abc");//使用隐式类型转换，转变成一个临时的string对象，但是临时对象是一个将亡值，属于右值，具有const属性，所以报错
    system("pause");
    return 0;
}