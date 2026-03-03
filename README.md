# CS253 Assignment 1

## Memory-Efficient Versioned File Indexer Using C++

---

## README.md

### Student Details

* **Name:** Arnav Gupta
* **Roll Number:** 240189
* **Course:** CS253 – Software Development and Operations

---

### Problem Overview

This project implements a **memory-efficient, versioned word-level file indexer** in C++. The system is designed to process very large text files incrementally using a fixed-size buffer, without ever loading the entire file into memory. For each file (called a *version*), the program builds a word-frequency index and supports multiple analytical queries.

---

### Key Features

* Incremental file processing using a fixed-size buffer (256 KB – 1024 KB)
* Case-insensitive word indexing
* Correct handling of tokens split across buffer boundaries
* Support for multiple file versions in a single execution
* Query support:

  * Word count query
  * Difference query between two versions
  * Top-K most frequent words query
* Object-Oriented Design with clear separation of concerns

---

### Design Overview

The program is structured using the following main classes:

1. **FileReader (Abstract Base Class)**
   Defines a common interface for buffered file reading.

2. **BufferedFileReader**
   Reads a file incrementally using a fixed-size buffer and implements the `FileReader` interface.

3. **Tokenizer**
   Converts raw text into alphanumeric tokens (words) and correctly handles words split across buffer boundaries.

4. **VersionedIndexer**
   Maintains a mapping from version name → (word → frequency). Each version is indexed independently.

5. **QueryProcessor**
   Executes word, difference, and Top-K queries on the indexed data.

---

### Compilation

```bash
g++ 240189_Arnav.cpp -o analyzer
```

---

### Usage Examples

#### Word Count Query

```bash
#Enter the dataset file's path in the "data.txt" space.
./analyzer --file data.txt --version v1 --buffer 512 --query word --word error
```

#### Top-K Query

```bash
#Enter the dataset file's path in the "data.txt" space.
./analyzer --file data.txt --version v1 --buffer 512 --query top --top 10
```

#### Difference Query

```bash
#Enter the dataset files' paths in the "data1.txt" and "data2.txt" spaces respectively.
./analyzer --file1 data1.txt --version1 v1 \
           --file2 data2.txt --version2 v2 \
           --buffer 512 --query diff --word error
```

---

### Output

The program prints:

* Version name(s)
* Query result
* Allocated buffer size (in KB)
* Total execution time (in seconds)

---

### Notes

* Memory usage depends only on the number of unique words, not file size.
* The dataset files are used as-is and are not modified.

---

## Assignment Report

### Introduction

Processing large text files efficiently is a common requirement in real-world systems such as log analyzers and search engines. Loading entire files into memory can lead to excessive memory consumption and poor scalability. This assignment focuses on building a memory-efficient system that processes large files incrementally while still supporting meaningful analytical queries.

---

### Objective

The objective of this assignment is to design and implement a versioned word-level indexer in C++ that:

* Processes large files using a fixed-size buffer
* Uses object-oriented design principles
* Demonstrates inheritance, polymorphism, templates, and exception handling
* Supports multiple queries over indexed data

---

### Word-Level Index

A word-level index is a data structure that maps each unique word in a file to the number of times it appears. Words are defined as contiguous sequences of alphanumeric characters, and indexing is performed in a case-insensitive manner.

Example:

```
error debug error info
```

Produces the index:

```
error → 2
debug → 1
info → 1
```

---

### Buffer-Based File Processing

The file is read incrementally using a fixed-size buffer (between 256 KB and 1024 KB). This ensures that:

* The entire file is never loaded into memory
* Memory usage remains independent of file size

Special care is taken to handle words that are split across buffer boundaries by carrying partial tokens between reads.

---

### Version Management

Each indexed file is associated with a user-defined version name. The system supports multiple versions within a single execution, allowing comparisons between different files through difference queries.

---

### Supported Queries

1. **Word Count Query**
   Returns the frequency of a given word in a specified version.

2. **Difference Query**
   Computes the difference in frequency of a word between two versions.

3. **Top-K Query**
   Returns the K most frequent words in a version, sorted in descending order of frequency.

---

### Object-Oriented Design and C++ Features

The implementation satisfies all specified C++ requirements:

* **Inheritance:** `BufferedFileReader` inherits from the abstract base class `FileReader`
* **Runtime Polymorphism:** File reading is performed via virtual function calls
* **Function Overloading:** Multiple query-handling functions are implemented
* **Templates:** A utility template function is used for case normalization
* **Exception Handling:** File I/O errors are handled using `try-catch` blocks

---

### Performance and Memory Analysis

* Memory usage grows only with the number of unique words
* File size has no impact on memory consumption
* Execution time scales linearly with file size

---

### Conclusion

This assignment demonstrates how careful object-oriented design and disciplined memory management can be used to build scalable systems in C++. The final implementation efficiently processes large files, supports multiple analytical queries, and adheres strictly to the given constraints and requirements.

---

### Execution Screenshot

*(Attach execution screenshot as required in the submission)*
