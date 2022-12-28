#!/bin/env python3

import sys
from collections import deque
from functools import cmp_to_key


def readList(s):
    return eval(s)


def compare(a, b):
    # print(f"  Comparing {a} and {b}")
    if isinstance(a, int) and isinstance(b, int):
        if a < b:
            return True
        elif a > b:
            return False

        return None

    elif isinstance(a, list) and isinstance(b, list):
        i = 0
        while (i < len(a) and i < len(b)):
            res = compare(a[i], b[i])
            if res is not None:
                return res
            i += 1

        return compare(len(a), len(b))

    else:
        if isinstance(a, int):
            a = [a]
            assert isinstance(b, list)

        if isinstance(b, int):
            b = [b]
            assert isinstance(a, list)

        return compare(a, b)

    1/0


def py_compare(a, b):
    res = compare(a, b)
    if res is None:
        return 0

    if res is True:
        return -1

    if res is False:
        return 1


def main():
    lines = deque(sys.stdin.readlines())
    lines2 = [readList(line) for line in lines if line.strip()]

    i = 1
    s = 0
    while lines:
        val1 = readList(lines.popleft())
        val2 = readList(lines.popleft())

        if compare(val1, val2):
            s += i

        i += 1
        if lines:
            lines.popleft()

    print(s)

    lines2.append([[2]])
    lines2.append([[6]])
    lines2.sort(key=cmp_to_key(py_compare))

    pt2 = lines2.index([[2]]) + 1
    pt6 = lines2.index([[6]]) + 1
    print(pt2 * pt6)


if __name__ == "__main__":
    main()
