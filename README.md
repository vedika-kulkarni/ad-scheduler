# AI-Powered Online Ad Slot Allocation System

A full-stack advertisement scheduling and optimization platform developed to maximize ad revenue through algorithmic slot allocation and profit optimization techniques.

The system integrates multiple scheduling and optimization algorithms implemented in C, combined with a web-based frontend and Node.js server layer for interactive ad allocation analysis.

---

# Problem Statement

Digital advertising platforms must efficiently allocate limited ad slots while maximizing:
- Revenue generation
- Advertisement visibility
- Resource utilization
- Scheduling efficiency

This project simulates an intelligent advertisement scheduling engine capable of selecting optimal ad placements using greedy and dynamic programming algorithms.

---

# Key Features

## Ad Scheduling Engine
- Intelligent advertisement slot allocation
- Multiple scheduling optimization algorithms
- Profit maximization strategies
- Capacity-constrained allocation support

---

## Optimization Algorithms

| Algorithm | Time Complexity | Purpose |
|---|---|---|
| Greedy (End Time) | O(n log n) | Earliest finishing activity selection |
| Greedy (Ratio) | O(n²) | Maximum bid-to-duration prioritization |
| DP Weighted Interval | O(n log n) | Optimal non-overlapping scheduling |
| DP Knapsack | O(n × W) | Capacity-based revenue optimization |

---

## Interactive Web Interface
- Real-time ad scheduling simulation
- Algorithm comparison visualization
- Dynamic input management
- Allocation result display

---

## Backend Processing
- Node.js server integration
- C program execution pipeline
- JSON-based data communication
- CLI and web execution support

---

# System Architecture

```text
Frontend (HTML/CSS/JavaScript)
            │
            ▼
Node.js Server Layer
            │
            ▼
C-Based Scheduling Engine
            │
            ▼
Optimization Algorithms
```

---

# Project Architecture

```text
ad_scheduler/
│
├── src/
│   └── ad_scheduler.c
│
├── public/
│   └── index.html
│
├── server.js
├── run.bat
├── run.sh
└── README.md
```

---

# Tech Stack

## Backend
- C Programming
- Node.js

## Frontend
- HTML5
- CSS3
- JavaScript

## Concepts & Algorithms
- Greedy Algorithms
- Dynamic Programming
- Interval Scheduling
- Knapsack Optimization

---

# Core Functionalities

## Scheduling Optimization
- Conflict-free ad allocation
- Revenue-focused scheduling
- Capacity-aware optimization

---

## Algorithm Comparison
- Compare scheduling approaches
- Analyze optimization performance
- Evaluate profit outputs

---

## Cross-Platform Execution
- Windows batch execution support
- Linux/Mac shell execution support
- CLI and browser-based interaction

---

# Installation & Setup

## 1. Clone Repository

```bash
git clone https://github.com/yourusername/ad_scheduler.git
cd ad_scheduler
```

---

## 2. Install Node.js

Download and install:

```text
https://nodejs.org
```

---

## 3. Compile C Program

### Windows

```bash
gcc src\\ad_scheduler.c -o ad_scheduler.exe -lm
```

### Linux / Mac

```bash
gcc src/ad_scheduler.c -o ad_scheduler -lm
```

---

## 4. Run Server

```bash
node server.js
```

---

## 5. Open Application

```text
http://localhost:3000
```

---

# CLI Execution Example

```bash
echo '{"method":"greedy","capacity":20,"ads":[{"name":"BrandX","start":1,"end":3,"bid":50},{"name":"Nike","start":2,"end":5,"bid":60}]}' | ./ad_scheduler
```

---

# Supported Scheduling Methods

- greedy
- ratio
- dp
- knapsack
- all

---

# Screenshots

## Scheduling Dashboard
<img width="653" height="890" alt="image" src="https://github.com/user-attachments/assets/dab44ad0-8c3f-47e5-9984-9735f1cdc044" />


---

## Algorithm Comparison Results
<img width="621" height="531" alt="image" src="https://github.com/user-attachments/assets/aa50ec6f-b511-4172-9804-fc5e7b231c62" />

---

# Future Improvements

- AI-based bid prediction
- Real-time scheduling analytics
- Cloud deployment
- Database integration
- User authentication
- Visualization dashboards
- API-based scheduling service

---

# Learning Outcomes

- Algorithm design & optimization
- Dynamic programming implementation
- Greedy scheduling techniques
- Full-stack integration
- Backend process communication
- Cross-language system integration
- Performance optimization concepts

---

# Deployment Options

The application can be deployed using:
- Render
- Railway
- Heroku
- Vercel (Frontend)

---

# Author

Vedika Kulkarni

GitHub: https://github.com/vedika-kulkarni 
