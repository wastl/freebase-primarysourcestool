#ifndef HAVE_SERIALIZER_HPP
#define HAVE_SERIALIZER_HPP 1

#include "Statement.hpp"

#include <iosfwd>
#include <vector>

/**
* Interface for serializer implementations. A serializer has to implement at
* least the write function.
*/
class Serializer {

public:

    virtual void write(std::ostream &out) {};


    Serializer& operator<<(Statement&);

protected:

    std::vector<Statement> statements;
};



#endif
