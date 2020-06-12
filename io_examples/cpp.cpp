#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
    // The final solution should use std::cin, but you may want to read from
    // a file for testing.
    // auto input = std::ifstream("/some/file");
    auto& input = std::cin;

    int p, l;
    input >> p >> l;

    for (int i = 0; i < l; i++) {
        int a, b;
        input >> a >> b;

        // Save the connection (a, b).
    }

    // Run your algorithm.
    int solution = 4;

    // Print your solution.
    std::cout << solution << std::endl;
}
