# How to set a specified bit of a binary value to a specified value

How to set a specified bit of a binary value to a specified value. Suppose we have a binary value `1010`, and we want to set the second bit to `0`.

## Exclusive OR (XOR) ^

First, it's necessary to understand the concept and properties of the XOR `^` operator. XORing with 0 results in the original value, and XORing with the same value results in 1. Another important property is that XORing a number twice with the same value results in the original number.

## Formula

Formula：Here, `x` represents the original value, `n` represents the `nth` bit position, and `a` represents the desired value (0 or 1) to be set.

First, the value of `(x&(1 << n))` is: it preserves the original value of the `nth` bit and sets all other bits to zero. Then, XOR this value with the original value `x` to obtain a new value: all bits are the same as the original value except the `nth` bit, which is set to zero (this is because XORing with `0` keeps the original value, and XORing with the same value results in `0`). Then, XOR this result with `(a << n)` to set the `nth` bit to `a` (this is because XORing with `0` results in the original value).

## Process

```bash
1、OriginalValue:               1 0 | 1 | 0     x
2、Other_bits_set_to_0:         0 0 | 1 | 0     x & ( 1 << n )
3、Desired value                0 0 | 0 | 0     a << n
4、XOR 1 and 2:                 1 0 | 0 | 0     x & ( 1 << n ) ^ x
5、XOR 3 and 4:                 1 0 | 0 | 0     ((x&(1 << n)) ^ x) ^ (a << n)
```