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

        cppdb::statement sql_add_snak_item=sql.prepare("INSERT INTO snak(property,svalue,vtype) VALUES (?,?,'item')");
        cppdb::statement sql_add_snak_string=sql.prepare("INSERT INTO snak(property,svalue,lang,vtype) VALUES (?,?,?,'string')");
        cppdb::statement sql_add_snak_quantity=sql.prepare("INSERT INTO snak(property,dvalue,vtype) VALUES (?,?,'quantity')");
        cppdb::statement sql_add_stmt=sql.prepare("INSERT INTO statement(subject,mainsnak) VALUES (?,?)");

        int64_t count = 0;
        Parser::parseTSV(in, [&sql, &count](Statement st)  {

            count++;
        });

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
