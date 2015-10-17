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

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <utility>

#include "solver.h"

using std::cout;
using std::endl;
using std::make_pair;
using std::ofstream;
using std::queue;
using std::string;

const uint16_t kSquare = 1 + 3*(2 + 3*(1 + 3*(2 + 3*(1 + 3*(2 + 3*(1 + 3*2))))));

class Side::Iterator {
 public:
  // Normalized rotation.
  explicit Iterator(Side side)
    : normalized_(side),
      rotated_(normalized_.side_) {
    // This will end as long as all states were fed from the solved one.
    while(!can_cut()) {
      rotate_by_one_piece();
    }
  }

  // A specific rotation.
  explicit Iterator(uint16_t side)
    : normalized_(side),
      rotated_(side) {
    if(!can_cut())
      cout << side << endl;
    assert(can_cut());
  }

  Side side() const {
    return normalized_;
  }

  Iterator& operator++() {
    do {
      rotate_by_one_piece();
    } while (!can_cut());
    return *this;
  }

  bool operator==(const iterator& other) {
    // normalized_ == Side::normalize(rotated_), therefore no need to compare.
    return rotated_ == other.rotated_;
  }

  // Straight angle worth of least significant trits in natural order.
  uint16_t right() {
    uint16_t side = rotated_;
    uint8_t totalangle = 0;
    uint16_t answer = 0;
    uint16_t rightpower = 1;
    for (;;) {
      uint8_t piece = side % 3;
      assert(piece != 0);
      totalangle += piece;
      assert(totalangle <= 6);
      answer += piece * rightpower;
      if (totalangle == 6) return answer;
      rightpower *= 3;
      side /= 3;
      assert (side != 0);
    }
  }

  uint16_t rotated_value() const { return rotated_; };

 private:
  void rotate_by_one_piece() {
    uint8_t piece = rotated_ % 3;
    assert(piece != 0);
    rotated_ /= 3;
    rotated_ += piece * normalized_.power_;
  }

  bool can_cut() {
    uint16_t side = rotated_;
    uint8_t totalangle = 0;
    for (;;) {
      assert(side % 3 != 0);
      totalangle += side % 3;
      if (totalangle > 6) return false;
      if (totalangle == 6) return true;
      side /= 3;
      assert (side != 0);
    }
  }

  Side normalized_;
  uint16_t rotated_;
};

Side::Side(uint16_t side)
  : npieces_(number_of_pieces(side)),
    power_ (pow(3, npieces_-1)),
    side_(normalize(side)) {
  assert(is_fullcircle(side_));
}

bool Side::operator<(const Side& other) const {
  return side_ < other.side_;
}

uint16_t Side::value() {
  return side_;
}

// static
uint8_t Side::number_of_pieces(uint16_t side) {
  uint8_t answer = 0;
  while (side > 0) {
    assert(side % 3 != 0);
    side /= 3;
    ++answer;
  }
  return answer;
}

uint16_t Side::normalize(uint16_t side) {
  uint16_t min = side;
  uint16_t rotated = side;
  do {
    if (rotated < min) min = rotated;
    uint8_t piece = rotated % 3;
    assert(piece != 0);
    rotated /= 3;
    rotated += piece * power_;
  } while (rotated != side);
  return min;
}

// static
bool Side::is_fullcircle(uint16_t side) {
  uint8_t totalangle = 0;
  while (side > 0) {
    assert(side % 3 != 0);
    totalangle += side % 3;
    side /= 3;
  }
  return totalangle == 12;
}

// begin() == end(), because rotation is of finite order.
Side::iterator Side::begin() {
  return Iterator(*this);
}

class Shape::Iterator {
 public:
  Iterator()
    : top_(kSquare),
      bottom_(kSquare),
      rotated_top_(kSquare),
      rotated_bottom_(kSquare) {}

  Iterator(Side top, Side bottom)
    : top_(top),
      bottom_(bottom),
      rotated_top_(top),
      rotated_bottom_(bottom) {};

