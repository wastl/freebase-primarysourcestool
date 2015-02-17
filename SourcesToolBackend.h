#ifndef HAVE_SOURCESTOOL_BACKEND_H_
#define HAVE_SOURCESTOOL_BACKEND_H_

#include "Statement.h"

#include <vector>
#include <string>

class SourcesToolBackend {

public:

    std::vector<Statement> getStatementsByQID(std::string &qid, bool approved);

    std::vector<Statement> getStatementsByTopic(std::string &topic, int count);

};


#endif // HAVE_SOURCESTOOL_BACKEND_H_