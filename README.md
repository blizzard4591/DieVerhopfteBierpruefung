# Die verhopfte Bierpruefung

The idea of this game is that you need to arrange the numbers from 1 to 12 in the twelve slots of the gameboard (a six-sided star) such that every connected row of four numbers sums up to 26.
There are six such rows, along the outer edges of the two triangles forming the star.

<img src="/Gameboard.png?raw=true" width="350" alt="The gameboard">

While the tabletop variant of this game asks you to provide one such solution, you might of course also be interested in all solutions.
Because of rotational and mirror symmetry of the star, every unique solution induces eleven further symmetric solutions.

This tool simply enumerates all possible 12! permutations of the twelve numbers and tests them for validity.
If a solutions is found viable, it is then deduplicated against solutions already discovered.

One a standard desktop computer, enumerating all 479.001.600 possible permutations plus checking and printing them takes roughly 85 seconds.
There are 960 solutions including the symmetry duplicates and 80 "unique" solutions.

<img src="/OneSolution.png?raw=true" width="350" alt="One possible solution">

### Is this efficient?
No. Not even close. Because of symmetries, one could stop the search when the first number reaches seven, but this is not done.
There is no smart backtracking, etc.
Feel free to improve!

### How are the fields numbered internally?
We start labelling with 0 at the edge of one triangle, fill it up all around and then label the second triangle, all clockwise.

<img src="/Numbering.png?raw=true" width="350" alt="One possible solution">

So, reading a solution like `6, 1, 9, 10, 2, 3, 11, 4, 5, 8, 7, 12` means `6, 1, 9, 10`, `10, 2, 3, 11` and `11, 4, 5, 6` form the first triangle (looping around back to the 6) and `8, 9, 2, 7`, `7, 3, 4, 12` and `12, 5, 1, 8` form the second triangle.

## License
The tool is governed by the GNU GPL v2.0 license. See [`LICENSE`](LICENSE) for more information.

## Requirements
 - [CMake](https://cmake.org/)
 - C++17, meaning a recent MSVC/GCC/Clang that supports at least C++17
 
## How to build

On Linux or MacOS, if CMake and a Compiler are readily available:
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

On Windows, use the CMake GUI to create project files for Visual Studio, open the project and build in Release mode for optimal performance.
