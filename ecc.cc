/*******************************************************************
Copyright 2016 Bence Béky

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
*******************************************************************/

// Certicom ECC challange easiest exercise: ECCp-79.

/*
Example:
p=23, a=1, b=1:
(3, 10) + (9, 7) = (17, 20)
2(3, 10) = (7, 12)


======= ECCp-79 ======= 
p = 62CE 5177412A CA899CF5 
a = 39C9 5E6DDDB1 BC45733C 
b = 1F16 D880E89D 5A1C0ED1 
n = 62CE 5177407B 7258DC31 (order of P)
P_x = 315D 4B201C20 8475057D 
P_y = 035F 3DF5AB37 0252450A 
Q_x = 0679 834CEFB7 215DC365 
Q_y = 4084 BC50388C 4E6FDFAB 
solution = 1387 56822DD5 FB093766
*/

#include <iostream>

using std::cout;
using std::endl;

// This class can hold a large prime p, as well as elements of the finite field
// of order p.  In the latter case, values should be in [0, p), but this is not
// enforced.
class Number {
 public:
  Number() = default;
  explicit Number(int value) : value_(value) {}

  bool operator==(const Number& other) {
    return value_ == other.value_;
  }

  bool operator!=(const Number& other) {
    return value_ != other.value_;
  }

  bool operator>=(const Number& other) {
    return value_ >= other.value_;
  }

  static Number negate(Number a, Number p) {
    if (a.value_ == 0)
      return a;
    return Number(p.value_ - a.value_);
  }

  static Number add(Number a, Number b, Number p) {
    int sum = a.value_ + b.value_;
    if (sum >= p.value_) sum -= p.value_;
    return Number(sum);
  }

  static Number mod(Number a, Number b) {
    while (a.value_ < 0) {
      a.value_ += b.value_;
    }
    while (a >= b) {
      a.value_ -= b.value_;
    }
    return a;
  }

  static void divide(Number a, Number b, Number* quotient, Number* remainder) {
    int q = 0;
    while (a.value_ < 0) {
      a.value_ += b.value_;
      --q;
    }
    while (a >= b) {
      a.value_ -= b.value_;
      ++q;
    }
    *quotient = Number(q);
    *remainder = Number(a);
  }

  static Number multiply(Number a, Number b, Number p) {
    return mod(Number(a.value_ * b.value_), p);
  }

  static Number invert(Number b, Number p) {
    const Number one(1);
    if (b == one) return one;
    Number a;
    Number q;
    Number::divide(p, b, &q, &a);
    Number s0 = Number::negate(q, p);
    Number s1(one);
    Number s2;
    while (a != one) {
      Number::divide(b, a, &q, &b);
      s2 = s1;
      s1 = s0;
      s0 = mod(Number(s2.value_ - q.value_ * s1.value_), p);
      if (b == one) {
        return Number::mod(s0, p);
      }
      Number::divide(a, b, &q, &a);
      s2 = s1;
      s1 = s0;
      s0 = mod(Number(s2.value_ - q.value_ * s1.value_), p);
    }
    return Number::mod(s0, p);
  }

 private:
  int value_;
};

int main() {
  if (Number::add(Number(12), Number(20), Number(23)) == Number(9)) {
    cout << "PASS Addition." << endl;
  } else  {
    cout << "FAIL Addition." << endl;
  }
  if (Number::multiply(Number(8), Number(9), Number(23)) == Number(3)) {
    cout << "PASS Multiplication." << endl;
  } else  {
    cout << "FAIL Multiplication." << endl;
  }
  const Number p(997);
  const Number one(1);
  int pass(0);
  int fail(0);
  for (Number i(one); i != Number(0); i = Number::add(i, one, p)) {
    if (Number::multiply(Number::invert(i, p), i, p) == one) {
      ++pass;
    } else {
      ++fail;
    }
  }
  cout << "Inversion: " << pass << " correct results, " << fail << " errors." << endl;
  return 0;
}
