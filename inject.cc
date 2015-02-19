// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "Parser.h"
#include "SerializerTSV.h"

int main(int argc, char **argv) {
    try {

        std::ifstream file(argv[1], std::ios_base::in | std::ios_base::binary);

        if(file.fail()) {
            std::cerr << "could not open data file" << std::endl;
            return 1;
        }

        boost::iostreams::filtering_istreambuf zin;
        zin.push(boost::iostreams::gzip_decompressor());
        zin.push(file);

        std::istream in(&zin);

        clock_t begin = std::clock();

        std::vector<Statement> statements;
        Parser::parseTSV(in, [&statements](Statement st)  {
            statements.push_back(st);
        });

        clock_t end = std::clock();
        std::cout << "parsing time (" << statements.size() << " statements): "
                << 1000 * (static_cast<double>(end - begin) / CLOCKS_PER_SEC)
                << "ms" << std::endl;

        Serializer::writeTSV(statements.cbegin(), statements.cend(), std::cout);

        clock_t endp = std::clock();
        std::cout << "printing time: "
                << 1000 * (static_cast<double>(endp - end) / CLOCKS_PER_SEC)
                << "ms" << std::endl;

        return 0;
    } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;

        return 1;
    }
}
