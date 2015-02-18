#include "Parser.h"

#include <string>
#include <algorithm>    // copy

#include <boost/tokenizer.hpp>


void parseTSV(std::istream &in, std::function<void(Statement)> handler) {
    typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;
    boost::char_separator<char> sep(" \t");

    std::vector< std::string > vec;
    std::string line;

    long id = 0;
    while (getline(in,line))
    {
        Tokenizer tok(line, sep);
        vec.assign(tok.begin(),tok.end());


        Statement stmt(id++,vec[0], vec[1], Value(vec[2]), qualifiers);
        statements[vec[0]];
        statements[vec[0]].push_back(stmt);
    }


}
