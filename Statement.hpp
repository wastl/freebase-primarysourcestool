#ifndef HAVE_STATEMENT_H
#define HAVE_STATEMENT_H 1

#include <string>

#include "Value.hpp"

class Statement {

public:



    /**
    * Return the (internal) database ID of the statement. Used for unique
    * identification purposes within the system.
    */
    long getID() const { return id; };

    /**
    * Return the Wikidata QID (subject) of the statement. Used to identify the
    * subject concept of the statement
    */
    const std::string& getQID() const { return qid; };

    /**
    * Return the Wikidata property of the statement.
    */
    const std::string& getProperty() const { return property; };


    const Value& getValue() const { return value; };


    /**
    * Return true if this statement has already been approved, false otherwise.
    */
    bool isApproved() const { return approved; };



private:

    long id;

    std::string qid, property;

    Value value;

    bool approved;

};

#endif