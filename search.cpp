#include "search.hpp"

#include <queue>

BoardState::BoardState(const std::vector<Square>& coords) :
  coords(coords)
{
}

/**
 * Lexicographic comparison of board states.
 */
struct BoardStateLexCompare
{
  bool operator()(const BoardState& lhs, const BoardState& rhs) const
  {
    for (size_t i = 0; i < lhs.coords.size(); ++ i)
    {
      if (lhs.coords.at(i).x < rhs.coords.at(i).x)
      {
        return true;
      }
      else if (lhs.coords.at(i).x > rhs.coords.at(i).x)
      {
        return false;
      }

      if (lhs.coords.at(i).y < rhs.coords.at(i).y)
      {
        return true;
      }
      else if (lhs.coords.at(i).y > rhs.coords.at(i).y)
      {
        return false;
      }
    }
    return false;
  }
};

bool IsWon(const BoardState& state)
{
  const auto& red_car_coord = state.coords.at(0);
  return (red_car_coord.x == 4) && (red_car_coord.y == 2);
}

Solution SearchSolution(Board&& board)
{
  auto first_state = BoardState{board.GetCarCoords()};
  auto visit_queue = std::queue<BoardState>({first_state});
  auto visited = std::map<BoardState, BoardState, BoardStateLexCompare>();
  visited.emplace(first_state, first_state);

  while (visit_queue.size() > 0)
  {
    auto& cur_state = visit_queue.front();
    if (IsWon(cur_state))
    {
      break;
    }

    board.ReplaceCarCoords(cur_state.coords);

    for (auto idx = 0; idx < cur_state.coords.size(); ++idx)
    {
      // Moving car forward is a possible next state
      auto moved = board.MoveCar(idx, true);
      if (moved)
      {
        auto state = BoardState{board.GetCarCoords()};
        if (!visited.contains(state))
        {
          visited.emplace(state, cur_state);
          visit_queue.push(state);
        }
        // Move car back to original state
        board.MoveCar(idx, false);
      }

      // Moving car backward is a possible next state
      moved = board.MoveCar(idx, false);
      if (moved)
      {
        auto state = BoardState{board.GetCarCoords()};
        if (!visited.contains(state))
        {
          visited.emplace(state, cur_state);
          visit_queue.push(state);
        }
        // Move car back to original state
        board.MoveCar(idx, true);
      }
    }

    visit_queue.pop();
  }

  std::vector<BoardState> moves;

  auto state = std::reference_wrapper(visit_queue.front());
  while (BoardStateLexCompare{}(state, first_state)
         || BoardStateLexCompare{}(first_state, state))
  {
    moves.push_back(state.get());
    state = visited.at(state);
  }

  auto solution = Solution{
    .moves = moves,
    .visited_states = visited.size()
  };

  return solution;
}

