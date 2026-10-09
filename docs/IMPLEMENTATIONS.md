# Input Contracts and Coursework Provenance

Programs assume complete inputs within the original problem constraints. They are
console exercises rather than general-purpose input-validation libraries. Empty
standard input exits cleanly. Paths below are relative to the supplied archive.
No reliable author identity was found in source comments; these mappings preserve
context without asserting independent authorship of algorithms or problem statements.

| Target | Original source | Problem reference |
|---|---|---|
| prime_path_bfs | Assignment/CS3334_22/CS3334_22/CS3334_22.cpp | Questions/22.pdf |
| dynamic_quadtree | Assignment/CS3334_748/CS3334_748/CS3334_748.cpp | Questions/748.pdf |
| fleet_distance | Assignment/CS3334_834/CS3334_834/CS3334_834.cpp | Questions/834.pdf |
| running_median | Assignment/CS3334_829/CS3334_829/CS3334_829.cpp | Questions/829.pdf |
| lru_cache | Assignment/CS3334_860/CS3334_860/CS3334_860.cpp | Questions/860.pdf |
| coin_flipping | Assignment/CS3334_741/CS3334_741/CS3334_741.cpp | Questions/741.pdf |
| customs | Assignment/CS3334_825/CS3334_825/CS3334_825.cpp | Questions/825.pdf |

## Formats and Bounds

- **prime_path_bfs:** case count (at most 100), then pairs of four-digit primes.
  Prints minimum changes, or `Impossible`. Out-of-range/non-prime endpoints also
  return `Impossible`. The sieve and per-digit BFS are unchanged.
- **dynamic_quadtree:** case count; for each case `k` (0..10), `2^k` binary rows,
  operation count (1..1000), then one-based row/column pairs. Prints the number of
  active tree nodes after each flip. The node pool retains unreachable descendants
  after merging: allocated storage is O(s² + F log s), for F flips, rather than
  always proportional to the current compressed tree. Input matrix adds O(s²).
  Stable integer indices preserve correctness across vector reallocations; C++17
  assignment sequencing is required for recursive append-and-assign expressions.
- **fleet_distance:** instruction count (1..200000); `M x y` appends x's fleet after
  y's fleet; `C x y` prints intervening ships, or -1 if disconnected. IDs are
  1..30000. The statement guarantees that merges involve different fleets.
  Self-query returns 0. Iterative two-pass compression removes the original deep
  recursion risk and accumulates the same distances. Space O(N).
- **running_median:** length (1..100000), then values (0..10^9). Prints only the
  median of each odd-length prefix, one per line. Heap insertion and balancing are
  preserved. Space O(n).
- **lru_cache:** repeated cases until EOF: positive capacity (<=10000), query count
  (<=1000000), distinct initial values ordered most recent first, then queries.
  Prints one concatenated hit/miss string followed by final recency order.
  The sample resolves ambiguous prose in the supplied statement in favor of this
  hit-string format. Initial de-duplication was removed because initial values are
  guaranteed distinct. Space O(n + m), including the buffered hit string.
- **coin_flipping:** cases until EOF: rows (1..100), columns (1..10), binary rows.
  Prints maximum heads. For each column subset, rows are independent, so choosing
  the better orientation of each row is optimal. Column flips are undone before
  the next subset. Original exhaustive/greedy algorithm retained. Space O(nm).
- **customs:** record count (<=100000), then arrival time, passenger count, and
  nationality IDs for each ship. Times are nondecreasing, <=10^9; total passengers
  <=300000, IDs <=100000. Window is `(arrival - 86400, arrival]`.
  Prints distinct nationalities per arrival. Same FIFO/frequency algorithm,
  storing timestamps directly and guarding queue access; zero-passenger records
  are handled safely. Space O(A + D), with active passengers A and nationalities D.

Formatting, descriptive names, direct standard-library includes, and newline output
were cleaned up. No new problem features, replacement algorithm, or framework was
introduced. No license is assigned to coursework materials without confirmed rights.
