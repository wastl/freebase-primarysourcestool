// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#ifndef HAVE_SERIALIZER_H_
#define HAVE_SERIALIZER_H_

#include <iostream>
#include <vector>

#include <cppcms/json.h>

#include "Statement.h"

namespace Serializer {

    /**
    * Write a collection of statements to an output stream as JSON.
    * Uses a JSON format resembling RDF/JSON but extended by some
    * Wikidata specific elements. Also, since we do not need to support
    * blank nodes and resources are identified with plain identifiers,
    * the format is significantly shorter.
    *
    * https://dvcs.w3.org/hg/rdf/raw-file/default/rdf-json/index.html
    */
    template<typename Iterator>
    void writePlainJSON(Iterator begin, Iterator end, std::ostream &out) {

        cppcms::json::value result;

        for(const Statement &stmt = *begin; begin != end; ++begin) {
            const std::string& lang = stmt.getValue().getLanguage();
            const std::string& prop = stmt.getProperty();
            const std::string& qid  = stmt.getQID();


            cppcms::json::value value;
            value["id"] = stmt.getID();
            if(lang != "") {
                value["lang"] = lang;
            }
            value["value"] = stmt.getValue().getValue();
            if (stmt.getValue().getType() == CONCEPT) {
                value["type"] = "entity";
            } else {
                value["type"] = "literal";
            }

            if(result[qid][prop].type() != cppcms::json::is_array) {
                result[qid][prop] = { value };
            } else {
                result[qid][prop].array().push_back(value);
            }

            std::cout << "processed (" << qid << "," <<prop<<","<<stmt.getValue()<<")"<<std::endl;
        }

        result.save(out, cppcms::json::readable);
    }

    /**
    * Write a collection of statements to an output stream as JSON.
    * Uses the WikiData JSON format documented at
    * https://www.mediawiki.org/wiki/Wikibase/Notes/JSON .
    * Each statement is represented as a "claim" in the Wikidata terminology.
    */
    template<typename Iterator>
    void writeWikidataJSON(Iterator begin, Iterator end, std::ostream &out) {

        cppcms::json::value entities;

        for(const Statement &stmt = *begin; begin != end; ++begin) {
            const std::string& lang = stmt.getValue().getLanguage();
            const std::string& prop = stmt.getProperty();
            const std::string& qid  = stmt.getQID();

            entities[qid]["claims"][prop]["id"] = stmt.getID();
            entities[qid]["claims"][prop]["type"] = "claim";
            entities[qid]["claims"][prop]["rank"] = "normal";

            cppcms::json::value snak;
            snak["snaktype"] = "value";
            snak["property"] = prop;

            if (stmt.getValue().getType() == CONCEPT) {
                snak["datavalue"]["type"] = "wikibase-entityid";
                snak["datavalue"]["value"]["entity-type"] = "item";
                snak["datavalue"]["value"]["numeric-id"] = atol(stmt.getValue().getValue().substr(1).c_str());
            } else {
                snak["datavalue"]["value"] = stmt.getValue().getValue();
                snak["datavalue"]["type"] = "string";
                if(stmt.getValue().getType() == LANG_LITERAL) {
                    snak["datavalue"]["language"] = lang;
                }
            }
            entities[qid]["claims"][prop]["mainsnak"] = snak;

            std::cout << "processed (" << qid << "," <<prop<<","<<stmt.getValue()<<")"<<std::endl;
        }

        cppcms::json::value result;
        result["entities"] = entities;
        result.save(out, cppcms::json::readable);
    }

    template<typename Iterator>
    void write(std::string format, Iterator begin, Iterator end, std::ostream &out) {
        if(format == "application/wikidata+json") {
            writeWikidataJSON(begin, end, out);
        } else {
            // other serializers not yet supported
            writePlainJSON(begin, end, out);
        }
    }


}

#endif // HAVE_SERIALIZER_H_
