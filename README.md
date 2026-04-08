# Online Ad Slot Allocation — DAA Project

## Team
- Algorithm: C Language (ad_scheduler.c)
- Frontend: HTML/CSS/JavaScript
- Server: Node.js (server.js)

## Algorithms Implemented (All in C)

| Algorithm | Time Complexity | Approach |
|---|---|---|
| Greedy (End Time) | O(n log n) | Activity selection by earliest end |
| Greedy (Ratio) | O(n²) | Best bid per unit duration first |
| DP Weighted Interval | O(n log n) | Provably optimal scheduling |
| DP Knapsack | O(n × W) | Capacity-constrained profit maximization |

## How to Run

### Prerequisites
- GCC compiler (MinGW on Windows, or `sudo apt install gcc` on Linux)
- Node.js (https://nodejs.org)

### Windows
```
Double-click run.bat
```
Or in cmd:
```
gcc src\ad_scheduler.c -o ad_scheduler.exe -lm
node server.js
```

### Linux / Mac
```bash
chmod +x run.sh
./run.sh
```

Then open **http://localhost:3000** in your browser.

## Run C Program Directly (CLI mode)

You can also test the C program directly:
```bash
echo '{"method":"greedy","capacity":20,"ads":[{"name":"BrandX","start":1,"end":3,"bid":50},{"name":"Nike","start":2,"end":5,"bid":60}]}' | ./ad_scheduler
```

Supported methods: `greedy`, `ratio`, `dp`, `knapsack`, `all`

## Project Structure
```
ad_scheduler/
├── src/
│   └── ad_scheduler.c     ← Main C program (all algorithms)
├── public/
│   └── index.html         ← Frontend UI
├── server.js              ← Node.js bridge server
├── run.bat                ← Windows launcher
├── run.sh                 ← Linux/Mac launcher
└── README.md
```
