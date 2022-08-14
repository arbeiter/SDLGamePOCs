mkdir -p bin
cd bin
rm -rf *
cmake -DCMAKE_BUILD_TYPE=Debug ..; make
cd clonegame
./clonegame