  Iterator(Side::iterator top, Side::iterator bottom)
    : top_(top.side()),
      bottom_(bottom.side()),
      rotated_top_(top),
      rotated_bottom_(bottom) {};

  Iterator& operator++() {
    ++rotated_top_;
    if (rotated_top_ == top_.begin()) {
      ++rotated_bottom_;
    }
    // No need to reset iterators because begin() == end().
    return *this;
  }

  Shape shape() const {
    return Shape(top_, bottom_);
  }

  Side::iterator rotated_top() const {
    return rotated_top_;
  }

  Side::iterator rotated_bottom() const {
    return rotated_bottom_;
  }

  bool operator==(const iterator& other) {
    return rotated_top_ == other.rotated_top_ && rotated_bottom_ ==
    other.rotated_bottom_;
  }

  bool operator!=(const iterator& other) {
    return !(*this == other);
  }

  // Twist right side of the toy, thus swapping top and bottom right halves.
  Iterator cut() {
    uint16_t top_right = rotated_top_.right();
    uint16_t top_left = rotated_top_.rotated_value() - top_right;
    while (top_left % 3 == 0) top_left /= 3;

    uint16_t bottom_right = rotated_bottom_.right();
    uint16_t bottom_left = rotated_bottom_.rotated_value() - bottom_right;
    while (bottom_left % 3 == 0) bottom_left /= 3;

    top_left *= pow(3, Side::number_of_pieces(bottom_right));
    bottom_left *= pow(3, Side::number_of_pieces(top_right));

    return Iterator(Side::iterator(top_left + bottom_right),
                    Side::iterator(bottom_left + top_right));
  }

 private:
  Side top_;
  Side bottom_;
  Side::iterator rotated_top_;
  Side::iterator rotated_bottom_;
};

Shape::Shape() : top_(kSquare), bottom_(kSquare) {}
Shape::Shape(const Side& top, const Side& bottom) : top_(top), bottom_(bottom) {};

// begin() == end(), because rotation is of finite order.
Shape::iterator Shape::begin() {
  return Iterator(top_, bottom_);
}

bool Shape::operator<(const Shape& other) const {
  if (top_ < other.top_) return true;
  if (other.top_ < top_) return false;
  return bottom_ < other.bottom_;
}

bool Shape::topheavy() const {
  return !(top_ < bottom_);
}

Solver::Solver() {}

void Solver::solve() {
  assert(solution_.empty());
  // Solved state is root of tree, at distance 0 from itself.
  Shape solved;
  solution_[solved] = make_pair(0, solved.begin());
  // Breadth first search with recording distance.
  queue<Shape> Q;
  Q.push(solved);
  while (!Q.empty()) {
    Shape state = Q.front();
    Q.pop();
    size_t distance = solution_[state].first;
    Shape::iterator rotated = state.begin();
    do {
      Shape::iterator new_rotation = rotated.cut();
      Shape new_state = new_rotation.shape();
      // In fact, the graph is not a tree, skip already visited states.
      if (new_state.topheavy() && solution_.count(new_state) == 0) {
        cout << "New state found at distance " << distance+1 << "." << endl;
        Q.push(new_state);
        solution_[new_state] = make_pair(distance+1, new_rotation);
      }
      ++rotated;
    } while (rotated != state.begin());
  }
}

size_t Solver::number_of_shapes() {
  return solution_.size();
}

void Solver::save_solution(const string& filename) {
  ofstream file;
  file.open(filename);
  for (const auto& value : solution_) {
    file << value.second.second.rotated_top().rotated_value() << " "
         << value.second.second.rotated_bottom().rotated_value() << endl;
  }
  file.close();
}

int main() {
  Solver solver;
  solver.solve();
  cout << "Total number of shapes: " << solver.number_of_shapes() << endl;
  solver.save_solution("solution.dat");
}
