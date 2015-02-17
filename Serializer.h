#ifndef HAVE_SERIALIZER_H_
#define HAVE_SERIALIZER_H_

#include <iostream>
#include <vector>

#include <cppcms/json.h>

#include "Statement.h"

namespace Serializer {


    /**
    * Write a collection of statements to an output stream as JSON.
    */
    template<typename Iterator>
    void writeJSON(Iterator begin, Iterator end, std::ostream &out) {

        cppcms::json::value entities;

        for(const Statement &stmt = *begin; begin != end; ++begin) {
            const std::string& lang = stmt.getValue().getLanguage();
            const std::string& prop = stmt.getProperty();
            const std::string& qid  = stmt.getQID();

            entities["entities"][qid][prop][lang]["value"] = stmt.getValue().getValue();
            if(stmt.getValue().getType() == LANG_LITERAL) {
                entities["entities"][qid][prop][lang]["language"] = lang;
            }

            std::cout << "processed (" << qid << "," <<prop<<","<<stmt.getValue()<<")"<<std::endl;
        }

        entities.save(out, cppcms::json::readable);
    }

    template<typename Iterator>
    void write(std::string format, Iterator begin, Iterator end, std::ostream &out) {
        if(format == "application/json") {
            writeJSON(begin, end, out);
        } else {
            // other serializers not yet supported
            writeJSON(begin, end, out);
        }
    }


}

#endif // HAVE_SERIALIZER_H_
