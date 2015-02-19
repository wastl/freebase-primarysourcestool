#ifndef HAVE_SERIALIZER_TSV_H_
#define HAVE_SERIALIZER_TSV_H_

#include <string>
#include <iomanip>

#include "Statement.h"

namespace Serializer {

    void writeValueTSV(const Value& v, std::ostream &out) {
        switch (v.getType()) {
            case ITEM:
                out << v.getString();
                break;
            case LOCATION:
                out << "@" << v.getLocation().first << "/" << v.getLocation().second;
                break;
            case QUANTITY:
                out << (v.getQuantity().sign()<0?"":"+") << v.getQuantity();
                break;
            case STRING:
                if (v.getLanguage() != "") {
                    out << v.getLanguage() << ":";
                }
                out.put('"');
                for(char c : v.getString()) {
                    if (c == '\\' || c == '"') {
                        out.put('\\');
                    }
                    out.put(c);
                }
                out.put('"');
                break;
            case TIME:
                out << std::setfill('0')
                        << "+" << std::setw(11) << v.getTime().tm_year
                        << "-" << std::setw(2) << v.getTime().tm_mon
                        << "-" << std::setw(2) << v.getTime().tm_mday
                        << "T" << std::setw(2) << v.getTime().tm_hour
                        << ":" << std::setw(2) << v.getTime().tm_min
                        << ":" << std::setw(2) << v.getTime().tm_sec
                        << "Z/" << v.getPrecision();
                break;
        }
    }

    /**
    * Write a sequence of statements to the output stream using the TSV format
    * used by Wikidata.
    * (http://tools.wmflabs.org/wikidata-todo/quick_statements.php)
    */
    template<typename Iterator>
    void writeTSV(Iterator begin, Iterator end, std::ostream &out) {

        for(; begin != end; ++begin) {
            const Statement& stmt = *begin;
            out << stmt.getQID() << "\t"
                << stmt.getProperty() << "\t";
            writeValueTSV(stmt.getValue(), out);

            for (const PropertyValue& pv : stmt.getQualifiers()) {
                out << "\t" << pv.getProperty() << "\t";
                writeValueTSV(pv.getValue(), out);
            }

            for (const PropertyValue& pv : stmt.getSources()) {
                out << "\t" << pv.getProperty() << "\t";
                writeValueTSV(pv.getValue(), out);
            }
            out << std::endl;
        }

    }

}  // namespace Serializer

#endif