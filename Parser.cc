#include "Parser.h"

#include <regex>
#include <string>
#include <algorithm>    // copy

#include <boost/tokenizer.hpp>


PropertyValue parsePropertyValue(std::string property, value) {
    // regular expressions for the different value formats

    // QID of entities, properties, sources
    static std::regex re_entity("[QPS]\d*");

    // time format (e.g. +00000001967-01-17T00:00:00Z/11)
    static std::regex re_time("\\+\d{11}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z/\d{2}");

    // numbers (+/-1234.4567)
    static std::regex re_num("[+-]\d+(\\.\d+)?");

    // locations (@LAT/LONG)
    static std::regex re_loc("@\d{2}(\\.\d+)?/\d{3}(\\.\d+)?");

    if (std::regex_match(value, re_entity)) {

    }
}

void parseTSV(std::istream &in, std::function<void(Statement)> handler) {
    typedef boost::tokenizer< boost::escaped_list_error<char> > Tokenizer;
    boost::escaped_list_error<char> sep("\\"," \t","\"");

    std::vector< std::string > vec;
    std::string line;

    long id = 0;
    while (getline(in,line))
    {
        Tokenizer tok(line, sep);
        vec.assign(tok.begin(),tok.end());

        Statement::extensions_t qualifiers, sources;

        // 0, 1, 2 are subject, predicate and object; everything that follows
        // is a qualifier or source, depending on first letter of predicate.
        for(int i=3; i<vec.size(); i += 2) {
            PropertyValue p()

            if (vec[i][0] == 'S') {
                sources.push_back(p);
            } else {
                qualifiers.push_back(p);
            }
        }

        handler(Statement(-1, vec[0], PropertyValue(vec[1],vec[2]), qualifiers, sources, UNAPPROVED));
    }

}
