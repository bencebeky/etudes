/*
Copyright 2015 Bence Béky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Solve the shape of the Square 1 puzzle: make it a cube starting from
// any valid shape.

#include <map>
#include <utility>

// Side is the shape of a side, modulo rotation.
// Side::Iterator is a side in a specific orientation.
class Side {
 public:
  Side(uint16_t side);

  class Iterator;
  using iterator = class Iterator;

  iterator begin();

  bool operator<(const Side& other) const;

  uint16_t value();
  static uint8_t number_of_pieces(uint16_t side);

 private:
  uint16_t normalize(uint16_t side);
  static bool is_fullcircle(uint16_t side);

  // Maximum 10 pieces on a side.
  uint8_t npieces_;

  // Maximum power factor is 3^9 = 19683, this fits in uint16.
  uint16_t power_;

  // A side is stored as a sequence of 1's and 2's, 1's representing small
  // wedges, 2's large ones.
  // Maximum side value is 2211111111 base 3 = 55768, this fits in uint16.
  uint16_t side_;
};

// Shape is the shape of the toy, modulo rotation of top and bottom sides.
// Shape::Iterator is the shape of the toy in a specific orientation of top and
// bottom sides.
class Shape {
 public:
  Shape();
  Shape(const Side& top, const Side& bottom);

  class Iterator;
  using iterator = class Iterator;

  iterator begin();

  bool operator<(const Shape& other) const;

 private:
  // |top_| is top view, |bottom_| is bottom view.
  Side top_;
  Side bottom_;
};

class Solver {
 public:
  Solver();

  void solve();
  size_t number_of_shapes();

 private:
  using SolutionType = std::map<Shape, std::pair<size_t, Shape>>;
  SolutionType solution_;
};
