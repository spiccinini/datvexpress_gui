#!/usr/bin/env bash
set -x
g++ -Wall ./DVB-S2/*.cpp  ./encoder.cpp  -I DVB-S2/  -o encoder
