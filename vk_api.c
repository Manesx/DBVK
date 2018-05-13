#include <stdio.h>
#include <dbvk/vk_api.h>
#include <tls.h>
#include <unistd.h>

const char *host = "api.vk.com";

#define BUFFER_SIZE 1024

typedef struct {
	char is_connected;
    struct tls *tls_conn;
    struct tls_config *tls_conf;
} CONNECTION;


static char internal_buffer[BUFFER_SIZE] = {0};
static CONNECTION connection;

const char *GET_request= "GET /method/messages.getHistory?"
        "rev=%d&"
        "offset=%d&"
        "count=%d&"
        "user_id=%d&"
        "peer_id=%d&"
        "access_token=%s&"
        "v=%s HTTP/1.0\r\n"
        "Accept: text/html\r\n"
        "User-Agent: DBVK\r\n"
        "Connection: Close\r\n"
        "Host: %s\r\n\r\n";


size_t vk_api_send (string *data) {
	size_t ret = 0;
	if (connection.is_connected)
		ret = tls_write(connection.tls_conn, data->value, data->length);
	return ret;
}

size_t vk_api_recv (string *buffer) {
    ssize_t ret = 0;
    if (connection.is_connected) {
    	do {
	        buffer->length += ret = tls_read(connection.tls_conn, (void *) (buffer->value + buffer->length), 1024);
	    } while (ret != 0);
	    connection.is_connected = 0;
	}
    return buffer->length;
}

string vk_api_gen_uri (int rev, int offset, int count, int user_id, int peer_id, const char *access_token, const char *version) {
    int s = 0;
    if (internal_buffer[0])
        memset(internal_buffer, 0, BUFFER_SIZE);
    s = snprintf(internal_buffer, BUFFER_SIZE, GET_request, rev, offset, count, user_id, peer_id, access_token, version, host);
    string temp = {.value = internal_buffer, .size = BUFFER_SIZE, .length = (size_t) s};
    return temp;
}

void vk_api_init () {
	tls_init ();
}

int vk_api_connect () {
	int ret = -1;
    if (!connection.is_connected) {
    	vk_api_free ();
    	memset (&connection, 0, sizeof(CONNECTION));
    	if (!(connection.tls_conn = tls_client()))
	       	ret = VK_API_ERR_CLIENT;
	    if (!(connection.tls_conf = tls_config_new()))
	    	ret = VK_API_ERR_CONFIG;
	    tls_config_set_protocols(connection.tls_conf, TLS_PROTOCOL_TLSv1_2);
	    tls_configure(connection.tls_conn, connection.tls_conf);
	    tls_connect(connection.tls_conn, host, "443");
    	ret = tls_handshake(connection.tls_conn);
        if (!ret)
            connection.is_connected = 1;
        else 
        	vk_api_connect ();
    } else
        ret = 0;
    return ret;
}

int vk_api_close () {
	int ret = 0;
	if (connection.is_connected)
		ret = tls_close(connection.tls_conn);
	connection.is_connected = 0;
    return ret;
}

void vk_api_free () {
	if (connection.tls_conf)
    	tls_config_free(connection.tls_conf);
    if (connection.tls_conn)
    	tls_free(connection.tls_conn);
}

