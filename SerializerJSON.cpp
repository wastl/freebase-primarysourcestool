#include "SerializerJSON.hpp"

#include <cppcms/json.h>

void SerializerJSON::write(std::ostream &out) {
    cppcms::json::value entities;

    for(Statement &stmt : statements) {
        const std::string& lang = stmt.getValue().getLanguage();
        const std::string& prop = stmt.getProperty();
        const std::string& qid  = stmt.getQID();

        entities["entities"][qid][prop][lang]["value"] = stmt.getValue().getValue();
        entities["entities"][qid][prop][lang]["language"] = lang;
    }

    out << entities;
}
