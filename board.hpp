#pragma once

#include <map>
#include <optional>
#include <vector>

struct Square
{
  int x;
  int y;
};

struct CarProperty
{
  bool horizontal;
  size_t length;
};

struct Board
{
  const size_t size_x;
  const size_t size_y;
  const size_t num_squares;

  Board(size_t size_x, size_t size_y);

  const std::vector<Square>& GetCarCoords() const;
  void ReplaceCarCoords(const std::vector<Square>& coords);

  const std::vector<CarProperty>& GetCarProps() const;

  /**
   * Adds a car and returns the index of the car.
   */
  std::optional<size_t>
  AddCar(size_t x, size_t y, bool horizontal, size_t length);

  /**
   * Returns true if car moved, false if car cannot move.
   *
   * @param idx      Index of the car to move.
   * @param positive Whether the car moves forward or backwards.
   */
  bool MoveCar(size_t idx, bool positive);
 
 private:
  std::vector<CarProperty> car_props_;
  std::vector<Square> car_coords_;
  std::vector<bool> filled_squares_;

  void InsertCarCoord(const Square& coord, bool horizontal, size_t length);

  bool InBoard(const Square& coord) const;

  bool Collides(const Square& coord) const;

  void UnfillSquare(const Square& coord);
  void FillSquare(const Square& coord);

  size_t GetRowMajorIdx(const Square& coord) const;
};

