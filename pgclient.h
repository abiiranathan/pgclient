#ifndef PGCLIENT_H
#define PGCLIENT_H

#include <libpq-fe.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct PGClient PGClient;

// Wraps the Query Results.
// Must be explicitly freed after every query to avoid memory leaks.
// Call pg_session_free to free the session.
typedef struct Result {
  PGresult *res;
  char *error;
  int count;
} Result;

// Connect to postgres database.
PGClient *pg_connect(const char *conninfo, char *error);

// Free memory pointed to by client.
void pg_free(PGClient *client);

// Returns the last error message from queries.
char *pg_last_error(PGClient *client);

// Free memory pointed to by sess.
void pg_result_free(Result *r);

// Populates headers with the string headers for the results-set in session.
void pg_column_headers(Result *r, char *headers[]);

// Run a query.
// Query results are wrapped in a Session
// If the query fails, check the last_error
Result *pg_query(PGClient *client, const char *query);

// Execute a query as a prepared statement.
// stmt is the query string with placeholders like $1, $2
// nParams is the number of placeHolders or parameters to replace.
// params is an array of parameters.
Result *pg_query_stmt(PGClient *client, const char *stmt, int nParams, const char params[]);

// Excute a command.
// Returns true if the command succeeded or false if it failed.
bool pg_exec(PGClient *client, const char *command);

// Execute n commands in a transaction.
// If any command fails, the transaction is rolled-back.
bool pg_exec_transaction(PGClient *client, const char *commands[], int n);

// Pretty-Print data in a session in a formatted table (like in psql)
void pg_pretty_print(Result *r);

#endif  // PGCLIENT_H
