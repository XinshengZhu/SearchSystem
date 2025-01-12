/* DocPage.c */
#include "DocPage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Creates and initializes the document database
 * @return Database connection handle or NULL on error
 */
sqlite3* initDatabase() {
    sqlite3 *db;
    char *errMsg = 0;
    // Open database connection
    int rc = sqlite3_open("collection.db", &db);
    if (rc != SQLITE_OK) {
        printf("Cannot open database %s: %s\n", "collection.db", sqlite3_errmsg(db));
        return NULL;
    }
    // Create documents table
    const char *sql = "CREATE TABLE IF NOT EXISTS documents ("
                     "doc_id INTEGER PRIMARY KEY,"
                     "content TEXT NOT NULL);";
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error initializing database %s: SQL error: %s\n", "collection.db", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/**
 * Loads documents from TSV file into database
 * @param db Database connection
 * @return true if successful, false otherwise
 */
bool loadDocuments(sqlite3 *db) {
    FILE *file = fopen("collection.tsv", "r");
    if (!file) {
        printf("Error opening file %s\n", "collection.tsv");
        return false;
    }
    char *line = NULL;
    size_t len = 0;
    // Begin transaction for faster insertion
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    // Prepare insert statement
    sqlite3_stmt *stmt;
    const char *sql = "INSERT OR REPLACE INTO documents (doc_id, content) VALUES (?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    while (getline(&line, &len, file) != -1) {
        char *tab = strchr(line, '\t');
        if (tab) {
            *tab = '\0';  // Split at tab
            int docId = (int)strtol(line, NULL, 10);
            char *content = tab + 1;
            // Remove trailing newline
            content[strcspn(content, "\n")] = 0;
            // Bind parameters and execute
            sqlite3_bind_int(stmt, 1, docId);
            sqlite3_bind_text(stmt, 2, content, -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                printf("Error inserting document %d\n", docId);
                sqlite3_finalize(stmt);
                free(line);
                fclose(file);
                return false;
            }
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
        }
    }
    // Commit transaction
    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    sqlite3_finalize(stmt);
    free(line);
    fclose(file);
    printf("SQLite database %s created with documents in %s.\n", "collection.db", "collection.tsv");
    return true;
}

/**
 * Retrieves document content by docId from initialized database
 * @param docId Document ID to retrieve
 */
char *getDocumentByDocId(int docId) {
    sqlite3 *db;
    if (sqlite3_open("collection.db", &db) != SQLITE_OK) {
        printf("Cannot open database %s: %s\n", "collection.db", sqlite3_errmsg(db));
        return NULL;
    }
    sqlite3_stmt *stmt;
    const char *sql = "SELECT content FROM documents WHERE doc_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparing statement\n");
        return NULL;
    }
    sqlite3_bind_int(stmt, 1, docId);
    char *docContent = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *text = (char*)sqlite3_column_text(stmt, 0);
        docContent = strdup(text);
    }
    sqlite3_finalize(stmt);
    return docContent;
}
