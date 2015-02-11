#include "SourcesToolBackend.hpp"

#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>


SourcesToolBackend::SourcesToolBackend(cppcms::service &srv) : cppcms::application(srv) {
    dispatcher().assign("/entity/(Q\\d+)", &SourcesToolBackend::getEntityByQID, this, 1);
    mapper().assign("number", "/entity/{1}");

}

void SourcesToolBackend::getEntityByQID(std::string qid) {

}

void SourcesToolBackend::getEntityByTopicUser() {

}

void SourcesToolBackend::approveEntity(std::string qid) {

}
