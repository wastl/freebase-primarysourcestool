#ifndef HAVE_VALUE_HPP
#define HAVE_VALUE_HPP

#include <string>

enum ValueType {
    CONCEPT, LITERAL, LANG_LITERAL
};

class Value {


public:

    Value(std::string &value) : value(value), type(CONCEPT) {
    }

    Value(std::string &value, std::string &language, ValueType type)
            : value(value), language(language), type(type) { }


    const std::string &getValue() const {
        return value;
    }

    const std::string &getLanguage() const {
        return language;
    }

    ValueType getType() const {
        return type;
    }

private:

    std::string value, language;

    ValueType type;

};

#endif