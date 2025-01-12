cmake_minimum_required(VERSION 3.10)
project(SearchSystem C)

set(CMAKE_C_STANDARD 11)

find_package(SQLite3 REQUIRED)

add_executable(DataParser DataParser/DataParser.c
        DataParser/DataParser.h
        DataParser/HashTable.c
        DataParser/HashTable.h
        DataParser/DocPage.c
        DataParser/DocPage.h)

target_link_libraries(DataParser PRIVATE SQLite::SQLite3)

add_executable(IndexBuilder IndexBuilder/IndexBuilder.c
        IndexBuilder/IndexBuilder.h
        IndexBuilder/MergeHeap.c
        IndexBuilder/MergeHeap.h
        IndexBuilder/InvertedIndex.c
        IndexBuilder/InvertedIndex.h
        IndexBuilder/Lexicon.c
        IndexBuilder/Lexicon.h
        IndexBuilder/Compression.c
        IndexBuilder/Compression.h
        IndexBuilder/Utils.c
        IndexBuilder/Utils.h)

target_link_libraries(IndexBuilder PRIVATE m)

add_executable(QueryProcessor QueryProcessor/QueryProcessor.c
        QueryProcessor/QueryProcessor.h
        QueryProcessor/LexiconTable.c
        QueryProcessor/LexiconTable.h
        QueryProcessor/InvertedList.c
        QueryProcessor/InvertedList.h
        QueryProcessor/Decompression.c
        QueryProcessor/Decompression.h
        QueryProcessor/QueryHeap.c
        QueryProcessor/QueryHeap.h
        DataParser/DocPage.c
        DataParser/DocPage.h)

target_link_libraries(QueryProcessor PRIVATE SQLite::SQLite3 m)
