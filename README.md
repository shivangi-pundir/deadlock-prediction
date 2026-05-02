# Automated Deadlock Prediction System

## CSE316 — Operating Systems | CA2 Project

## Project Overview
A system that predicts potential deadlocks by analyzing 
process dependencies and resource request patterns using 
Banker's Algorithm and Resource Allocation Graph (RAG).

## Modules
- **Module 1:** Process & Resource Simulator
- **Module 2:** Deadlock Prediction Engine (Banker's Algorithm)
- **Module 3:** Visualization & Reporting

## Technology Used
- Language: C (C99)
- Compiler: GCC
- Tools: GitHub, VS Code

## How to Run
```bash
make
./deadlock
```

## Features
- Banker's Algorithm for safe state detection
- RAG cycle detection using DFS
- Resource request prediction
- Risk level indicator (Safe/Warning/Danger)
- Results saved to output/results.txt