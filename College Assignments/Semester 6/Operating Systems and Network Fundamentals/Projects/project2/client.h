#ifndef __client_h__
#define __client_h__

#include <stdbool.h>

#include "server.h"

bool get_record (char *, char *, char *, ids_resp_t **);
bool check_record (char *, ids_resp_t *);

// Additional Methods
pid_t create_cksum_child (int *pipe, const char *filename);
char *get_cksum (const char *filename);
char *split_string (char *buffer);

#endif
