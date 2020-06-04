# Assignment 7

github: https://github.com/Vishnu44d/iss/tree/master/ass7

## Problem

Implement the Chinese Remainder Theorem. Hence solve for x from the following set of
simultaneous congruences:

x ≡ 6 (mod 11), x ≡ 13 (mod 16), x ≡ 9 (mod 21), x ≡ 19 (mod 25)

---

## Solution

1. chineseRemainderTh.c implemets the solution.

2. General form of equation is: x ≡ b (mod m)

3. The variable n contains the number of equations.

4. The array B contains the list of b<sub>i</sub>s.

5. The array M contains the list of m<sub>i</sub>s.

6. It just prints the value of x.

7. To run the code

```bash
1. gcc -o chRemainderTh chineseRemainderTh.c
2. ./chRemainderTh
```
