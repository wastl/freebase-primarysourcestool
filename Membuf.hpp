#ifndef HAVE_MEMBUF_HPP
#define HAVE_MEMBUF_HPP 1

#include <streambuf>

/*
 * In-memory buffer to read data from a POST request body as stream. Example use:
 *
 * Membuf body(request().raw_post_data());
 * std::istream in(&body);
 * in >> input;
 */
class Membuf : public std::streambuf {
public:
    Membuf(char* base, std::size_t size) {
        this->setp(base, base + size);
        this->setg(base, base, base + size);
    }

    Membuf(std::pair<void*, size_t> pair) : membuf((char *) pair.first, pair.second) {};

    std::size_t written() const { return this->pptr() - this->pbase(); }
    std::size_t read() const    { return this->gptr() - this->eback(); }
};

#endif