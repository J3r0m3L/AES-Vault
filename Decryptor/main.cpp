#include <iostream>

int main() {
    int a {80};
    int b = a;
    
    std::cout << a << std::endl;
    std::cout << b << std:: endl;
    
    b = 70;
    
    std::cout << a << std::endl;
    std::cout << b << std:: endl;
    
    return 0;
}