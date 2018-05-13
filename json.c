#include <json-c/json.h>
#include <stdio.h>
#include <dbvk/json.h>
#include <sqlite3.h>
#include <unistd.h>
#include <malloc.h>

#define BUFF_MAX_SIZE 10000

static char *buff = 0;
static int count = 0;

int clean (char * const str) {
	int i;
	for (i = 0; str[i]; i++)
		str[i] = '\0';
	return i;
}

char *json_format (const char *const str) {
	int str_pos, str_new_pos;
	char *new_str = buff + (BUFF_MAX_SIZE / 2);
		if (*new_str)
			clean (new_str);
		for (str_pos = 0, str_new_pos = 0; str[str_pos]; str_pos++, str_new_pos++) {
			if (str[str_pos] == '"') {
				new_str[str_new_pos] = '"';
				new_str[str_new_pos += 1] = '"';
			} else
				new_str[str_new_pos] = str[str_pos];
		}
	return new_str;
}

enum {
	TOKEN,
	RESPONSE,
	ARRAY_MESSAGES,
	MESSAGE,
	BODY,
	FROM_ID,
	DATE
};

int json_to_sqlite (string buffer, sqlite3 *db) {
	if (!buff)
		buff = (char*) malloc (BUFF_MAX_SIZE);
    int i;
    ssize_t array_size = 0, ret = 0;
    json_object *objects[7] = {0};
    if (buffer.value) {
        char *s = strstr(buffer.value, "\r\n\r\n") + 4;
        if (s)
            objects[TOKEN] = json_tokener_parse(s);
    }
    if (objects[TOKEN]) {
        objects[RESPONSE] = json_object_object_get(objects[TOKEN], "response");
        if (objects[RESPONSE]) {
        	ret = json_object_get_int(json_object_object_get(objects[RESPONSE], "count"));
        	objects[ARRAY_MESSAGES] = json_object_object_get(objects[RESPONSE], "items");
	        array_size = json_object_array_length(objects[ARRAY_MESSAGES]);
	        for (i = 0; i < array_size; i++) {
	        	count++;
	            objects[MESSAGE] = json_object_array_get_idx(objects[ARRAY_MESSAGES], i);
	            memset(buff, 0, BUFF_MAX_SIZE);
	            objects[BODY] = json_object_object_get(objects[MESSAGE], "body");
	            objects[FROM_ID] = json_object_object_get(objects[MESSAGE], "from_id");
	            objects[DATE] = json_object_object_get(objects[MESSAGE], "date");
	            sqlite3_snprintf(BUFF_MAX_SIZE, buff, "INSERT INTO Messages VALUES(%d, \"%s\", %d, %d);",
	                    count,
	                    json_format(json_object_get_string(objects[BODY])),
	                    json_object_get_int(objects[FROM_ID]),
	                    json_object_get_int(objects[DATE]));
	            if (sqlite3_exec(db, buff, 0, 0, 0) != SQLITE_OK)
	            	fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
	        }
        }
    }
    printf("Очищаем память\n");
    json_sqlite_free ((void**)objects, 0);
    return ret;
}

void json_sqlite_free(void **objects, int offset) {
	json_object **temp = (json_object**)objects;
	if (!objects) {
		free (buff);
		buff = 0;
	} else {
		for (; offset < 7; offset++) {
			free (temp[offset]);
			temp[offset] = 0;
		}
	}
}

