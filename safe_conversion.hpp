#pragma once

#include <cstddef>
#include <limits>

/**
 * Converts a size_t value to int if possible, throws if conversion will
 * overvlow
 */
int SafelyConvert(size_t x);

