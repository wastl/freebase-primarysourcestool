#ifndef HAVE_STATEMENT_H
#define HAVE_STATEMENT_H 1

#include <string>

#include "Value.hpp"

enum ApprovalState {
    APPROVED, UNAPPROVED, OTHERSOURCE, WRONG, SKIPPED
};

class Statement {

public:

    Statement(long id, std::string &qid, std::string &property, Value const &value)
            : id(id), qid(qid), property(property), value(value), approved(UNAPPROVED) {
    }

    Statement(long id, std::string &qid, std::string &property, Value const &value, ApprovalState const &approved)
            : id(id), qid(qid), property(property), value(value), approved(approved) {
    }

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
    ApprovalState getApprovalState() const { return approved; };



private:

    long id;

    std::string qid, property;

    Value value;

    ApprovalState approved;

};

#endif