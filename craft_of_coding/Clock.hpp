#ifndef _CLOCK_HPP
#define _CLOCK_HPP

#include <iostream>
#include <iomanip>
#include <chrono>

class Clock {
protected:
  std::chrono::high_resolution_clock::time_point start_time;
public:
  Clock() {
    tick();
  }
  void tick() {
    start_time = std::chrono::high_resolution_clock::now();
  }
  auto tock() {
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
  }
  // Print elapsed time with newline
  void ptock() {
    auto elapsed = tock();
    std::cout << "Took " << elapsed << "ns ~ " << double(elapsed)/1e9 << "s" << std::endl;
  }
};

#endif
