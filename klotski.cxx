/* Klotski solver. */
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

using std::array;
using std::cout;
using std::endl;
using std::map;
using std::set;
using std::vector;

class Solution {
 public:
  // State of the board, that is, position of blocks.
  // Each index corresponds to a cell on the board.
  // Each element is the label of the block that occupies that cell,
  // or ' ' if the cell is empty.
  using State = array<char,20>;

  // Generate |graph_|.
  void GenerateGraph(State initial);

 private:
  enum Direction {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
  };

  // Is the cell corresponding to |index| on the |direction| edge of the board,
  // that is, one of the |direction|-most cells.
  bool IsEdge(Direction direction, size_t index) {
    switch (direction) {
      case Direction::LEFT:
        return index%4 == 0;
      case Direction::RIGHT:
        return index%4 == 3;
      case Direction::TOP:
        return index < 4;
      case Direction::BOTTOM:
        return index > 15;
    }
    return true;
  }

  // Return the index corresponding to the cell that is adjacent to |index|
  // and is to |direction| from it.
  size_t Move(Direction direction, size_t index) {
    switch (direction) {
      case Direction::LEFT:
        return index - 1;
      case Direction::RIGHT:
        return index + 1;
      case Direction::TOP:
        return index - 4;
      case Direction::BOTTOM:
        return index + 4;
    }
    return 0;
  }

  // Pop an element of |states_to_explore_| and add it together with its edges
  // to |graph_|, and add all its not yet explored neighbors to
  // |states_to_explore_|.
  void AddNeighbors();

  // Undirected graph with states as vertices and valid moves as edges.
  // Every vertex is mapped to its set of neighbors.
  map<State, set<State>> graph_;

  set<State> states_to_explore_;
  set<char> labels_;
};

void Solution::GenerateGraph(State initial) {
  assert(graph_.empty());
  assert(states_to_explore_.empty());
  assert(labels_.empty());
  cout << "Generating graph..." << endl;
  for (char c : initial) {
    // ' ' is a special label denoting empty space.
    if (c != ' ') {
      labels_.insert(c);
    }
  }
  states_to_explore_.insert(initial);
  while (!states_to_explore_.empty())
    AddNeighbors();
  cout << graph_.size() << " vertices found." << endl;
}

void Solution::AddNeighbors() {
  set<State>::iterator explore_iterator = states_to_explore_.begin();
  State current = *explore_iterator;
  states_to_explore_.erase(explore_iterator);
  map<State, set<State>>::iterator graph_iterator;
  bool success;
  std::tie(graph_iterator, success) =
      graph_.insert(std::make_pair(current, set<State>({})));
  assert(success);
  // Cycle through blocks.
  for (char label : labels_) {
    vector<size_t> indices;
    for (size_t i = 0; i < current.size(); ++i) {
      if (current[i] == label)
        indices.push_back(i);
    }
    // Cycle through possible directions.
    for (Direction direction : {LEFT, RIGHT, TOP, BOTTOM}) {
      bool canmove = true;
      for (size_t i : indices) {
        if (IsEdge(direction, i)) {
          canmove = false;
          break;
        }
        char neighbor = current[Move(direction, i)];
        if (neighbor != ' ' && neighbor != label) {
          canmove = false;
          break;
        }
      }
      if (!canmove)
        continue;
      State newstate(current);
      for (size_t i : indices)
        newstate[i] = ' ';
      for (size_t i : indices)
        newstate[Move(direction, i)] = label;
      graph_iterator->second.insert(newstate);
      if (graph_.find(newstate) != graph_.end())
        continue;
      states_to_explore_.insert(newstate);
    }
  }
}

int main() {
  Solution solution;
  Solution::State initial({
      '0', '1', '1', '2',
      '0', '1', '1', '3',
      '4', '5', '6', '7',
      '4', '8', '6', '7',
      ' ', ' ', '9', '9'});
  solution.GenerateGraph(initial);
  return 0;
}
