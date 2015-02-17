#ifndef HAVE_STATEMENT_H_
#define HAVE_STATEMENT_H_

#include <string>

#include "Value.h"

enum ApprovalState {
    APPROVED, UNAPPROVED, OTHERSOURCE, WRONG, SKIPPED
};

class Statement {
 public:
    Statement(int64_t id, const std::string &qid, const std::string &property,
              const Value  &value)
            : id(id), qid(qid), property(property), value(value),
              approved(UNAPPROVED) { }

    Statement(int64_t id, const std::string &qid, const std::string &property,
              const Value &value, ApprovalState const &approved)
            : id(id), qid(qid), property(property), value(value),
              approved(approved) { }


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


    const Value& getValue() const { return value; }


    /**
    * Return true if this statement has already been approved, false otherwise.
    */
    ApprovalState getApprovalState() const { return approved; }

 private:
    int64_t id;

    std::string qid, property;

    Value value;

    ApprovalState approved;
};

#endif  // HAVE_STATEMENT_H_
