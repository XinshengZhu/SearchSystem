/* DocPage.h */
#ifndef DOC_PAGE_H
#define DOC_PAGE_H

#include <sqlite3.h>
#include <stdbool.h>

/* Function prototypes */
sqlite3* initDatabase();    // Initialize sqlite3 database
bool loadDocuments(sqlite3 *db);    // Load documents from disk to sqlite3 database
char *getDocumentByDocId(int docId);    // Get document by docId from sqlite3 database

#endif
