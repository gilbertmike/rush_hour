#include "board.hpp"

#include <stdexcept>

#include "safe_conversion.hpp"

Board::Board(size_t size_x, size_t size_y) :
  size_x(size_x), size_y(size_y), num_squares(size_x*size_y),
  filled_squares_(num_squares, false)
{
}

const std::vector<Square>& Board::GetCarCoords() const
{
  return car_coords_;
}

void Board::ReplaceCarCoords(const std::vector<Square>& coords)
{
  car_coords_.clear();
  for (size_t i = 0; i < num_squares; ++i)
  {
    filled_squares_.at(i) = false;
  }
  for (size_t i = 0; i < coords.size(); ++i)
  {
    const auto& coord = coords.at(i);
    const auto& prop = car_props_.at(i);
    auto horizontal = prop.horizontal;
    auto length = prop.length;
    InsertCarCoord(coord, horizontal, length);
  }
}

const std::vector<CarProperty>& Board::GetCarProps() const
{
  return car_props_;
}

std::optional<size_t> Board::AddCar(
  size_t x,
  size_t y,
  bool horizontal,
  size_t length
)
{
  auto idx = car_props_.size();
  auto coord = Square{SafelyConvert(x), SafelyConvert(y)};

  // Check if car can be inserted
  if (horizontal)
  {
    for (auto coord_to_check = coord;
         coord_to_check.x < x + length;
         ++coord_to_check.x)
    {
      if (Collides(coord_to_check) || !InBoard(coord_to_check))
      {
        return std::nullopt;
      }
    }
  }
  else
  {
    for (auto coord_to_check = coord;
         coord_to_check.y < y + length;
         ++coord_to_check.y)
    {
      if (Collides(coord_to_check) || !InBoard(coord_to_check))
      {
        return std::nullopt;
      }
    }
  }

  // Below, commit to inserting car
  car_props_.emplace_back(CarProperty{horizontal, length});

  InsertCarCoord(coord, horizontal, length);

  return idx;
}

void Board::InsertCarCoord(const Square& coord, bool horizontal, size_t length)
{
  car_coords_.push_back(coord);
  
  if (horizontal)
  {
    for (auto coord_to_fill = coord;
         coord_to_fill.x < coord.x + length;
         ++coord_to_fill.x)
    {
      FillSquare(coord_to_fill);
    }
  }
  else
  {
    for (auto coord_to_fill = coord;
         coord_to_fill.y < coord.y + length;
         ++coord_to_fill.y)
    {
      FillSquare(coord_to_fill);
    }
  }
}

bool Board::MoveCar(size_t idx, bool positive)
{
  const auto& car_prop = car_props_.at(idx);
  const auto& horizontal = car_prop.horizontal;
  const auto& length = car_prop.length;

  auto& car_coord = car_coords_.at(idx);

  // Coord the tip of the car will fill
  auto back = car_coord;
  if (horizontal)
  {
    back.x += length-1;
  }
  else
  {
    back.y += length-1;
  }

  auto coord_to_fill = Square{};
  if (positive)
  {
    coord_to_fill = back;
    if (horizontal)
    {
      coord_to_fill.x += 1;
    }
    else
    {
      coord_to_fill.y += 1;
    }
  }
  else
  {
    coord_to_fill = car_coord;
    if (horizontal)
    {
      coord_to_fill.x -= 1;
    }
    else
    {
      coord_to_fill.y -= 1;
    }
  }

  // Remove current car before checking for collision
  if (horizontal)
  {
    for (auto coord = car_coord; coord.x < car_coord.x + length; ++coord.x)
    {
      UnfillSquare(coord);
    }
  }
  else
  {
    for (auto coord = car_coord; coord.y < car_coord.y + length; ++coord.y)
    {
      UnfillSquare(coord);
    }
  }

  // Only the tip needs to be checked for collision
  if (!InBoard(coord_to_fill) || Collides(coord_to_fill))
  {
    // Put car back in
    if (horizontal)
    {
      for (auto coord = car_coord; coord.x < car_coord.x + length; ++coord.x)
      {
        FillSquare(coord);
      }
    }
    else
    {
      for (auto coord = car_coord; coord.y < car_coord.y + length; ++coord.y)
      {
        FillSquare(coord);
      }
    }
    return false;
  }

  // Put car back in
  if (horizontal)
  {
    for (auto coord = car_coord; coord.x < car_coord.x + length; ++coord.x)
    {
      FillSquare(coord);
    }
  }
  else
  {
    for (auto coord = car_coord; coord.y < car_coord.y + length; ++coord.y)
    {
      FillSquare(coord);
    }
  }
  
  // Below, we commit to moving car
  // First, update collision detector
  if (positive)
  {
    UnfillSquare(car_coord);
  }
  else
  {
    UnfillSquare(back);
  }
  FillSquare(coord_to_fill);

  // Finally, car moves
  auto delta = -1;
  if (positive)
  {
    delta = 1;
  }
  if (horizontal)
  {
    car_coord.x += delta;
  }
  else
  {
    car_coord.y += delta;
  }

  return true;
}

bool Board::InBoard(const Square& coord) const
{
  if ((coord.x >= size_x) || (coord.x < 0))
  {
    return false;
  }
  else if ((coord.y >= size_y) || (coord.y < 0))
  {
    return false;
  }
  return true;
}

bool Board::Collides(const Square& coord) const
{
  return filled_squares_.at(GetRowMajorIdx(coord));
}

void Board::UnfillSquare(const Square& coord)
{
  filled_squares_.at(GetRowMajorIdx(coord)) = false;
}

void Board::FillSquare(const Square& coord)
{
  filled_squares_.at(GetRowMajorIdx(coord)) = true;
}

size_t Board::GetRowMajorIdx(const Square& coord) const
{
  if ((coord.x < 0) || (coord.x >= size_x)
      || (coord.y < 0) || (coord.y >= size_y))
  {
    throw std::logic_error("Invalid coordinate to convert");
  }

  return coord.x + size_x*coord.y;
}

