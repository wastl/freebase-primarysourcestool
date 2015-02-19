#include "Persistence.h"

static int64_t addSnak(cppdb::session &sql, const PropertyValue &pv) {

    switch (pv.getValue().getType()) {
        case ITEM:
            return (sql << "INSERT INTO snak(property,svalue,vtype) "
                           "VALUES (?,?,'item')"
                        << pv.getProperty() << pv.getValue().getString()
                        << cppdb::exec
                   ).last_insert_id();
        case STRING:
            return (sql << "INSERT INTO snak(property,svalue,lang,vtype) "
                           "VALUES (?,?,?,'string')"
                        << pv.getProperty() << pv.getValue().getString()
                        << pv.getValue().getLanguage() << cppdb::exec
                   ).last_insert_id();
        case QUANTITY:
            return (sql << "INSERT INTO snak(property,dvalue,vtype) "
                           "VALUES (?,?,'quantity')"
                        << pv.getProperty() << static_cast<double>(pv.getValue().getQuantity())
                        << cppdb::exec
                   ).last_insert_id();
        case TIME:
            return (sql << "INSERT INTO snak(property,tvalue,precision,vtype) "
                           "VALUES (?,?,?,'time')"
                    << pv.getProperty() << pv.getValue().getTime()
                    << pv.getValue().getPrecision() << cppdb::exec
            ).last_insert_id();
        case LOCATION:
            return (sql << "INSERT INTO snak(property,lat,lng,vtype) "
                           "VALUES (?,?,?,'location')"
                    << pv.getProperty() << pv.getValue().getLocation().first
                    << pv.getValue().getLocation().second << cppdb::exec
            ).last_insert_id();

    }
}

Persistence::Persistence(cppdb::session &sql, bool managedTransactions)
        : sql(sql), managedTransactions(managedTransactions) {

}

void Persistence::addStatement(const Statement& st){
    if (!managedTransactions)
        sql.begin();

    int64_t snakid = addSnak(sql, st.getPropertyValue());
    int64_t stmtid = (
            sql << "INSERT INTO statement(subject,mainsnak) VALUES (?,?)"
                << st.getQID() << snakid << cppdb::exec).last_insert_id();

    for(const PropertyValue& pv : st.getQualifiers()) {
        int64_t qualid = addSnak(sql, pv);
        sql << "INSERT INTO qualifier(stmt,snak) VALUES (?,?)"
            << stmtid << qualid << cppdb::exec;
    }

    for(const PropertyValue& pv : st.getSources()) {
        int64_t qualid = addSnak(sql, pv);
        sql << "INSERT INTO source(stmt,snak) VALUES (?,?)"
            << stmtid << qualid << cppdb::exec;
    }

    if (!managedTransactions)
        sql.commit();
}

void Persistence::updateStatement(const Statement &st) {
    updateStatement(st.getID(), st.getApprovalState());
}

void Persistence::updateStatement(int64_t id, ApprovalState state) {
    sql.begin();

    sql << "UPDATE statement SET state = ? WHERE id = ?"
        << state << id << cppdb::exec;

    sql.commit();
}

/*
Statement Persistence::getStatement(int64_t id) {
    return Statement(0, std::basic_string<char, char_traits < _CharT>, allocator < _CharT >> (), PropertyValue(), std::vector<PropertyValue, allocator < PropertyValue>>
    (), std::vector<PropertyValue, allocator < PropertyValue>>
    (), SKIPPED);
}

std::vector<Statement> Persistence::getStatementsByQID(const std::string &qid) {
    return std::vector<Statement, allocator < Statement>>
    ();
}

std::vector<Statement> Persistence::getStatementsRandom(int count) {
    return std::vector<Statement, allocator < Statement>>
    ();
}

std::vector<Statement> Persistence::getStatementsByTopic(std::string &topic, int count) {
    return std::vector<Statement, allocator < Statement>>
    ();
}
*/