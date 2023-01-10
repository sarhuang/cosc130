#!/bin/bash
xxd $1 > 1.txt
xxd $2 > 2.txt
vimdiff 1.txt 2.txt
rm -rf 1.txt 2.txt
