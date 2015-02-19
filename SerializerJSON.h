// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#ifndef HAVE_SERIALIZERJSON_H_
#define HAVE_SERIALIZERJSON_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

#include <cppcms/json.h>

#include "Statement.h"

namespace Serializer {

    /**
    * Create a Wikidata JSON 'snak', representing a property/value pair with
    * metadata. Takes as argument an internal PropertyValue structure and
    * returns a CppCMS JSON value for further use.
    */
    cppcms::json::value createWikidataSnak(const PropertyValue& pv) {
        const Value& v = pv.getValue();

        cppcms::json::value snak;
        snak["snaktype"] = "value";
        snak["property"] = pv.getProperty();
        switch (v.getType()) {
            case ITEM: {
                    const std::string &qid = pv.getValue().getString();
                    snak["datavalue"]["type"] = "wikibase-entityid";
                    if (qid[0] == 'Q') {
                        snak["datavalue"]["value"]["entity-type"] = "item";
                    } else if (qid[0] == 'P') {
                        snak["datavalue"]["value"]["entity-type"] = "property";
                    } else if (qid[0] == 'S') {
                        snak["datavalue"]["value"]["entity-type"] = "source";
                    } else {
                        snak["datavalue"]["value"]["entity-type"] = "unknown";
                    }
                    snak["datavalue"]["value"]["numeric-id"] =
                            atol(v.getString().substr(1).c_str());
                }
                break;
            case STRING: {;
                    const std::string &txt = v.getString();
                    snak["datavalue"]["type"] = "string";
                    snak["datavalue"]["value"] = txt;
                    if (pv.getValue().getLanguage() != "") {
                        // TODO: is this correct? documentation doesn't mention the case
                        snak["datavalue"]["language"] = v.getLanguage();
                    }
                }
                break;
            case LOCATION:
                snak["datavalue"]["type"] = "globecoordinate";
                snak["datavalue"]["value"]["latitude"] = v.getLocation().first;
                snak["datavalue"]["value"]["longitude"] = v.getLocation().second;
                break;
            case TIME: {
                    std::ostringstream fmt;
                    fmt << std::setfill('0')
                            << "+" << std::setw(11) << v.getTime().tm_year
                            << "-" << std::setw(2) << v.getTime().tm_mon
                            << "-" << std::setw(2) << v.getTime().tm_mday
                            << "T" << std::setw(2) << v.getTime().tm_hour
                            << ":" << std::setw(2) << v.getTime().tm_min
                            << ":" << std::setw(2) << v.getTime().tm_sec
                            << "Z";
                    snak["datavalue"]["type"] = "time";
                    snak["datavalue"]["value"]["time"] = fmt.str();
                    snak["datavalue"]["value"]["precision"] = v.getPrecision();
                }
                break;
        }
        return snak;
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

        for(; begin != end; ++begin) {
            const Statement& stmt = *begin;

            const std::string& prop = stmt.getProperty();
            const std::string& qid  = stmt.getQID();

            cppcms::json::value entity;

            entity["id"] = stmt.getID();
            entity["type"] = "claim";
            entity["rank"] = "normal";
            entity["mainsnak"] = createWikidataSnak(stmt.getPropertyValue());

            std::map<std::string, int> qcount;
            for(const PropertyValue& pv : stmt.getQualifiers()) {
                int c = 0;

                auto it = qcount.find(pv.getProperty());
                if (it != qcount.end()) {
                    c = it->second;
                }

                entity["qualifiers"][pv.getProperty()][c] = createWikidataSnak(pv);

                qcount[pv.getProperty()] = ++c;

            }

            std::map<std::string, int> scount;
            for(const PropertyValue& pv : stmt.getSources()) {
                int c = 0;

                auto it = scount.find(pv.getProperty());
                if (it != scount.end()) {
                    c = it->second;
                }

                entity["references"][0]["snaks"][pv.getProperty()][c] = createWikidataSnak(pv);

                scount[pv.getProperty()] = ++c;
            }


            entities[qid]["claims"][prop] = entity;
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

#endif // HAVE_SERIALIZERJSON_H_
