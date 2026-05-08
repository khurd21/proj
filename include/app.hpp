#ifndef APP_HPP
#define APP_HPP

namespace proj {

class App {
public:
  App(int argc, char **argv);

  int run();

private:
  int m_argc{};
  char **m_argv{};
};

} // namespace proj

#endif // APP_HPP
