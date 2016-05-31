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

int add(int a, int b, int p) {
  int c = a+b;
  return c >= p ? c-p : c;
}

int mod(int a, int b, int* quot) {
  int q = 0;
  while (a<0) {
    a += b;
    --q;
  }
  while (a>=b) {
    a -= b;
    ++q;
  }
  if (quot) *quot = q;
  return a;
}

int multiply(int a, int b, int p) {
  return mod(a*b, p, nullptr);
}

int invert(int b, int p) {
  if (b==1) return 1;
  int q;
  int a = mod(p, b, &q);
  int s0 = -q;
  int s1 = 1;
  int s2;
  while (a != 1) {
    b = mod(b, a, &q);
    s2 = s1;
    s1 = s0;
    s0 = s2 - q*s1;
    if (b==1) {
      return mod(s0, p, nullptr);
    }
    a = mod(a, b, &q);
    s2 = s1;
    s1 = s0;
    s0 = s2 - q*s1;
  }
  return mod(s0, p, nullptr);
}

int main() {
  if (add(12, 20, 23) == 9) {
    cout << "PASS Addition." << endl;
  } else  {
    cout << "FAIL Addition." << endl;
  }
  if (multiply(8, 9, 23) == 3) {
    cout << "PASS Multiplication." << endl;
  } else  {
    cout << "FAIL Multiplication." << endl;
  }
  int p = 997;
  bool pass = true;
  for (int i = 1; i < p; ++i) {
    if (multiply(invert(i, p), i, p) != 1) {
      pass = false;
    }
  }
  if (pass) {
    cout << "PASS Inversion." << endl;
  } else {
    cout << "FAIL Inversion." << endl;
  }
  return 0;
}

#include <iostream>

#include <gmp.h>

using std::cout;
using std::endl;

void invert(mpz_t i, mpz_t b, mpz_t p) {
  if (mpz_cmp_ui(b, 1) == 0) {
    mpz_set_ui(i, 1);
    return;
  }
  mpz_t q;
  mpz_init(q);
  mpz_t a;
  mpz_init(a);
  mpz_fdiv_qr(q, a, p, b);
  mpz_t s0;
  mpz_init(s0);
  mpz_neg(s0, q);
  mpz_t s1;
  mpz_init_set_ui(s1, 1);
  mpz_t s2;
  mpz_init(s2);
  mpz_t tmp;
  mpz_init(tmp);
  while (mpz_cmp_ui(a, 1) != 0) {
    mpz_fdiv_qr(q, b, b, a);
    mpz_set(s2, s1);
    mpz_set(s1, s0);
    mpz_mul(tmp, q, s1);
    mpz_sub(s0, s2, tmp);
    if (mpz_cmp_ui(b, 1) == 0) {
      mpz_fdiv_r(i, s0, p);
      return;
    }
    mpz_fdiv_qr(q, a, a, b);
    mpz_set(s2, s1);
    mpz_set(s1, s0);
    mpz_mul(tmp, q, s1);
    mpz_sub(s0, s2, tmp);
  }
  mpz_fdiv_r(i, s0, p);
}

int main() {
  mpz_t p;
  mpz_init_set_ui(p, 997);
  bool pass = true;
  mpz_t inverse;
  mpz_init(inverse);
  mpz_t prod;
  mpz_init(prod);
  mpz_t r;
  mpz_init(r);
  for (int i = 1; i < 997; ++i) {
    mpz_t mpzi;
    mpz_init_set_ui(mpzi, i);
    invert(inverse, mpzi, p);
    mpz_mul(prod, inverse, mpzi);
    mpz_fdiv_r(r, prod, p);
    if (mpz_cmp_ui(r, 1) != 0) {
      cout << "iteration " << i << " fail" << endl;
      pass = false;
    }
  }
  if (pass) {
    cout << "PASS Inversion." << endl;
  } else {
    cout << "FAIL Inversion." << endl;
  }
  return 0;
}
