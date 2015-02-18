// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#ifndef HAVE_STATEMENT_H_
#define HAVE_STATEMENT_H_

#include <string>
#include <vector>

#include <boost/multiprecision/cpp_dec_float.hpp>

#include "Qualifier.h"
#include "Value.h"

enum ApprovalState {
    APPROVED, UNAPPROVED, OTHERSOURCE, WRONG, SKIPPED
};

enum ValueType {
    ITEM, STRING, TIME, LOCATION, QUANTITY
};


// define decimal datatype to represent wikidata quantities as multi-
// precision
typedef boost::multiprecision::cpp_dec_float_50 decimal_t;

// define locations as pairs of doubles
typedef std::pair<double,double> location_t;

// define values as either string, time, location or quantity
union Value {
    std::string str;
    time_t      time;
    location_t  loc;
    decimal_t   quantity;

    Value(std::string s) : str(s) { }
    Value(time_t t) : time(t) { }
    Value(location_t l) : loc(l) { }
    Value(decimal_t d) : quantity(d) { }
};


/**
* Property-value pair, also called 'snak' in Wikidata terminology. Used to
* represent the primary property and value, as well as qualifiers and sources.
*
* All value types are represented internally using the string representation
* described at http://tools.wmflabs.org/wikidata-todo/quick_statements.php
*/
class PropertyValue {


public:


    PropertyValue(std::string property, std::string value,
                  std::string lang, value_t type)
            : property(property), value(value), language(lang), type(type) {
    }

    // default copy constructor and assignment operator
    PropertyValue(const PropertyValue& other) = default;
    PropertyValue& operator=(const PropertyValue& other) = default;


    const std::string &getProperty() const {
        return property;
    }

    const Value &getValue() const {
        return value;
    }

    const std::string &getLanguage() const {
        return language;
    }

    const ValueType &getType() const {
        return type;
    }

private:
    std::string property, language;

    Value value;

    ValueType type;

};

/**
* Representation of a Freebase/Wikidata statement to be approved by users.
* Similar to RDF triples, but a simplified model without blank nodes. For each
* statement we also store an internal (database) ID that allows to uniquely
* identify the statement, and the current state of approval.
*/
class Statement {

typedef std::vector<PropertyValue> extensions_t;

 public:


    // main constructor; will be called usually in a parser or with database results
    // so we use copy by value and rely on the compiler to optimize using rvalues
    Statement(int64_t id, std::string qid, PropertyValue propertyValue,
              extensions_t qualifiers, extensions_t sources, ApprovalState approved)
            : id(id), qid(qid), propertyValue(propertyValue),
              qualifiers(qualifiers), sources(sources), approved(approved) { }


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
    const std::string& getProperty() const {
        return propertyValue.getProperty();
    }


    const std::string &getValue() const {
        return propertyValue.getValue();
    }


    const PropertyValue &getPropertyValue() const {
        return propertyValue;
    }

    const extensions_t &getQualifiers() const {
        return qualifiers;
    }

    const extensions_t &getSources() const {
        return sources;
    }

    /**
    * Return true if this statement has already been approved, false otherwise.
    */
    ApprovalState getApprovalState() const { return approved; }

 private:
    int64_t id;

    std::string qid;

    PropertyValue propertyValue;

    extensions_t qualifiers;
    extensions_t sources;


    ApprovalState approved;
};

#endif  // HAVE_STATEMENT_H_
