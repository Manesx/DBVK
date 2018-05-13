#ifndef _STRING_H_
  #define _STRING_H_
    #include <dbvk/string.h>
  #endif

#define VK_API_OK 0
#define VK_API_ERR_CLIENT -1
#define VK_API_ERR_CONFIG -2
#define VK_API_NOT_CONNECTED -3

extern void vk_api_init ();
extern size_t vk_api_send (string *data);
extern size_t vk_api_recv (string *buffer);
extern int vk_api_connect ();
extern string vk_api_gen_uri (int rev, int offset, int count, int user_id, int peer_id, const char *access_token, const char *version);
extern void vk_api_free ();
extern int vk_api_close ();
