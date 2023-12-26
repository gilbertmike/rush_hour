#include <time.h>
#include <ncurses.h>

#include "search.hpp"

int main(int argc, char* argv[])
{
  const int ASCII_A = 65;

  auto board = Board(6, 6);

  board.AddCar(0, 2, true, 2);
  board.AddCar(0, 0, false, 2);
  board.AddCar(1, 0, true, 2);
  board.AddCar(3, 0, true, 3);
  board.AddCar(2, 1, true, 2);
  board.AddCar(4, 1, false, 2);
  board.AddCar(2, 2, false, 2);
  board.AddCar(0, 3, true, 2);
  board.AddCar(3, 3, true, 2);
  board.AddCar(5, 3, false, 3);
  board.AddCar(1, 4, true, 2);
  board.AddCar(3, 4, false, 2);
  board.AddCar(0, 5, true, 3);

  clock_t start = clock();
  auto solution = SearchSolution(std::move(board));
  clock_t end = clock();

  auto time = (double)(end - start)/CLOCKS_PER_SEC;

  initscr();
  noecho();
  refresh();

  printw("Found solution in %f s\n", time);
  printw("Explored %i states (%f states/s)\n",
         solution.visited_states,
         solution.visited_states/time);

  printw("Press any key to see solution\n");
  printw("Press CTRL+C to quit\n");
  printw("When viewing solution:\n");
  printw("  - Press F to move forward\n");
  printw("  - Press B to move backward\n");

  getch();

  const auto& car_props = board.GetCarProps();

  auto k = solution.moves.size()-1;
  while (true)
  {
    const auto& state = solution.moves.at(k);
    erase();
    for (auto i = 0; i < state.coords.size(); ++i)
    {
      const auto& coord = state.coords.at(i);
      bool horizontal = car_props.at(i).horizontal;
      auto length = car_props.at(i).length;

      auto x = coord.x;
      auto y = coord.y;
      if (horizontal)
      {
        for (auto j = 0; j < length; ++j)
        {
          mvaddch(y, x, i + ASCII_A);
          ++x;
        }
      }
      else
      {
        for (auto j = 0; j < length; ++j)
        {
          mvaddch(y, x, i + ASCII_A);
          ++y;
        }
      }
    }
    refresh();

    auto ch = getch();
    if ((ch == 'f') && (k >= 1))
    {
      --k;
    }
    else if ((ch == 'b') && (k <= solution.moves.size()-2))
    {
      ++k;
    }
  }

  endwin();

  return 0;
}
