// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#include "SourcesToolBackend.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "Parser.h"

void SourcesToolBackend::init() {

    std::ifstream file(db_path, std::ios_base::in | std::ios_base::binary);

    if(file.fail()) {
        std::cerr << "could not open data file" << std::endl;
        return;
    }

    boost::iostreams::filtering_istreambuf zin;
    zin.push(boost::iostreams::gzip_decompressor());
    zin.push(file);

    std::istream in(&zin);

    Parser::parseTSV(in, [this](Statement st){
        statements[st.getQID()]; // make sure vector is initialised
        statements[st.getQID()].push_back(st);
    });


    initialised = true;

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
