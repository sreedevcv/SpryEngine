#include <iostream>
#include <glad/glad.h>

#include "Entity.hpp"
#include "Vec2.hpp"

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;

    spry::Vec2<int> pos(1, 2);
    spry::Entity<int> e{ pos };

    return 0;
}
