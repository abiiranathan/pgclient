#include "users.h"

#include <stdlib.h>
#include <string.h>

// helper function to extract a single user from PGResult
static void users_populate_user(PGresult *res, int tuple_num, User *user) {
  user->id = atoi(PQgetvalue(res, tuple_num, 0));
  user->created_at = PQgetvalue(res, tuple_num, 1);
  user->updated_at = PQgetvalue(res, tuple_num, 2);
  user->username = PQgetvalue(res, tuple_num, 3);
  user->first_name = PQgetvalue(res, tuple_num, 4);
  user->last_name = PQgetvalue(res, tuple_num, 5);
  user->email = PQgetvalue(res, tuple_num, 6);
  user->password = PQgetvalue(res, tuple_num, 7);
  user->is_superuser = strncmp(PQgetvalue(res, tuple_num, 8), "t", 1) == 0 ? true : false;
  user->last_login = PQgetvalue(res, tuple_num, 9);
  user->active = strncmp(PQgetvalue(res, tuple_num, 10), "t", 1) == 0 ? true : false;
}

void users_populate_user_array(Result *sess, User users[]) {
  for (int i = 0; i < sess->count; i++) {
    users_populate_user(sess->res, i, &users[i]);
  }
}

void users_print_user(User user) {
  printf("ID: %d\n", user.id);
  printf("CreatedAt: %s\n", user.created_at);
  printf("UpdatedAt: %s\n", user.updated_at);
  printf("Username: %s\n", user.username);
  printf("FirstName: %s\n", user.first_name);
  printf("LastName: %s\n", user.last_name);
  printf("Email: %s\n", user.email);
  printf("Password: %s\n", user.password);
  printf("IsSuperuser: %s\n", user.is_superuser ? "true" : "false");
  printf("LastLogin: %s\n", user.last_login);
  printf("Active: %s\n", user.active ? "true" : "false");
}
