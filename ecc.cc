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

#define ASSERT(a) ((a) ? cout : (cout << "Assertion fails in line " << __LINE__ << endl))

#include <iostream>
#include <limits>

using std::cout;
using std::endl;

// Large unsigned number.
class Number {
 public:
  Number() = default;
  Number(const Number&) = default;
  explicit Number(uint64_t value) : value_(value) {}

  bool operator==(const Number& other) {
    return value_ == other.value_;
  }

  bool operator!=(const Number& other) {
    return value_ != other.value_;
  }

  bool operator>(const Number& other) {
    return value_ > other.value_;
  }

  bool operator>=(const Number& other) {
    return value_ >= other.value_;
  }

  Number& operator<<=(int a) {
    ASSERT(a==1);
    value_ <<= a;
    return *this;
  }

  Number& operator>>=(int a) {
    ASSERT(a==1);
    value_ >>= a;
    return *this;
  }

  Number& operator+=(const Number& other) {
    ASSERT(std::numeric_limits<uint64_t>::max() - value_ >= other.value_);
    value_ += other.value_;
    return *this;
  }

  friend Number operator+(Number lhs, const Number& rhs) {
    lhs += rhs;
    return lhs;
  }

  Number& operator-=(const Number& other) {
    ASSERT(value_ >= other.value_);
    value_ -= other.value_;
    return *this;
  }

  friend Number operator-(Number lhs, const Number& rhs) {
    lhs -= rhs;
    return lhs;
  }

  // Warning: no overflow check.
  friend Number operator*(Number lhs, const Number& rhs) {
    lhs.value_ *= rhs.value_;
    return lhs;
  }

  static Number mod(Number a, Number b) {
    ASSERT(b.value_>0);
    Number b_shifted(b);
    while (a >= b_shifted)
      b_shifted <<= 1;
    while (b_shifted != b) {
      b_shifted >>= 1;
      if (a >= b_shifted)
        a -= b_shifted;
    }
    return a;
  }

  static void divide(Number a, Number b, Number* quotient, Number* remainder) {
    ASSERT(b.value_>0);
    uint64_t q = 0;
    uint64_t p = 1;
    Number b_shifted(b);
    while (a >= b_shifted) {
      p <<= 1;
      b_shifted <<= 1;
    }
    while (p != 1) {
      p >>= 1;
      b_shifted >>= 1;
      if (a >= b_shifted) {
        q += p;
        a -= b_shifted;
      }
    }
    *quotient = Number(q);
    *remainder = Number(a);
  }

 private:
  uint64_t value_;
};

// An element of the finite field of order p, where p is prime.  Represented by
// an integer value in [0, p).  Behavior is undefined for values outside this
// interval.  p is not stored to save space.
class Element {
 public:
  Element() = default;
  explicit Element(uint64_t value) : value_(value) {}
  explicit Element(Number value) : value_(value) {}

  bool operator==(const Element& other) {
    return value_ == other.value_;
  }

  bool operator!=(const Element& other) {
    return value_ != other.value_;
  }

  static Element negate(Element a, Number p) {
    if (a == Element(0))
      return a;
    return Element(p - a.value_);
  }

  static Element add(Element a, Element b, Number p) {
    Number sum = a.value_ + b.value_;
    if (sum >= p) sum -= p;
    return Element(sum);
  }

  static Element multiply(Element a, Element b, Number p) {
    return Element(Number::mod(a.value_ * b.value_, p));
  }

  static Element invert(Element i, Number p) {
    const Number one(1);
    if (i.value_ == one) return Element(one);
    Number a(i.value_);
    Number b;
    Number q;
    Number::divide(p, a, &q, &b);
    Element s0 = Element::negate(Element(q), p);
    Element s1(one);
    Element s2;
    while (b != one) {
      Number::divide(a, b, &q, &a);
      s2 = s1;
      s1 = s0;
      s0 = Element::add(
          s2, Element::negate(Element(Number::mod(q * s1.value_, p)), p), p);
      if (a == one) {
        return s0;
      }
      Number::divide(b, a, &q, &b);
      s2 = s1;
      s1 = s0;
      s0 = Element::add(
          s2, Element::negate(Element(Number::mod(q * s1.value_, p)), p), p);
    }
    return s0;
  }


 private:
  Number value_;
};

int main() {
  // Basic tests.
  ASSERT(Element::add(Element(12), Element(20), Number(23)) == Element(9));
  ASSERT(Element::multiply(Element(8), Element(9), Number(23)) == Element(3));
  ASSERT(Number::mod(Number(2), Number(5)) == Number(2));
  ASSERT(Number::mod(Number(7), Number(5)) == Number(2));
  ASSERT(Number::mod(Number(12), Number(5)) == Number(2));
  ASSERT(Number::mod(Number(17), Number(5)) == Number(2));
  Number q, r;
  ASSERT((Number::divide(Number(2), Number(5), &q, &r), q == Number(0) && r == Number(2)));
  ASSERT((Number::divide(Number(7), Number(5), &q, &r), q == Number(1) && r == Number(2)));
  ASSERT((Number::divide(Number(12), Number(5), &q, &r), q == Number(2) && r == Number(2)));
  ASSERT((Number::divide(Number(17), Number(5), &q, &r), q == Number(3) && r == Number(2)));

  //const Number p(29311);
  const Number p(100057);
  //const Number p(15485863);
  //const Number p(982451653);
  const Element one(1);
  uint64_t pass(0);
  uint64_t fail(0);
  for (Element i(one); i != Element(0); i = Element::add(i, one, p)) {
    if (Element::multiply(Element::invert(i, p), i, p) == one) {
      ++pass;
    } else {
      ++fail;
    }
  }
  cout << "Inversion: " << pass << " correct results, " << fail << " errors." << endl;
  return 0;
}
