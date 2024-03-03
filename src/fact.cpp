#include "fact.h"

/**
 * @brief Factorial numbers
 * 
 */
std::uint32_t factorial(std::uint32_t number) {
  return number <= 1 ? 1 : factorial(number - 1) * number;
}