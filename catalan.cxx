/* Print all valid nested parenthesis stings up to given length. */
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
 
using namespace std;

/* Calculate nth Catalan number.  Long int can handle up to n=14. */
long int catalan(int n) {
  /* C(n) = (2n choose n) - (2n choose n-1) 
   * = (1/n - 1/(n+1)) * (2n)! / ((n-1)! n!)
   * = (2n)! / (n! (n+1)!). */
  long int binom = 1;
  int i;
  for(i=n+2; i<=2*n; i++)
    binom *= i;
  for(i=1; i<=n; i++)
    binom /= i;
  return binom;
}

/* Generate all strings of n pair of parentheses strings, given the same for
 * shorter strings. */
vector<string> generateNextParenStrings(vector<vector<string>> allParenStrings) {
  vector<string> nextParenStrings;
  // Bootstrap the recursion with the single sting of zero length.
  if (allParenStrings.size() == 0) {
    nextParenStrings.push_back(string(""));
    return nextParenStrings;
  }
  /* Otherwise, divide the string at the first valid substring, that is, when
   * the difference in the number of ``('' and ``)'' first reaches zero for a
   * nonempty substring.  This is the left part, which is a valid string when
   * leading ``('' and trailing ``)'' are stripped.  Right part is also a
   * valid string, of appropriate length.  Hence the two outer iterators, one
   * reverse. */
  vector<vector<string>>::iterator leftOuter;
  vector<vector<string>>::reverse_iterator rightOuter;
  vector<string>::iterator leftInner, rightInner;
  for(leftOuter = allParenStrings.begin(),
      rightOuter = allParenStrings.rbegin();
      leftOuter != allParenStrings.end();
      leftOuter++, rightOuter++)
    /* Inner iterators scan possible realizations of substrings.  The
     * number of them gets multiplied together. */
    for(leftInner = leftOuter->begin(); leftInner != leftOuter->end();
        leftInner++)
      for(rightInner = rightOuter->begin(); rightInner != rightOuter->end();
          rightInner++) {
        // Concatenate substrings according to rules above.
        stringstream parenString;
        parenString << "(" << *leftInner << ")" << *rightInner;
        nextParenStrings.push_back(parenString.str());
      }
  return nextParenStrings;
}

/* Generate vector of vectors of all strings.  Outer vector scans all lengths
 * from 0 to n, inner vector scans realizations. */
vector<vector<string>> generateAllParenStrings(int n) {
  vector<vector<string>> allParenStrings;
  // Repeat n+1 times: generate strings of next length given all shorter ones.
  for(int i=0; i<=n; i++)
    allParenStrings.push_back(generateNextParenStrings(allParenStrings));
  return allParenStrings;
}

// Print all strings up to given length.
void printAllParenStrings(vector<vector<string>> allParenStrings) {
  int n;
  vector<vector<string>>::iterator outer;
  // Scan through lengths.
  for (n=0, outer = allParenStrings.begin(); outer != allParenStrings.end(); n++, outer++) {
    vector<string>::iterator inner;
    cout << "n = " << n << ", C(n) = " << catalan(n) << ": ";
    // Print realizations, separated by comma, but not at the end.
    for (inner = outer->begin(); inner != outer->end(); inner++) {
      cout << *inner;
      if (inner+1 != outer->end())
        cout << ", ";
    }
    cout << endl;
  }
  return;
}

int main() {
  // Set maximum length.
  const int maxn = 6;
  // Generate all strings up to given length.
  vector<vector<string>> allParenStrings = generateAllParenStrings(maxn);
  // Print them.
  printAllParenStrings(allParenStrings);
  return 0;
}
