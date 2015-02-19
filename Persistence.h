#ifndef HAVE_PERSISTENCE_H_
#define HAVE_PERSISTENCE_H_

#include <cppdb/frontend.h>
#include <elf.h>
#include <fcntl.h>

#include "Statement.h"

/**
* Abstraction class for database persistence. Offers low-level methods for
* querying and updating statements in a given CppDB SQL session.
*
* Implements a certain level of caching using the cppcms caching functionality.
*/
class Persistence {
public:

    /**
    * Create a new persistence layer using the given database session. If
    * managedTransactions is given and set to true, the persistence assumes
    * that the caller manages transactions and does not do any transaction
    * handling itself for updates (useful for bulk updates).
    */
    Persistence(cppdb::session &sql, bool managedTransactions = false);

    /**
    * Add a new statement to the database.
    */
    void addStatement(const Statement& st);

    /**
    * Update the database state of the given statement. Currently only takes
    * into account the approval state.
    */
    void updateStatement(const Statement& st);

    /**
    * Update the database state of the given statement. Currently only takes
    * into account the approval state.
    */
    void updateStatement(int64_t id, ApprovalState state);

    /**
    * Retrieve the statement with the given database ID.
    */
    Statement getStatement(int64_t id);

    /**
    * Return a list of all statements with the given QID as subject.
    */
    std::vector<Statement> getStatementsByQID(const std::string& qid);

    /**
    * Return a list of count random statements. Selection is up to the backend.
    */
    std::vector<Statement> getStatementsRandom(int count);

    /**
    * Return a list of count random statements concerned with the topic given
    * as string.
    */
    std::vector<Statement> getStatementsByTopic(std::string &topic, int count);


private:
    // reference to the wrapped sql session
    cppdb::session& sql;

    bool managedTransactions;
};

#endif