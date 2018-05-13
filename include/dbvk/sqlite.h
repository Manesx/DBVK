
typedef struct sqlite3 sqlite3;

extern int sqlite_create (const char *filename, int text_size);
extern int sqlite_open_db (sqlite3 **db, const char *filename, int flags);
extern const char *sqlite_errmsg(sqlite3 *db);
extern int sqlite_exec(sqlite3 *db, const char *sql, int (*callback)(void*,int,char**,char**), char **errmsg);
extern int sqlite_close (sqlite3 *db);