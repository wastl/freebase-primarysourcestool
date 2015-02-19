// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#include "SourcesToolBackend.h"

#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "Parser.h"

std::string build_connection(
        const std::string& db_driver, const std::string& db_name,
        const std::string& db_host, const std::string& db_port,
        const std::string& db_user, const std::string& db_pass
) {
    std::ostringstream out;
    out << db_driver << ":database=" << db_name;
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


SourcesToolBackend::SourcesToolBackend(
        const std::string &db_driver, const std::string &db_name,
        const std::string &db_host, const std::string &db_port,
        const std::string &db_user, const std::string &db_pass) {

    connpool = cppdb::pool::create(build_connection(
            db_driver, db_name, db_host, db_port, db_user, db_pass));
}

std::vector<Statement> SourcesToolBackend::getStatementsByQID(std::string & qid, bool approved){
    if(!initialised) {
        init();
    }
    return statements[qid];
}

std::vector<Statement> SourcesToolBackend::getStatementsByTopic(std::string &topic, int count) {
    std::vector<Statement> stmts;

    return stmts;
}
