#include <iostream>

class Foo {
public:
    int val() const {
        return m;
    }

private:
    int m {};
}

int main() {
    std::cout << "Welcome to High Performance C++" << std::endl;
    return 0;
}