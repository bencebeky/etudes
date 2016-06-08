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

using std::cout;
using std::endl;

// Large unsigned number.
class Number {
 public:
  Number() = default;
  Number(const Number&) = default;
  Number(uint64_t value) : value_(value) {}

  static Number Infinity() {
    return Number(0x8000000000000000);
  }

  bool is_infinity() const {
    return value_ == 0x8000000000000000;
  }

  bool operator==(const Number& other) const {
    return value_ == other.value_;
  }

  bool operator!=(const Number& other) const {
    return value_ != other.value_;
  }

  bool operator>(const Number& other) const {
    ASSERT(!is_infinity());
    ASSERT(!other.is_infinity());
    return value_ > other.value_;
  }

  bool operator<(const Number& other) const {
    ASSERT(!is_infinity());
    ASSERT(!other.is_infinity());
    return value_ < other.value_;
  }

  bool operator>=(const Number& other) const {
    ASSERT(!is_infinity());
    ASSERT(!other.is_infinity());
    return value_ >= other.value_;
  }

  Number& operator<<=(int a) {
    ASSERT(!is_infinity());
    ASSERT(a==1);
    value_ <<= a;
    return *this;
  }

  Number& operator>>=(int a) {
    ASSERT(!is_infinity());
    ASSERT(a==1);
    value_ >>= a;
    return *this;
  }

  Number& operator+=(const Number& other) {
    ASSERT(!is_infinity());
    ASSERT(!other.is_infinity());
    ASSERT(0x7fffffffffffffff - value_ >= other.value_);
    value_ += other.value_;
    return *this;
  }

  friend Number operator+(Number lhs, const Number& rhs) {
    lhs += rhs;
    return lhs;
  }

  Number& operator-=(const Number& other) {
    ASSERT(!is_infinity());
    ASSERT(!other.is_infinity());
    ASSERT(value_ >= other.value_);
    value_ -= other.value_;
    return *this;
  }

  friend Number operator-(Number lhs, const Number& rhs) {
    lhs -= rhs;
    return lhs;
  }

  static Number multiply(Number a, Number b, Number p) {
    ASSERT(!a.is_infinity());
    ASSERT(!b.is_infinity());
    ASSERT(!p.is_infinity());
    ASSERT(a < p);
    ASSERT(b < p);
    return mod(a.value_ * b.value_, p);
  }


  // Warning: no overflow check.
  friend Number operator*(Number lhs, const Number& rhs) {
    ASSERT(!lhs.is_infinity());
    ASSERT(!rhs.is_infinity());
    lhs.value_ *= rhs.value_;
    return lhs;
  }

  static Number mod(Number a, Number b) {
    ASSERT(!a.is_infinity());
    ASSERT(!b.is_infinity());
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
    ASSERT(!a.is_infinity());
    ASSERT(!b.is_infinity());
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
    *quotient = q;
    *remainder = a;
  }

  static bool bitwise_and_is_zero(Number a, Number b) {
    ASSERT(!a.is_infinity());
    ASSERT(!b.is_infinity());
    if (a.value_ & b.value_)
      return false;
    return true;
  }

 private:
  uint64_t value_;
};

// An element of the finite field of order p, where p is prime, or special value
// Infinity.  Represented by a Number corresponding to integer value in [0, p),
// or special value Infinity.  Behavior is undefined for other values.
// p is not stored to save space.
class Element {
 public:
  Element() = default;
  Element(uint64_t value) : value_(value) {}
  Element(Number value) : value_(value) {}

  static Element Infinity() {
    return Element(Number::Infinity());
  }

  bool is_infinity() {
    return value_.is_infinity();
  }

  bool operator==(const Element& other) {
    return value_ == other.value_;
  }

  bool operator!=(const Element& other) {
    return value_ != other.value_;
  }

  static Element negate(Element a, Number p) {
    ASSERT(!a.is_infinity());
    ASSERT(a.value_ < p);
    if (a == 0)
      return a;
    return Element(p - a.value_);
  }

  static Element add(Element a, Element b, Number p) {
    ASSERT(!a.is_infinity());
    ASSERT(!b.is_infinity());
    ASSERT(!p.is_infinity());
    ASSERT(a.value_ < p);
    ASSERT(b.value_ < p);
    Number sum = a.value_ + b.value_;
    if (sum >= p) sum -= p;
    return Element(sum);
  }

  static Element subtract(Element a, Element b, Number p) {
    return add(a, negate(b, p), p);
  }

  static Element divide(Element a, Element b, Number p) {
    return multiply(a, invert(b, p), p);
  }

  static Element multiply(Element a, Element b, Number p) {
    ASSERT(!a.is_infinity());
    ASSERT(!b.is_infinity());
    ASSERT(!p.is_infinity());
    ASSERT(a.value_ < p);
    ASSERT(b.value_ < p);
    return Number::multiply(a.value_, b.value_, p);
  }

