#ifndef HAVE_SERIALIZER_JSON_HPP
#define HAVE_SERIALIZER_JSON_HPP 1

#include "Serializer.hpp"

class SerializerJSON : public Serializer {

public:


    virtual void write(std::ostream &out) override;
};


#endif

