#ifndef HAVE_QUALIFIER_H_
#define HAVE_QUALIFIER_H_

#include <string>

#include "Value.h"

class Qualifier {
public:

    Qualifier(const std::string& property, Value& value)
            : property(property), value(value) {  }


    const std::string& getProperty() const {
        return property;
    }

    const Value& getValue() const {
        return value;
    }

private:
    std::string property;
    Value value;
};

#endif  // HAVE_QUALIFIER_H_