// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <cppdb/frontend.h>

#include "Parser.h"
#include "SerializerTSV.h"
#include "Persistence.h"

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

        cppdb::session sql("sqlite3:db=fb.db");

        sql.begin();
        Persistence p(sql, true);

        int64_t count = 0;
        Parser::parseTSV(in, [&sql, &p, &count](Statement st)  {
            p.addStatement(st);
            count++;

            // batch commit
            if(count % 100000 == 0) {
                sql.commit();
                sql.begin();
            }
        });
        sql.commit();

        clock_t end = std::clock();
        std::cout << "injection time (" << count << " statements): "
                << 1000 * (static_cast<double>(end - begin) / CLOCKS_PER_SEC)
                << "ms" << std::endl;


        return 0;
    } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;

        return 1;
    }
}
