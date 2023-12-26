#include <safe_conversion.hpp>

#include <stdexcept>

int SafelyConvert(size_t x)
{
  if (x > std::numeric_limits<int>::max())
  {
    throw std::logic_error("Cannot safely convert");
  }
  return static_cast<int>(x);
}

