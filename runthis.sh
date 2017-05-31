#!/bin/bash
gcc pstree.c -o pstree
./pstree
dot -Tpng graph.dot -o pstree.png
