# Gerp

**gerp** is a C++-based file search tool inspired by the Unix `grep` command.  
It recursively indexes and searches files within a given directory for specific words, supporting both case-sensitive and case-insensitive queries.

## Features

- **Efficient indexing and search** using a nested hash table structure (`insensitive → sensitive → occurrence count`)
- **Recursive directory traversal** implemented with an n-ary tree representation
- **Optimized file I/O** and **memory-efficient data storage** for large datasets
- **Robust string processing** to handle special characters and formatting

## Technologies & Concepts

- C++
- Custom data structure design
- String hashing
- File system traversal and management
- Algorithm optimization for large-scale text processing

## About This Project

This project significantly enhanced my skills in systems programming, data structures, and the design of scalable search tools.
