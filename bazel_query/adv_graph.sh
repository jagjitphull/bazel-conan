#!/bin/bash
# Create a custom dot file with better styling

bazel query "deps(//app:main)" --output=graph | \
  dot -Tpng \
    -Grankdir=TB \
    -Nshape=box \
    -Nstyle=filled \
    -Nfillcolor=lightblue \
    -Nfontsize=12 \
    -Efontsize=10 \
    -Gsplines=ortho \
    -o custom_graph.png

echo "Graph saved to custom_graph.png"

