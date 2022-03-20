mkdir -p bin
cd bin
rm -rf *
cmake ..; make
cd clonegame
./clonegame
