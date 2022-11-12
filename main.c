#include <stdio.h>
#include <stdlib.h>

#include "pgclient.h"
#include "users.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s dsn\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char *conninfo = argv[1];
  char error[256];
  PGClient *client = pg_connect(conninfo, error);
  if (client == NULL) {
    fprintf(stderr, "Connection error: %s\n", error);
    exit(EXIT_FAILURE);
  }

  Result *sess = pg_query(client, "SELECT * FROM users");
  if (sess == NULL) {
    printf("%s\n", pg_last_error(client));
    exit(EXIT_FAILURE);
  }

  User users[sess->count];
  users_populate_user_array(sess, users);
  users_print_user(users[0]);
  pg_pretty_print(sess);

  // Free resources
  pg_result_free(sess);
  pg_free(client);

  return EXIT_SUCCESS;
}
