#include <iostream>

// My C++ game development project

int main() {

  int health = 100;
  int damage = 25;

  health -= damage;

  std::cout << "Remaining health: " << health << '\n';

  return 0;
}
