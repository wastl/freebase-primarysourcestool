#include "SourcesToolBackend.hpp"

#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <fstream>


SourcesToolBackend::SourcesToolBackend(cppcms::service &srv)
        : cppcms::application(srv) {

    // map GET and POST requests to /entities/<QID> to the respective handlers
    // we use a helper function to distinguish both cases, since cppcms
    // currently does not really support REST
    dispatcher().assign("/entities/(Q\\d+)",
            &SourcesToolBackend::handleGetPostEntity, this, 1);
    mapper().assign("entity_by_qid", "/entities/{1}");

    // map request to random entity selector
    dispatcher().assign("/entities/any",
            &SourcesToolBackend::getEntityByTopicUser, this);
    mapper().assign("entity_by_topic_user", "/entities/any");

}


void SourcesToolBackend::handleGetPostEntity(std::string qid) {
    if(request().request_method() == "POST") {
        approveEntity(qid);
    } else {
        getEntityByQID(qid);
    }
}

void SourcesToolBackend::getEntityByQID(std::string qid) {
    clock_t begin = std::clock();

    // currently always return the test QID
    std::ifstream input(settings()["datafile"].str());

    if(input.fail()) {
        response().status(404, "could not open data file, please specify path in config.json");
        return;
    }

    cppcms::json::value entity;

    input >> entity;

    response().content_type("application/json");
    entity.save(response().out(), cppcms::json::readable);

    clock_t end = std::clock();
    std::cout << "GET /entities/" << qid << " time: " << 1000 * (double(end - begin) / CLOCKS_PER_SEC) << "ms" << std::endl;
}

void SourcesToolBackend::getEntityByTopicUser() {
    // currently always return the test QID

    clock_t begin = std::clock();

    // currently always return the test QID
    std::ifstream input(settings()["datafile"].str());

    if(input.fail()) {
        response().status(404, "could not open data file, please specify path in config.json");
        return;
    }

    cppcms::json::value entity;

    input >> entity;

    response().content_type("application/json");
    entity.save(response().out(), cppcms::json::readable);

    clock_t end = std::clock();
    std::cout << "GET /entities/any time: " << 1000 * (double(end - begin) / CLOCKS_PER_SEC) << "ms" << std::endl;
}

void SourcesToolBackend::approveEntity(std::string qid) {
    clock_t begin = std::clock();

    if(request().get("approved") != "true") {
        response().status(418, "I'm a teapot");
    }

    clock_t end = std::clock();
    std::cout << "POST /entities/" << qid << " time: " << 1000 * (double(end - begin) / CLOCKS_PER_SEC) << "ms" << std::endl;
}
