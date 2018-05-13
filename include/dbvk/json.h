#ifndef _STRING_H_
  #define _STRING_H_
    #include <dbvk/string.h>
  #endif

typedef struct sqlite3 sqlite3;

extern int json_to_sqlite (string buffer, sqlite3 *db);
extern void json_sqlite_free(void **objects, int offset);
