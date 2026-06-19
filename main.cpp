#include "Tests.hpp"
#include "UI.hpp"

#include <cstring>
#include <iostream>

int main(int argc, char** argv) {
    try {
        RunAllTests();
    } catch (const std::exception& ex) {
        std::cerr << "Tests failed: " << ex.what() << std::endl;
        return 1;
    }

    if (argc > 1 && std::strcmp(argv[1], "--tests-only") == 0) {
        return 0;
    }

    RunUI();

    return 0;
}
