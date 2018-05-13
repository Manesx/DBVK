#include <dbvk/sqlite.h>
#include <sqlite3.h>
#include <string.h>
#include <stdio.h>


int sqlite_create (const char *filename, int text_size) {
	sqlite3 *db = 0;
	int ret = 0;
	char buff[512];
	memset (buff, 0, 512);
	snprintf (buff, 512, "CREATE TABLE Messages ("
    						"id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
    						"text CHAR (%d),"
    						"peer_id INT,"
    						"date DATE);", text_size);
	ret = sqlite_open_db (&db, filename, 0x00000004 | 0x00000002);
	if (!ret) {
		ret = sqlite_exec (db, buff, 0, 0);
		sqlite_close (db);
	}
	return ret;
}

int sqlite_open_db (sqlite3 **db, const char *filename, int flags) {
	return sqlite3_open_v2 (filename, db, flags, 0);
}

const char *sqlite_errmsg (sqlite3 *db) {
	return sqlite3_errmsg (db);
}

int sqlite_exec (sqlite3 *db, const char *sql, int (*callback)(void*,int,char**,char**), char **errmsg) {
	return sqlite3_exec (db, sql, callback, 0, errmsg);
}

int sqlite_close (sqlite3 *db) {
	return sqlite3_close (db);
}