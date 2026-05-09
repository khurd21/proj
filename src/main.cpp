#include "app.hpp"
#include "cli.hpp"

#include <exception>
#include <iostream>

int main(int argc, char **argv) {
  try {
    return proj::App{argc, argv}.run();
  } catch (const proj::CLIExit &ex) {
    if (ex.exit_code() == 0) {
      std::cout << ex.what();
    } else {
      std::cerr << ex.what();
    }
    return ex.exit_code();
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "Error: unknown exception\n";
    return 1;
  }
}
