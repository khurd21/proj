#include "app.hpp"

#include <exception>
#include <iostream>

int main(int argc, char **argv) {
  try {
    return proj::App{argc, argv}.run();
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "Error: unknown exception\n";
    return 1;
  }
}
