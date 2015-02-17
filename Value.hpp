#ifndef HAVE_VALUE_HPP
#define HAVE_VALUE_HPP

#include <string>

class Value {


public:


    Value(std::string &value, std::string &language, std::string &type)
            : value(value), language(language), type(type) { }


    const std::string &getValue() const {
        return value;
    }

    const std::string &getLanguage() const {
        return language;
    }

    const std::string &getType() const {
        return type;
    }

private:

    std::string value, language, type;

};

#endif