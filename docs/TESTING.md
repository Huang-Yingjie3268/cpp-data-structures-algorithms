# Verification Evidence

Verification date: 2026-10-08 (Asia/Shanghai).
Compiler: MSVC 19.44.35219, Windows x64, C++17, Release (/O2), /W4.
Build: CMake 3.31.6 with Ninja. Python: 3.11.0.
A final clean build compiled and linked all seven executables with no compiler warnings.
CTest: 1 regression suite passed, 0 failed; 380 successful executable checks.
Random seed: 3334. Counts are process invocations, not individual assertions or input records.

| Program | C++17 build | Executable checks | Behavior evidence |
|---|---|---:|---|
| coin_flipping | Passed | 78 | Published two-case sample; 75 random small matrices vs full row AND column exhaustive reference; 100x10 uniform grid |
| customs | Passed | 71 | Both published samples; 65 random streams vs naive past-record scan; repeated times; exact 86400 boundary; empty records; 100000 records/300000 passengers |
| dynamic_quadtree | Passed | 63 | Published sample; k=0; random images and flips; repeated split/merge; k=10 with 1000 flips vs full image/tree recount |
| fleet_distance | Passed | 33 | Published sample; disconnected/same/adjacent ships; 30 random streams vs explicit ordered fleets; chain of 30000 ships |
| lru_cache | Passed | 63 | Published multi-case sample; 60 random streams vs plain list simulation; capacity 1; repeated hits and evictions |
| prime_path_bfs | Passed | 4 | Published sample; equal endpoints; digit-boundary primes; 70 random pairs + 2 boundary pairs vs explicit wildcard-bucket graph; invalid endpoints |
| running_median | Passed | 68 | Published sample; singleton, equal, sorted/reversed inputs; 60 random sequences vs sorted prefixes; n=100000 |

## Reproduce

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Or run `python tests/verify.py --bin-dir build` directly (use `build/Release` for a
multi-configuration generator). All references and random-case generation are included.

## Limits

These checks establish observed behavior on the tested inputs; they are not a formal
proof over all inputs. No GCC/Clang cross-platform run or AddressSanitizer run was
performed. Performance timings are not claimed. Maximum-size cases were included
for quadtree, median, fleet depth, coin dimensions and customs; LRU was checked on
small randomized cases rather than all million-query patterns. Programs assume the
documented valid input formats. Hash-table guarantees are expected, not adversarial
worst-case bounds. The quadtree node pool intentionally retains merged descendants.

The original recursive fleet code was converted to equivalent iterative weighted
compression, with self-query defined as zero. Customs queue expiration now checks
emptiness on every access and safely handles zero-passenger records. No unresolved
behavior mismatch was observed among selected programs.
