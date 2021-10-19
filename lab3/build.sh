#!/bin/bash

./bench -m RAM_CACHE -b 1024 -l 10
./bench -m SSD_MB -b 1024 -l 10

./bench -m SSD_CL -b 1Mb -l 10
./bench -m flush_MB -b 1024 -l 10