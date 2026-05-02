# AI Pathfinder
Simple C++ & ImGui program to find the shortest path from a start to an end goal around convex polygonal obstacles using three graph searching algorithms.

<img width="954" height="1030" alt="image" src="https://github.com/user-attachments/assets/f9938743-3123-4c9a-bb05-0f9a0427fb0f" />

The code is split into the following components:
- UI code by [@DAPOTTIS](https://github.com/DAPOTTIS)
- Visibility Graph by [@Mr-NOBY](https://github.com/Mr-NOBY)
- Graph search algorithms by [@PierreRamez](https://github.com/PierreRamez)

# Build instructions
to run, use the following commands:
```
mkdir build
```
after creating the build folder, run the following 2 commands to build:
```
cmake -DCMAKE_BUILD_TYPE=Debug -B "build/"
cmake --build build/
```

this will create a binary called AIPathfinder in the build folder, to run:
```
./build/AIPathfinder
```