  static Element invert(Element i, Number p) {
    const Number one(1);
    if (i.value_ == one) return Element(one);
    Number a(i.value_);
    Number b;
    Number q;
    Number::divide(p, a, &q, &b);
    Element s0 = Element::negate(q, p);
    Element s1(one);
    Element s2;
    while (b != one) {
      Number::divide(a, b, &q, &a);
      s2 = s1;
      s1 = s0;
      s0 = Element::add(
          s2, Element::negate(Number::multiply(Number::mod(q, p), s1.value_, p), p), p);
      if (a == one) {
        return s0;
      }
      Number::divide(b, a, &q, &b);
      s2 = s1;
      s1 = s0;
      s0 = Element::add(
          s2, Element::negate(Number::multiply(Number::mod(q, p), s1.value_, p), p), p);
    }
    return s0;
  }

 private:
  Number value_;
};

// Point on an elliptic curve y^2 = x^3 + ax + b over finite field of order p,
// where p is prime.  a, b, p are not stored to save space.
class Point {
 public:
  Point(Element x, Element y) : x_(x), y_(y) {}

  static Point Infinity() {
    return Point(Element::Infinity(), 0);
  }

  bool is_infinity() {
    return x_.is_infinity();
  }

  bool operator==(const Point& other) {
    return x_ == other.x_ && y_ == other.y_;
  }

  static Point multiply(Point A, Point B, Number p, Number a) {
    if (A.is_infinity())
      return B;
    if (B.is_infinity())
      return A;
    if (A.x_ == B.x_) {
      if (Element::add(A.y_, B.y_, p) == 0)
        return Infinity();
      Element s(Element::divide(
          Element::add(Element::multiply(3, Element::multiply(A.x_, A.x_, p), p), a, p),
          Element::multiply(2, A.y_, p), p));
      Element x(Element::subtract(
          Element::multiply(s, s, p),
          Element::multiply(2, A.x_, p), p));
      Element y(Element::negate(Element::add(A.y_,
          Element::multiply(s, Element::subtract(x, A.x_, p), p), p), p));
      return Point(x, y);
    }
    Element s(Element::divide(
        Element::subtract(A.y_, B.y_, p),
        Element::subtract(A.x_, B.x_, p),
        p));
    Element x(Element::subtract(
        Element::multiply(s, s, p),
        Element::add(A.x_, B.x_, p),
        p));
    Element y(Element::negate(Element::add(
        A.y_,
        Element::multiply(s, Element::subtract(x, A.x_, p), p),
        p), p));
    return Point(x, y);
  }

  static Point power(Point A, Number exponent, Number p, Number a) {
    Point answer(Point::Infinity());
    if (exponent == 0)
      return answer;
    Number q(1);
    while (true) {
      if (!Number::bitwise_and_is_zero(exponent, q)) {
        answer = multiply(answer, A, p, a);
        exponent -= q;
        if (exponent == 0)
          return answer;
      }
      q <<= 1;
      A = multiply(A, A, p, a);
    }
  }

  static Point invert(Point A, Number p) {
    return Point(A.x_, Element::negate(A.y_, p));
  }

 private:
  Element x_;
  Element y_;
};

int main() {
  // Basic tests.
  ASSERT(Number::Infinity().is_infinity());
  ASSERT(!Number(42).is_infinity());
  ASSERT(Element::add(12, 20, 23) == 9);
  ASSERT(Element::multiply(8, 9, 23) == 3);
  ASSERT(Number::mod(2, 5) == 2);
  ASSERT(Number::mod(7, 5) == 2);
  ASSERT(Number::mod(12, 5) == 2);
  ASSERT(Number::mod(17, 5) == 2);
  Number q, r;
  ASSERT((Number::divide(2, 5, &q, &r), q == 0 && r == 2));
  ASSERT((Number::divide(7, 5, &q, &r), q == 1 && r == 2));
  ASSERT((Number::divide(12, 5, &q, &r), q == 2 && r == 2));
  ASSERT((Number::divide(17, 5, &q, &r), q == 3 && r == 2));

  // Benchmark inversion in finite field.
  /*
  //const Number p(29311);
  //const Number p(100057);
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
  */

  // Test elliptic curve operations.
  // p=23, a=1, b=1:
  Number p(23);
  Number a(1);
  ASSERT(Point::multiply(Point(3, 10), Point(9, 7), p, a) == Point(17, 20));
  ASSERT(Point::multiply(Point(9, 7), Point(3, 10), p, a) == Point(17, 20));
  ASSERT(Point::power(Point(3, 10), 2, p, a) == Point(7, 12));
  ASSERT(Point::power(Point(3, 10), 3, p, a) == Point::multiply(Point(7, 12), Point(3, 10), p, a));
  ASSERT(Point::multiply(Point(3, 10), Point::Infinity(), p, a) == Point(3, 10));
  ASSERT(Point::multiply(Point::Infinity(), Point(3, 10), p, a) == Point(3, 10));
  ASSERT(Point::multiply(Point(3, 10), Point::invert(Point(3, 10), p), p, a) == Point::Infinity());
  ASSERT(Point::multiply(Point::invert(Point(9, 7), p), Point(9, 7), p, a) == Point::Infinity());
  ASSERT(Point::invert(Point::Infinity(), p) == Point::Infinity());
  return 0;
}
