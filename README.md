# README: A Small Optimized Search System

## Overview
This project is written and tested entirely on macOS Sequoia throughout the development process.
We also simply test the project on a Ubuntu docker container to ensure the compatibility.
It may not work on other operating systems without modifications.

There are three main stages in this project:
- Data Parsing: Converts raw documents into intermediate format and creates a page table with SQLite
- Index Building: Creates compressed inverted index structure by merging intermediate files and generates a lexicon
- Query Processing: Handles conjunctive and disjunctive searches based on queries and retrieves relevant documents

## Files
Here's a brief description of all code files in the root directory (`SearchSystem/`) of the project:

```
SearchSystem/
├─── DataParser/
│    ├─── DataParser.c/h         # Handles parsing of raw document files into intermediate format for indexing and page table creation
│    ├─── DocPage.c/h            # Manages document page table storage and tracking with SQLite
│    └─── HashTable.c/h          # Implements hash table structure for storing words and their document occurrences
│
├─── IndexBuilder/
│    ├─── Compression.c/h        # Implements compression algorithms for document IDs and impact scores
│    ├─── IndexBuilder.c/h       # Handles main index construction functionality and lexicon generation
│    ├─── InvertedIndex.c/h      # Implements core inverted index data structure
│    ├─── Lexicon.c/h            # Manages dictionary of words and their chunk locations
│    ├─── MergeHeap.c/h          # Implements heap structure for merging multiple intermediate files
│    └─── Utils.c/h              # Implements utility functions for document processing and BM25 scoring
│
├─── QueryProcessor/
│    ├─── Decompression.c/h      # Handles decompression of document IDs and impact scores
│    ├─── InvertedList.c/h       # Manages posting lists during query processing
│    ├─── LexiconTable.c/h       # Implements hash table for fast term lookup in lexicon
│    ├─── QueryHeap.c/h          # Implements heap structure for maintaining top-K query results
│    └─── QueryProcessor.c/h     # Handles main query processing and document retrieval functionality
│
└─── CMakeLists.txt              # CMake build configuration file
```

## Requirements
- The `sqlite` library is required to compile the project. Install it using the following command:

  ```bash
  # macOS
  $ brew install sqlite
  ```
  ```bash
  # Ubuntu
  $ sudo apt update
  $ sudo apt install libsqlite3-dev
  ```
- The `cmake` build system is required to compile the project. Install it using the following command:

  ```bash
  # macOS
  $ brew install cmake
  ```
  ```bash
  # Ubuntu
  $ sudo apt update
  $ sudo apt install cmake
  ```

- [Passage Retrieval Dataset](https://msmarco.z22.web.core.windows.net/msmarcoranking/collection.tar.gz) is required to be downloaded and extracted in the root dictionary (`SearchSystem/`) of the project necessarily using the following commands:

  ```bash
  # macOS
  $ curl -O https://msmarco.z22.web.core.windows.net/msmarcoranking/collection.tar.gz
  $ tar -xzvf collection.tar.gz
  ```
  ```bash
  # Ubuntu
  $ wget https://msmarco.z22.web.core.windows.net/msmarcoranking/collection.tar.gz
  $ tar -xzvf collection.tar.gz
  ```

## Compilation
* Move to the root directory of the project

  ```bash
  $ cd SearchSystem
  ```

* Create a build directory and move into it

  ```bash
  $ mkdir build && cd build
  ```

* Move the downloaded and extracted `collection.tsv` file from the root dictionary to the `build` directory

  ```bash
  $ mv ../collection.tsv .
  ```

* Run CMake to generate the build files

  ```bash
  $ cmake ..
  ```

* Build the project using the generated build files

  ```bash
  $ cmake --build .
  ```
## Execution
It should be noted that all three executables have no input parameters and must be run in the order specified above to ensure proper functionality.

* Run the DataParser executables first in the `build` directory

  ```bash
  $ ./DataParser
  ```

* Run the IndexBuilder executables next in the `build` directory

  ```bash
  $ ./IndexBuilder
  ```
  
* Run the QueryProcessor executables last in the `build` directory
    
  ```bash
  $ ./QueryProcessor
  ```

When printing the content of original documents, if you are using the terminal in VSCode, you may encounter the issue that the terminal cannot display the content properly.
This is because the terminal in VSCode does not support the display of some special characters (unprintable ASCII).
In this case, you can try another IDE like Clion or use the terminal in your own system to run the executables.
