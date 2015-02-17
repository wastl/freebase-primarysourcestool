#ifndef HAVE_VALUE_H_
#define HAVE_VALUE_H_

#include <iostream>
#include <string>

enum ValueType {
    CONCEPT, LITERAL, LANG_LITERAL
};

class Value {
 public:
    explicit Value(const std::string &value)
            : value(value), language(""), type(CONCEPT) { }

    Value(const std::string &value, const std::string &language, ValueType type)
            : value(value), language(language), type(type) { }


    // default copy constructor and assignment operator
    Value(const Value& other) = default;
    Value& operator=(const Value& other) = default;


    const std::string& getValue() const {
        return value;
    }

    const std::string& getLanguage() const {
        return language;
    }

    ValueType getType() const {
        return type;
    }

 private:
    std::string value, language;

    ValueType type;
};

inline std::ostream& operator<<(std::ostream &out, const Value &v) {
    return out << v.getValue();
}

#endif  // HAVE_VALUE_H_
