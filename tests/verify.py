"""Deterministic independent behavior checks for all seven executables (Python 3.8+)."""
import argparse
from pathlib import Path
import random
import subprocess
from collections import deque

parser = argparse.ArgumentParser()
parser.add_argument('--bin-dir', type=Path, required=True)
args = parser.parse_args()
rng = random.Random(3334)
counts = {}

def check(name, data, expected):
    exe = args.bin_dir / name
    if not exe.exists():
        exe = exe.with_suffix('.exe')
    result = subprocess.run([str(exe.resolve())], input=data, text=True,
                            capture_output=True, timeout=30)
    assert result.returncode == 0, (name, result.returncode, result.stderr)
    want = str(expected).split()
    assert result.stdout.split() == want, (name, data[:1000], want[:100], result.stdout[:1000])
    counts[name] = counts.get(name, 0) + 1

# Published samples, plus EOF with no data for all programs.
samples = {
    'prime_path_bfs': ('3\n1033 8179\n1373 8017\n1033 1033\n', '6\n7\n0\n'),
    'dynamic_quadtree': ('2\n2\n0011\n0001\n1111\n0111\n3\n3 1\n3 3\n2 3\n2\n0000\n0000\n0000\n0000\n1\n1 1\n', '13\n17\n13\n9\n'),
    'fleet_distance': ('4\nM 2 3\nC 1 2\nM 2 4\nC 4 2\n', '-1\n1\n'),
    'running_median': ('7\n1 3 5 7 9 11 6\n', '1\n3\n5\n6\n'),
    'lru_cache': ('4 4\n1 2 3 4\n5\n4\n2\n1\n1 2\n2\n1\n1\n', '0011\n1 2 4 5\n01\n1\n'),
    'coin_flipping': ('5 4\n1010\n0101\n1010\n1010\n1010\n5 4\n0010\n1101\n0110\n0110\n1011\n', '20\n17\n'),
    'customs': ('4\n1 4 1 2 2 3\n3 2 2 3\n86401 2 3 4\n86402 1 5\n', '3\n3\n3\n4\n')
}
for name, (data, expected) in samples.items():
    check(name, data, expected)
    check(name, '', '')
check('customs', '3\n1 4 4 1 2 2\n2 2 2 3\n10 1 3\n', '3 4 4')

# BFS reference constructs explicit adjacency via wildcard digit buckets.
prime = [True] * 10000
prime[0] = prime[1] = False
for p in range(2, 100):
    if prime[p]:
        for multiple in range(p*p, 10000, p): prime[multiple] = False
primes = [p for p in range(1000, 10000) if prime[p]]
buckets = {}
for p in primes:
    digits = str(p)
    for i in range(4): buckets.setdefault(digits[:i]+'*'+digits[i+1:], []).append(p)
adj = {p: set() for p in primes}
for group in buckets.values():
    for p in group: adj[p].update(q for q in group if q != p)
def prime_distance(start, end):
    distance = {start: 0}; queue = deque([start])
    while queue:
        p = queue.popleft()
        if p == end: return distance[p]
        for q in adj[p]:
            if q not in distance:
                distance[q] = distance[p]+1; queue.append(q)
    return 'Impossible'
pairs = [(1009,1009), (9973,1009)] + [(rng.choice(primes),rng.choice(primes)) for _ in range(70)]
check('prime_path_bfs', str(len(pairs))+'\n'+'\n'.join(f'{a} {b}' for a,b in pairs),
      '\n'.join(str(prime_distance(a,b)) for a,b in pairs))
check('prime_path_bfs', '2\n999 1033\n1033 1000\n', 'Impossible Impossible')

# Quadtree reference recomputes the entire minimal representation after each pixel flip.
def node_count(grid, row, col, side):
    color = grid[row][col]
    if all(grid[r][c] == color for r in range(row,row+side) for c in range(col,col+side)):
        return 1
    half = side//2
    return 1 + sum(node_count(grid,row+dr,col+dc,half) for dr,dc in [(0,0),(0,half),(half,0),(half,half)])
for k in range(5):
    n = 1 << k
    for trial in range(12):
        grid = [[rng.randrange(2) for _ in range(n)] for _ in range(n)]
        if trial in (0,1): grid = [[trial for _ in range(n)] for _ in range(n)]
        data = '1\n'+str(k)+'\n'+'\n'.join(''.join(map(str,row)) for row in grid)+'\n'
        operations = [(0,0)]*2 + [(rng.randrange(n),rng.randrange(n)) for _ in range(40)]
        data += str(len(operations))+'\n'+'\n'.join(f'{r+1} {c+1}' for r,c in operations)
        expected = []
        for r,c in operations:
            grid[r][c] ^= 1; expected.append(node_count(grid,0,0,n))
        check('dynamic_quadtree',data,' '.join(map(str,expected)))
