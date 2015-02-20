// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#ifndef HAVE_SOURCESTOOL_BACKEND_H_
#define HAVE_SOURCESTOOL_BACKEND_H_

#include "Statement.h"

#include <vector>
#include <map>
#include <string>

#include <cppcms/json.h>
#include <cppdb/frontend.h>

std::string build_connection(
        const std::string& db_driver, const std::string& db_name,
        const std::string& db_host, const std::string& db_port,
        const std::string& db_user, const std::string& db_pass
);

/**
* Database backend for accessing statements and entities. Manages the
* SQL connection details and wraps the Persistence implementation.
*/
class SourcesToolBackend {
public:
    SourcesToolBackend(const cppcms::json::value& config);

    SourcesToolBackend(
            const std::string& db_driver, const std::string& db_name,
            const std::string& db_host, const std::string& db_port,
            const std::string& db_user, const std::string& db_pass
    );

    /**
    * Return a statement by ID. Throws PersistenceException if not found.
    */
    Statement getStatementByID(int64_t id);

    /**
    * Return a list of statements for the given entity ID. If unapprovedOnly
    * is set, return only statements with state "unapproved".
    */
    std::vector<Statement> getStatementsByQID(std::string &qid, bool unapprovedOnly);

    /**
    * Return a list of count random statements relating to a topic.
    */
    std::vector<Statement> getStatementsByTopic(std::string &topic, int count);

    /**
    * Update the approval state of the statement with the given ID.
    */
    void updateStatement(int64_t id, ApprovalState state, std::string user);

private:

    // CppDB uses a connection pool internally, so we just remember the
    // connection string
    std::string connstr;

};


#endif // HAVE_SOURCESTOOL_BACKEND_H_