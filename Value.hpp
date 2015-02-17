#ifndef HAVE_VALUE_HPP
#define HAVE_VALUE_HPP 1

#include <iostream>
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

    operator const std::string& () const { return value; }
private:

    std::string value, language;

    ValueType type;

};

inline std::ostream& operator<<(std::ostream &out, const Value &v) {
    return out << v.getValue();
}

#endif