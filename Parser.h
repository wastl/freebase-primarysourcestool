#ifndef HAVE_PARSER_H_
#define HAVE_PARSER_H_

#include <iostream>

#include "Statement.h"

namespace Parser {

    void parseTSV(std::istream &in, std::function<void(Statement)> handler);

}  // namespace Parser

#endif