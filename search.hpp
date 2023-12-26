#pragma once

#include "board.hpp"

struct BoardState
{
  const std::vector<Square> coords;

  BoardState(const std::vector<Square>& coords);
};


struct Solution
{
  std::vector<BoardState> moves;
  size_t visited_states;
};

Solution SearchSolution(Board&& board);
