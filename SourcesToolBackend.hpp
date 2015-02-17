#ifndef HAVE_SOURCESTOOL_BACKEND_HPP
#define HAVE_SOURCESTOOL_BACKEND_HPP 1

class SourcesToolBackend {

public:

    std::vector<Statement> getStatementsByQID(std::string &qid, bool approved);

    std::vector<Statement> getStatementsByTopic(std::string &topic, int count);

};


#endif