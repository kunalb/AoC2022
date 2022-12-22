#include <iostream>
#include <list>
#include <utility>

class Demo {
public:
  int x;

  Demo(int x) : x(x) { std::cout << "Construct " << this << " "; }
  Demo(const Demo &other) : x(other.x) {
    std::cout << "Copy " << this << " " << &other << " ";
  }
  Demo(Demo &&other) : x(std::exchange(other.x, 0)) {
    std::cout << "Move " << this << " " << &other << " ";
  }
};

int main() {
  std::list<Demo> demo;

  std::cout << "emplace_back > ";
  demo.emplace_back(2);
  std::cout << std::endl;

  std::cout << "auto& val = emplace_back > ";
  auto &x = demo.emplace_back(3);
  std::cout << std::endl;

  std::cout << "auto val = emplace_back > ";
  auto y = demo.emplace_back(4);
  std::cout << std::endl;
}