# Maximum k and maximum number of operations, repeatedly split/merge the same pixel.
n = 1024
check('dynamic_quadtree','1\n10\n'+('0'*n+'\n')*n+'1000\n'+('1 1\n')*1000,
      ' '.join(str(41 if i%2==0 else 1) for i in range(1000)))

# DSU reference stores each fleet explicitly in order; no parent/distance formula.
for trial in range(30):
    fleets = [[i] for i in range(1,51)]
    commands = []; expected = []
    for _ in range(250):
        x,y = rng.randint(1,50),rng.randint(1,50)
        fx = next(f for f in fleets if x in f); fy = next(f for f in fleets if y in f)
        if rng.random()<0.4 and fx is not fy:
            commands.append(f'M {x} {y}')
            fy.extend(fx); fleets.remove(fx)
        else:
            commands.append(f'C {x} {y}')
            expected.append(-1 if fx is not fy else max(0,abs(fx.index(x)-fx.index(y))-1))
    check('fleet_distance',str(len(commands))+'\n'+'\n'.join(commands),' '.join(map(str,expected)))
commands = [f'M {i} {i+1}' for i in range(1,30000)] + ['C 1 30000','C 1 1','C 29999 30000','C 1 15000']
check('fleet_distance',str(len(commands))+'\n'+'\n'.join(commands),'29998 0 0 14998')

# Median reference sorts each prefix independently.
sequences = [[0],[10**9],[0]*101,list(range(200)),list(range(199,-1,-1))]
sequences += [[rng.randrange(10**9+1) for _ in range(rng.randint(1,200))] for _ in range(60)]
for values in sequences:
    expected = [sorted(values[:i])[i//2] for i in range(1,len(values)+1,2)]
    check('running_median',str(len(values))+'\n'+' '.join(map(str,values)),' '.join(map(str,expected)))
check('running_median','100000\n'+' '.join(map(str,range(100000))),
      ' '.join(str(i//2) for i in range(1,100001,2)))

# LRU reference is a plain list, using linear membership and moves.
for trial in range(60):
    n = rng.randint(1,20); values = rng.sample(range(1,60),n)
    queries = [rng.randint(1,60) for _ in range(120)]
    state = values[:]; hits = ''
    for value in queries:
        if value in state: hits+='1'; state.remove(value)
        else: hits+='0'; state.pop()
        state.insert(0,value)
    check('lru_cache',f'{n} {len(queries)}\n'+' '.join(map(str,values))+'\n'+'\n'.join(map(str,queries)),
          hits+'\n'+' '.join(map(str,state)))
check('lru_cache','1 4\n7\n7\n8\n8\n7\n','1010\n7')

# Coin reference enumerates BOTH row and column flips, independently of the greedy rule.
for trial in range(75):
    n,m = rng.randint(1,5),rng.randint(1,5)
    grid = [[rng.randrange(2) for _ in range(m)] for _ in range(n)]
    best = max(sum(grid[r][c]^((rm>>r)&1)^((cm>>c)&1) for r in range(n) for c in range(m))
               for rm in range(1<<n) for cm in range(1<<m))
    check('coin_flipping',f'{n} {m}\n'+'\n'.join(''.join(map(str,row)) for row in grid),best)
check('coin_flipping','100 10\n'+('0'*10+'\n')*100,1000)

# Customs reference scans all past ship records; empty ships and exact time boundaries included.
for trial in range(65):
    records = []; now = 1; expected = []
    for _ in range(80):
        now += rng.choice([0,1,86399,86400,86401,200000])
        nations = [rng.randint(1,15) for _ in range(rng.randrange(9))]
        records.append((now,nations))
        expected.append(len({nation for t,items in records if now-86400<t<=now for nation in items}))
    data = str(len(records))+'\n'+'\n'.join(f'{t} {len(items)} '+' '.join(map(str,items)) for t,items in records)
    check('customs',data,' '.join(map(str,expected)))
check('customs','3\n1 1 1\n2 0\n86403 0\n','1 1 0')
check('customs','4\n1 1 1\n86401 0\n172801 0\n172802 1 2\n','1 0 0 1')
check('customs','100000\n'+'\n'.join(f'{i} 3 1 2 3' for i in range(1,100001)), ' '.join(['3']*100000))
for name,count in sorted(counts.items()): print(f'{name}: PASS ({count} executable checks)')
print(f'Total: {sum(counts.values())} executable checks passed; deterministic seed 3334.')
