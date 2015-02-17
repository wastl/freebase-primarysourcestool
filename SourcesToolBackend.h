// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#ifndef HAVE_SOURCESTOOL_BACKEND_H_
#define HAVE_SOURCESTOOL_BACKEND_H_

#include "Statement.h"

#include <vector>
#include <map>
#include <string>

class SourcesToolBackend {
public:
    explicit SourcesToolBackend(const std::string& db_path)
            : db_path(db_path), initialised(false) {}

    std::vector<Statement> getStatementsByQID(std::string &qid, bool approved);

    std::vector<Statement> getStatementsByTopic(std::string &topic, int count);

private:
    void init();

    std::map<std::string,std::vector<Statement>> statements;
    std::string db_path;
    bool initialised;
};


#endif // HAVE_SOURCESTOOL_BACKEND_H_