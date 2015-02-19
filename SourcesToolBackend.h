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

class SourcesToolBackend {
public:
    SourcesToolBackend(const cppcms::json::value& config);

    SourcesToolBackend(
            const std::string& db_driver, const std::string& db_name,
            const std::string& db_host, const std::string& db_port,
            const std::string& db_user, const std::string& db_pass
    );

    std::vector<Statement> getStatementsByQID(
            std::string &qid, bool unapprovedOnly);

    std::vector<Statement> getStatementsByTopic(std::string &topic, int count);

private:

    // CppDB uses a connection pool internally, so we just remember the
    // connection string
    std::string connstr;

};


#endif // HAVE_SOURCESTOOL_BACKEND_H_