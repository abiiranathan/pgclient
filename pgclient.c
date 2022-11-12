#include "pgclient.h"

#include <string.h>

typedef struct PGClient {
  PGconn *conn;        // PGConn is a connection to postgres
  char *last_error;    // latest error message
  bool query_success;  // Status of last query
} PGClient;

PGClient *pg_connect(const char *conninfo, char *error) {
  PGconn *conn = PQconnectdb(conninfo);
  if (PQstatus(conn) == CONNECTION_BAD) {
    PQfinish(conn);
    char *err = PQerrorMessage(conn);
    strncpy(error, err, strlen(err) + 1);  // NOLINT
    return NULL;
  }

  PGClient *client = malloc(sizeof(PGClient));
  client->conn = conn;
  return client;
}

void pg_free(PGClient *client) {
  PQfinish(client->conn);
  client->conn = NULL;
  free(client);
  client = NULL;
}

char *pg_last_error(PGClient *client) { return client->last_error; }

void pg_result_free(Result *r) {
  PQclear(r->res);
  r->res = NULL;

  free(r);
  r = NULL;
}

void pg_column_headers(Result *r, char *headers[]) {
  int nCols = PQnfields(r->res);
  for (int i = 0; i < nCols; i++) {
    headers[i] = PQfname(r->res, i);
  }
}

Result *pg_query(PGClient *client, const char *query) {
  client->last_error = "";
  client->query_success = false;

  PGresult *res = PQexec(client->conn, query);
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    client->last_error = PQerrorMessage(client->conn);
    return NULL;
  }

  // allocate memory to hold the results
  Result *r = malloc(sizeof(Result));
  if (r == NULL) {
    return NULL;
  }

  r->res = res;
  r->count = PQntuples(res);
  client->query_success = true;
  return r;
}

Result *pg_query_stmt(PGClient *client, const char *stmt, int nParams, const char params[]) {
  client->last_error = "";
  client->query_success = false;

  PGresult *res = PQexecParams(client->conn, stmt, nParams, NULL, &params, NULL, NULL, 0);
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    client->last_error = PQerrorMessage(client->conn);
    return NULL;
  }

  // allocate memory to hold the results
  Result *r = malloc(sizeof(Result));
  if (r == NULL) {
    return NULL;
  }

  r->res = res;
  r->count = PQntuples(res);
  client->query_success = true;
  return r;
}

bool pg_exec(PGClient *client, const char *command) {
  client->last_error = "";
  client->query_success = false;

  PGresult *res = PQexec(client->conn, command);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    client->last_error = PQerrorMessage(client->conn);
    PQclear(res);
    return false;
  }

  PQclear(res);
  return true;
}

bool pg_exec_transaction(PGClient *client, const char *commands[], int n) {
  bool failed = false;

  PGresult *res = PQexec(client->conn, "BEGIN");
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    PQclear(res);
    printf("BEGIN command failed\n");
    return false;
  }

  PQclear(res);

  // Run the command in order
  for (int i = 0; i < n; i++) {
    if (!pg_exec(client, commands[i])) {
      failed = true;
      break;
    }
  }

  // Check status of executed commands
  if (!failed) {
    res = PQexec(client->conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
      PQclear(res);
      printf("COMMIT command failed\n");
      return false;
    }
  } else {
    printf("%s\n", client->last_error);
  }
  return !failed;
}

void pg_pretty_print(Result *r) { PQdisplayTuples(r->res, stdout, 1, "|", 1, false); }
