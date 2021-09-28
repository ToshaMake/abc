g++ -O0 -o bench0 bench.cpp
./bench0 resultO0.csv -O0
g++ -O1 -o bench1 bench.cpp
./bench1 resultO1.csv -O1
g++ -O2 -o bench2 bench.cpp
./bench2 resultO2.csv -O2
g++ -O3 -o bench3 bench.cpp
./bench3 resultO3.csv -O3