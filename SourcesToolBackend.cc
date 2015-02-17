#include "SourcesToolBackend.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>    // copy

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/tokenizer.hpp>

std::vector<Statement> SourcesToolBackend::getStatementsByQID(std::string & qid, bool approved){
    std::vector<Statement> stmts;

    std::ifstream file("../data/p27.txt.gz", std::ios_base::in | std::ios_base::binary);

    if(file.fail()) {
        std::cerr << "could not open data file" << std::endl;
        return stmts;
    }


    boost::iostreams::filtering_istreambuf zin;
    zin.push(boost::iostreams::gzip_decompressor());
    zin.push(file);

    std::istream in(&zin);


    typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;
    boost::char_separator<char> sep(" \t");

    std::vector< std::string > vec;
    std::string line;

    long id = 0;
    while (getline(in,line))
    {
        Tokenizer tok(line, sep);
        vec.assign(tok.begin(),tok.end());

        id++;
        if(vec[0] == qid) {
            Statement stmt(id,vec[0], vec[1], Value(vec[2]));
            stmts.push_back(stmt);
        }
    }

    std::cout << "scanned " << id << " lines " << std::endl;

    return stmts;
}

std::vector<Statement> SourcesToolBackend::getStatementsByTopic(std::string &topic, int count) {
    std::vector<Statement> stmts;

    return stmts;
}
