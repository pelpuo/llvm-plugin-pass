#!/bin/bash

rm -rf ./build
mkdir -p ./build

cd ./build && cmake ../customReorderPass && make
cd ..

rm -f test.bc new_test.ll

echo "### Running LLVM pass on test.ll"
echo ""

opt --load-pass-plugin=/home/local/ASUAD/ekayang/Documents/Github/llvm-plugin-pass/build/lib/CustomReorder.so --passes="reorder-custom" --inputfile=file.txt demo.ll -o demo2.bc

echo ""
echo "### Disassembling test.bc to new_test.ll"

llvm-dis demo2.bc -o demo2.ll
