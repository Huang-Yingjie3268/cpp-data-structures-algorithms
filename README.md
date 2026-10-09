# C++ Data Structures and Algorithms

Seven C++ console programs covering representative data structures and algorithms, selected from undergraduate Data Structures coursework (CS3334). Each exercise covers a different technique: BFS over prime-number states, split-and-merge quadtrees, weighted disjoint sets, two-heap medians, LRU caching, exhaustive coin-flip search, or FIFO sliding windows. The selection keeps representative examples rather than every homework problem.

The programs use C++17 and standard-library containers, read standard input and write standard output. Each has an example input/output pair, and CMake builds the collection as separate executables. A Python regression suite checks samples, boundaries and comparisons with independent reference implementations. These are implementations of established coursework algorithms, not original algorithm research; detailed input constraints and complexity notes accompany the code.
## Implementations

| Program | Problem and approach | Time complexity |
| --- | --- | --- |
| [Prime path](src/graph/prime_path_bfs.cpp) | Fewest single-digit changes between four-digit primes; sieve and BFS | Sieve O(U log log U); query O(U + V + E) |
| [Dynamic quadtree](src/tree/dynamic_quadtree.cpp) | Count compressed image-tree nodes after pixel flips; recursive split/merge | Build O(s² log s) upper bound; flip O(log s) |
| [Fleet distance](src/disjoint_set/fleet_distance.cpp) | Join ordered fleets and count intervening ships; weighted path compression | O(N) worst case per find |
| [Running median](src/heap/running_median.cpp) | Median of each odd-length prefix; max heap and min heap | O(n log n) total |
| [LRU cache](src/cache/lru_cache.cpp) | Track hits, misses and recency; list and hash table | O(n + m) expected total |
| [Coin flipping](src/search/coin_flipping.cpp) | Maximize heads; enumerate column subsets and choose row flips | O(2^m n m) |
| [Customs window](src/sliding_window/customs.cpp) | Count nationalities in the preceding 24 hours; FIFO and frequencies | O(R + P) expected total |

Here `U = 10000`; `V, E` are reachable prime states and edges; `s` is image side length; and `N = 30000` ships. LRU uses capacity `n` and `m` queries; coin flipping uses `n` rows and `m <= 10` columns. Customs uses `R` arrival records and `P` passengers. Hash-table bounds are expected, and directional fleet union does not claim the inverse-Ackermann bound of union by rank.

These programs use C++17 and STL containers. [Input formats and implementation notes](docs/IMPLEMENTATIONS.md) explain constraints, storage costs and the mapping to the original exercises.

## Getting started

Requires CMake 3.16+ and a C++17 compiler. Python 3.8+ is needed for regression tests; CMake registers them when Python is available.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Programs read standard input and write standard output:

```bash
./build/prime_path_bfs < examples/prime_path_bfs.in
```

With a Windows Visual Studio generator, executables are in `build/Release/`. Use Command Prompt for input redirection:

```bat
build\Release\prime_path_bfs.exe < examples\prime_path_bfs.in
```

To compile a single exercise:

```bash
g++ -std=c++17 -O2 -Wall -Wextra src/graph/prime_path_bfs.cpp -o prime_path_bfs
```

## Example: prime path

BFS searches the graph of four-digit primes, changing one digit per step. The included sample is:

```text
3
1033 8179
1373 8017
1033 1033
```

Output:

```text
6
7
0
```

Each of the seven programs has a matching `.in` / `.out` pair in [examples/](examples/). The quadtree example splits a uniform 2×2 image and merges it after a second flip; the running-median example illustrates heap balancing over odd-length prefixes.

## Testing

The recorded C++17/MSVC 19.44 run passed **380 executable checks**, including samples, boundary cases, seeded comparisons with independent reference implementations and selected maximum-size inputs. These are earlier results, not a new full-suite run for this documentation cleanup. [Testing notes](docs/TESTING.md) explain the commands and coverage.

## Coursework context and contributions

The repository collects coursework solutions, not original algorithm research. Earlier cleanup kept the algorithmic approaches while adding descriptive names, tests and fixes such as iterative weighted path compression and safe queue expiration. Individual authorship is not independently established by the retained source comments, so no exclusive ownership of every implementation is claimed.

Original problem PDFs and unselected exercises are excluded. No license has been assigned without confirmed rights to the coursework materials.

## Limitations

The programs assume complete inputs within the documented exercise constraints. They are separate console exercises, with no common application interface or general-purpose input-validation layer.
