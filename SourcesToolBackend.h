// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#ifndef HAVE_SOURCESTOOL_BACKEND_H_
#define HAVE_SOURCESTOOL_BACKEND_H_

#include "Statement.h"
#include "SourcesToolBackend.cc"

#include <vector>
#include <map>
#include <string>

#include <cppdb/pool.h>

std::string build_connection(
        const std::string& db_driver, const std::string& db_name,
        const std::string& db_host, const std::string& db_port,
        const std::string& db_user, const std::string& db_pass
);

class SourcesToolBackend {
public:
    SourcesToolBackend(
            const std::string& db_driver, const std::string& db_name,
            const std::string& db_host, const std::string& db_port,
            const std::string& db_user, const std::string& db_pass
    );

    std::vector<Statement> getStatementsByQID(std::string &qid, bool approved);

    std::vector<Statement> getStatementsByTopic(std::string &topic, int count);

private:

    // CppDB SQL connection pool
    cppdb::pool::pointer connpool;

};


#endif // HAVE_SOURCESTOOL_BACKEND_H_