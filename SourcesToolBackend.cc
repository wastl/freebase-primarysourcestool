// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#include "SourcesToolBackend.h"

#include <sstream>
#include <iostream>

#include "Persistence.h"

std::string build_connection(
        const std::string& db_driver, const std::string& db_name,
        const std::string& db_host, const std::string& db_port,
        const std::string& db_user, const std::string& db_pass
) {
    std::ostringstream out;
    out << db_driver << ":db=" << db_name;
    if (db_host != "") {
        out << ";host=" << db_host;
    }
    if (db_port != "") {
        out << ";port=" << db_port;
    }
    if (db_user != "") {
        out << ";user=" << db_user;
    }
    if (db_pass != "") {
        out << ";pass=" << db_pass;
    }
    return out.str();
}

SourcesToolBackend::SourcesToolBackend(const cppcms::json::value& config) {
    connstr = build_connection(
            config["driver"].str(), config["name"].str(), config["host"].str(),
            config["port"].str(), config["user"].str(), config["password"].str());
}

SourcesToolBackend::SourcesToolBackend(
        const std::string &db_driver, const std::string &db_name,
        const std::string &db_host, const std::string &db_port,
        const std::string &db_user, const std::string &db_pass) {

    connstr = build_connection(
            db_driver, db_name, db_host, db_port, db_user, db_pass);
}

std::vector<Statement> SourcesToolBackend::getStatementsByQID(
        std::string &qid, bool unapprovedOnly){
    cppdb::session sql(connstr); // released when sql is destroyed

    Persistence p(sql);
    return p.getStatementsByQID(qid, unapprovedOnly);

}

std::vector<Statement> SourcesToolBackend::getRandomStatements(int count, bool unapprovedOnly) {
    cppdb::session sql(connstr); // released when sql is destroyed

    Persistence p(sql);
    return p.getRandomStatements(count, unapprovedOnly);
}

Statement SourcesToolBackend::getStatementByID(int64_t id) {
    cppdb::session sql(connstr); // released when sql is destroyed

    Persistence p(sql);
    return p.getStatement(id);
}

void SourcesToolBackend::updateStatement(int64_t id, ApprovalState state, std::string user) {
    cppdb::session sql(connstr); // released when sql is destroyed

    Persistence p(sql);
    p.updateStatement(id, state);

    // TODO: add user information about approval
}

std::vector<Statement> SourcesToolBackend::getStatementsByRandomQID(bool unapprovedOnly) {
    cppdb::session sql(connstr); // released when sql is destroyed

    Persistence p(sql);
    std::string qid = p.getRandomQID(unapprovedOnly);

    return p.getStatementsByQID(qid, unapprovedOnly);
}
