#ifndef USERS_H
#define USERS_H
#include <stdbool.h>

#include "pgclient.h"

typedef struct User {
  int id;
  char *created_at;
  char *updated_at;
  char *username;
  char *first_name;
  char *last_name;
  char *email;
  char *password;
  bool is_superuser;
  char *last_login;
  bool active;
} User;

// Extract users from the PGResult.
void users_populate_user_array(Result *sess, User users[]);

// print user to stdout
void users_print_user(User user);

#endif  // USERS_H
