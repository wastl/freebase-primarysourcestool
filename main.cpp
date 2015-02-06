#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/json.h>
#include <istream>
#include <stdlib.h>
#include <ctime>

class membuf : public std::streambuf {
public:
    membuf(char* base, std::size_t size) {
        this->setp(base, base + size);
        this->setg(base, base, base + size);
    }

    membuf(std::pair<void*, size_t> pair) : membuf((char *) pair.first, pair.second) {};

    std::size_t written() const { return this->pptr() - this->pbase(); }
    std::size_t read() const    { return this->gptr() - this->eback(); }
};

class hello : public cppcms::application {
public:
    hello(cppcms::service &srv) : cppcms::application(srv) {
        dispatcher().assign("/number/(\\d+)", &hello::number, this, 1);
        mapper().assign("number", "/number/{1}");

        dispatcher().assign("/smile", &hello::smile, this);
        mapper().assign("smile", "/smile");

        dispatcher().assign("/data", &hello::data, this);
        mapper().assign("data", "/data");

        dispatcher().assign("/post", &hello::post, this);
        mapper().assign("post", "/post");

        dispatcher().assign("", &hello::welcome, this);
        mapper().assign("");

        mapper().root("/hello");
    }

    void number(std::string num) {
        int no = atoi(num.c_str());
        response().out() << "The number is " << no << "<br/>\n";
        response().out() << "<a href='" << url("/") << "'>Go back</a>";
    }

    void smile() {
        response().out() << ":-) <br/>\n";
        response().out() << "<a href='" << url("/") << "'>Go back</a>";
    }

    void welcome() {
        response().out() <<
                "<h1> Welcome To Page with links </h1>\n"
                "<a href='" << url("/number", 1) << "'>1</a><br>\n"
                "<a href='" << url("/number", 15) << "'>15</a><br>\n"
                "<a href='" << url("/data") << "' >Get Data</a><br>\n"
                "<a href='" << url("/smile") << "' >:-)</a><br>\n";
    }

    void data() {
        cppcms::json::value input;
        cppcms::json::value result;

        membuf body(request().raw_post_data());
        std::istream in(&body);

        in >> input;

        result["request"] = input;

        response().content_type("application/json");
        response().out() << result;
    }

    void post() {
        clock_t begin = std::clock();

        cppcms::json::value input;
        cppcms::json::value result;

        membuf body(request().raw_post_data());
        std::istream in(&body);

        in >> input;

        std::cout << "Name: " << input["name"] << std::endl;

        result["result"] = input;

        response().content_type("application/json");
        response().out() << result;

        clock_t end = std::clock();

        std::cout << "Elapsed time: " << 1000 * (double(end - begin) / CLOCKS_PER_SEC) << "ms" << std::endl;
    }
};


int main(int argc, char **argv) {
    try {
        cppcms::service srv(argc, argv);
        srv.applications_pool().mount(
                cppcms::applications_factory<hello>()
        );
        srv.run();

        return 0;
    }
    catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;

        return 1;
    }
}
