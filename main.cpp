#include <iostream>
#include <glad/glad.h>

#include "Entity.hpp"
#include "Vec2.hpp"
#include "Mat4.hpp"

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;

    spry::Vec2<int> pos(1, 2);
    //spry::Entity<int> e{ pos };

    spry::Vec2 po2(2, 4);
    spry::Mat4<float> a;
    //spry::Entity g(po2);

    return 0;
}
