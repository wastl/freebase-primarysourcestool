#ifndef HAVE_PARSER_H_
#define HAVE_PARSER_H_

#include <iostream>

#include "Statement.h"


void parseTSV(std::istream &in, std::function<void(Statement)> handler);


#endif