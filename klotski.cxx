/* Klotski solver. */
#include <string>
#include <cassert>
#include <algorithm>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <queue>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::set;
using std::queue;
using std::map;

class Storage {
 public:
  const string* Store(string&& key) {
    set<string>::iterator hint = strings_.lower_bound(key);
    const string& answer = (*hint == key) ? *hint : *strings_.insert(hint, std::move(key));
    return &answer;
  }

 private:
  set<string> strings_;
};

class Solution {
 public:
  void GenerateGraph(const string& initial);

 private:
  bool IsLeft(uint8_t index);
  bool IsRight(uint8_t index);
  bool IsTop(uint8_t index);
  bool IsBottom(uint8_t index);
  uint8_t ToLeft(uint8_t index);
  uint8_t ToRight(uint8_t index);
  uint8_t ToTop(uint8_t index);
  uint8_t ToBottom(uint8_t index);

  void AddNeighbors();

  map<const string*, set<const string*>> graph_;
  queue<const string*> queue_;
  set<char> labels_;
  Storage storage_;
};

void Solution::GenerateGraph(const string& initial) {
  assert(graph_.empty());
  assert(queue_.empty());
  assert(labels_.empty());
  for (char c : initial) {
    // " " is a special label denoting empty space.
    if (c != ' ') {
      labels_.insert(c);
    }
  }
  cout << "Generating graph..." << endl;
  queue_.push(&initial);
  while (!queue_.empty())
    AddNeighbors();
  cout << graph_.size() << " vertices found." << endl;
}

bool Solution::IsLeft(uint8_t index) {
  return index%4 == 0;
}

bool Solution::IsRight(uint8_t index) {
  return index%4 == 3;
}

bool Solution::IsTop(uint8_t index) {
  return index < 4;
}

bool Solution::IsBottom(uint8_t index) {
  return index > 15;
}

uint8_t Solution::ToLeft(uint8_t index) {
  return index - 1;
}

uint8_t Solution::ToRight(uint8_t index) {
  return index + 1;
}

uint8_t Solution::ToTop(uint8_t index) {
  return index - 4;
}

uint8_t Solution::ToBottom(uint8_t index) {
  return index + 4;
}

void Solution::AddNeighbors() {
  const string* vertex = queue_.front();
  queue_.pop();
  for (char label : labels_) {
    vector<uint8_t> indices;
    for (uint8_t i = 0; i < vertex->size(); ++i) {
      if ((*vertex)[i] == label)
        indices.push_back(i);
    }
    // Try moving left.
    bool canmove = true;
    for (uint8_t i : indices) {
      if (IsLeft(i)) {
        canmove = false;
        break;
      }
      char neighbor = (*vertex)[ToLeft(i)];
      if (neighbor != ' ' && neighbor != label) {
        canmove = false;
        break;
      }
    }
    if (canmove)
      cout << label << " can be moved to the left.";
    // Try moving right.
    canmove = true;
    for (uint8_t i : indices) {
      if (IsRight(i)) {
        canmove = false;
        break;
      }
      char neighbor = (*vertex)[ToRight(i)];
      if (neighbor != ' ' && neighbor != label) {
        canmove = false;
        break;
      }
    }
    if (canmove) {
      string newvertex(*vertex);
      for (uint8_t i : indices)
        newvertex[i] = ' ';
      for (uint8_t i : indices)
        newvertex[ToRight(i)] = label;
      const string* newvertexptr = storage_.Store(std::move(newvertex));
      queue_.push(newvertexptr);
      // Push to graph_.
      // Avoid cycles.
    }
  }
}

int main() {
  Solution solution;
  solution.GenerateGraph("000000000000112 112 ");
  return 0;
}
