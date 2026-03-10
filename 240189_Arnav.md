# Memory-Efficient Versioned File Indexer

**CS253 Assignment 1**  
**Name:** Arnav Gupta  
**Roll Number:** 240189

---

## Overview

This program implements a **memory-efficient, versioned word-level file indexer** in C++. It processes large text files using a fixed-size buffer (never loading the full file into memory), builds a frequency index of all words per version, and supports three analytical queries:

- **Word Count Query** — frequency of a word in a version
- **Top-K Query** — top K most frequent words in a version
- **Difference Query** — frequency difference of a word between two versions

---

## Project Structure

```
240189_ArnavGupta.cpp   — C++ source code
240189_ArnavGupta.md    — This README
240189_ArnavGupta.pdf   — Assignment report
240189_ArnavGupta.jpg   — Execution screenshot
```

---

## Classes

| Class | Responsibility |
|---|---|
| `FileReader` | Abstract base class defining the `readChunk` interface |
| `BufferedFileReader` | Reads file in fixed-size KB chunks using `ifstream` |
| `Tokenizer` | Splits chunks into alphanumeric tokens; handles cross-boundary words via carry buffer |
| `VersionedIndexer` | Maintains per-version word frequency maps |
| `QueryProcessor` | Executes word, diff, and top-K queries against the index |

---

## Design Highlights

- **OOP with Polymorphism:** `FileReader` is an abstract base class with a pure virtual `readChunk()`. `BufferedFileReader` derives from it, enabling runtime polymorphism.
- **Template Utility:** A generic `toLowerCase<T>` function template normalizes strings for case-insensitive matching.
- **Carry Buffer:** The `Tokenizer` class uses a `carry` string to preserve partial tokens at chunk boundaries, ensuring correct counts across buffer splits.
- **Exception Handling:** File open errors and runtime exceptions are caught via `try/catch` blocks.

---

## Compilation

```bash
g++ -O2 240189_ArnavGupta.cpp -o analyzer
```

---

## Usage

### Word Count Query
```bash
./analyzer --file dataset_v1.txt --version v1 \
           --buffer 512 --query word --word error
```
**Sample Output:**
```
Version: v1
Word count: 142
Buffer size: 512 KB
Execution time: 0.38 seconds
```

---

### Top-K Query
```bash
./analyzer --file dataset_v1.txt --version v1 \
           --buffer 512 --query top --top 10
```
**Sample Output:**
```
Top-10 words in version v1:
error -> 142
info -> 98
debug -> 75
warning -> 60
...
Buffer size: 512 KB
Execution time: 0.41 seconds
```

---

### Difference Query
```bash
./analyzer --file1 dataset_v1.txt --version1 v1 \
           --file2 dataset_v2.txt --version2 v2 \
           --buffer 512 --query diff --word error
```
**Sample Output:**
```
Difference (v2 - v1): 27
Buffer size: 512 KB
Execution time: 0.72 seconds
```

---

## Command-Line Arguments

| Argument | Description |
|---|---|
| `--file <path>` | Input file for single-version queries |
| `--file1 <path>` | First file for diff query |
| `--file2 <path>` | Second file for diff query |
| `--version <name>` | Version label for single-version queries |
| `--version1 <name>` | Version label for first file |
| `--version2 <name>` | Version label for second file |
| `--buffer <kb>` | Buffer size in KB (256–1024) |
| `--query <type>` | `word` \| `diff` \| `top` |
| `--word <token>` | Target word for word/diff queries |
| `--top <k>` | Number of top results |

---

## Buffer & Memory

- Buffer size must be between **256 KB** and **1024 KB**.
- The tokenizer's carry buffer ensures words split across chunk boundaries are counted correctly.
- Memory usage grows only with the number of **unique words**, not file size.

---

## Assumptions

- Words are defined as contiguous sequences of alphanumeric characters (`[a-zA-Z0-9]`).
- All comparisons are case-insensitive (tokens are lowercased before indexing).
- Version names are arbitrary strings provided via CLI.
- For `--query word` and `--query top`, only `--file` and `--version` are required.
- For `--query diff`, `--file1`, `--file2`, `--version1`, and `--version2` are required.
