#!/usr/bin/env python3
import sys

if len(sys.argv) != 3:
    print("Usage: upper.py <in> <out>", file=sys.stderr)
    sys.exit(2)

src, out = sys.argv[1], sys.argv[2]
with open(src, "r", encoding="utf-8") as f:
    text = f.read()

with open(out, "w", encoding="utf-8") as f:
    f.write(text.upper())

