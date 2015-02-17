#include "Serializer.hpp"

Serializer& Serializer::operator<<(Statement & statement){
    statements.push_back(statement);
    return *this;
}