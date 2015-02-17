// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#ifndef HAVE_STATEMENT_H_
#define HAVE_STATEMENT_H_

#include <string>
#include <vector>

#include "Qualifier.h"
#include "Value.h"

enum ApprovalState {
    APPROVED, UNAPPROVED, OTHERSOURCE, WRONG, SKIPPED
};

/**
* Representation of a Freebase/Wikidata statement to be approved by users.
* Similar to RDF triples, but a simplified model without blank nodes. For each
* statement we also store an internal (database) ID that allows to uniquely
* identify the statement, and the current state of approval.
*/
class Statement {
 public:
    Statement(int64_t id, const std::string &qid, const std::string &property,
              const Value  &value, std::vector<Qualifier> &qualifiers)
            : id(id), qid(qid), property(property), value(value),
              qualifiers(qualifiers), approved(UNAPPROVED) { }

    Statement(int64_t id, const std::string &qid, const std::string &property,
              const Value &value, std::vector<Qualifier> &qualifiers,
              ApprovalState const &approved)
            : id(id), qid(qid), property(property), value(value),
              qualifiers(qualifiers), approved(approved) { }


    // default copy constructor and assignment operator
    Statement(const Statement& other) = default;
    Statement& operator=(const Statement& other) = default;


    /**
    * Return the (internal) database ID of the statement. Used for unique
    * identification purposes within the system.
    */
    int64_t getID() const { return id; }

    /**
    * Return the Wikidata QID (subject) of the statement. Used to identify the
    * subject concept of the statement
    */
    const std::string& getQID() const { return qid; }

    /**
    * Return the Wikidata property of the statement.
    */
    const std::string& getProperty() const { return property; }


    Value const &getValue() const { return value; }


    const std::vector<Qualifier> &getQualifiers() const {
        return qualifiers;
    }

/**
    * Return true if this statement has already been approved, false otherwise.
    */
    ApprovalState getApprovalState() const { return approved; }

 private:
    int64_t id;

    std::string qid, property;

    Value value;

    std::vector<Qualifier> qualifiers;

    ApprovalState approved;
};

#endif  // HAVE_STATEMENT_H_
