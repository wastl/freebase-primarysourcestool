// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#include "SourcesToolService.h"

#include <fstream>

#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>

#include "SerializerTSV.h"
#include "SerializerJSON.h"
#include "Persistence.h"

SourcesToolService::SourcesToolService(cppcms::service &srv)
        : cppcms::application(srv), backend(settings()["database"]) {
    // map GET and POST requests to /entities/<QID> to the respective handlers
    // we use a helper function to distinguish both cases, since cppcms
    // currently does not really support REST
    dispatcher().assign("/entities/(Q\\d+)",
            &SourcesToolService::getEntityByQID, this, 1);
    mapper().assign("entity_by_qid", "/entities/{1}");

    // map request to random entity selector
    dispatcher().assign("/entities/any",
            &SourcesToolService::getEntityByTopicUser, this);
    mapper().assign("entity_by_topic_user", "/entities/any");

    dispatcher().assign("/statements/(\\d+)",
            &SourcesToolService::handleGetPostStatement, this, 1);
    mapper().assign("stmt_by_id", "/statements/{1}");

}

void SourcesToolService::handleGetPostStatement(std::string stid) {
    if (request().request_method() == "POST") {
        approveStatement(std::stoll(stid));
    } else {
        getStatement(std::stoll(stid));
    }
}

void SourcesToolService::getEntityByQID(std::string qid) {
    clock_t begin = std::clock();

    std::vector<Statement> statements = backend.getStatementsByQID(qid, true);

    if (statements.size() > 0) {
        serializeStatements(statements);
    } else {
        response().status(404, "no statements found for entity "+qid);
    }

    clock_t end = std::clock();
    std::cout << "GET /entities/" << qid << " time: "
            << 1000 * (static_cast<double>(end - begin) / CLOCKS_PER_SEC)
            << "ms" << std::endl;
}

void SourcesToolService::getEntityByTopicUser() {
    // currently always return the test QID

    clock_t begin = std::clock();

    // currently always return the test QID
    std::ifstream input(settings()["datafile"].str());

    if (input.fail()) {
        response().status(404,
                "could not open data file, please specify in config.json");
        return;
    }

    cppcms::json::value entity;

    input >> entity;

    response().content_type("application/json");
    entity.save(response().out(), cppcms::json::readable);

    clock_t end = std::clock();
    std::cout << "GET /entities/any time: "
            << 1000 * (static_cast<double>(end - begin) / CLOCKS_PER_SEC)
            << "ms" << std::endl;
}

void SourcesToolService::approveStatement(int64_t stid) {
    clock_t begin = std::clock();

    ApprovalState state = UNAPPROVED;

    // return 403 forbidden when there is no user given
    if (request().get("user") == "") {
        response().status(403, "Forbidden: invalid or missing user");
        return;
    }

    // determine the updated state or return 400 in case it is invalid
    if (request().get("state") == "approved") {
        state = APPROVED;
    } else if(request().get("state") == "wrong") {
        state = WRONG;
    } else if(request().get("state") == "othersource") {
        state = OTHERSOURCE;
    } else if(request().get("state") == "unapproved") {
        state = UNAPPROVED;
    } else {
        response().status(400, "Bad Request: invalid or missing state parameter");
        return;
    }

    // check if statement exists and update it with new state
    try {
        Statement st = backend.getStatementByID(stid);
        backend.updateStatement(stid, state, request().get("user"));
    } catch(PersistenceException const &e) {
        response().status(404, "Statement not found");
    }

    clock_t end = std::clock();
    std::cout << "POST /statements/" << stid << " time: "
              << 1000 * (static_cast<double>(end - begin) / CLOCKS_PER_SEC)
              << "ms" << std::endl;
}

void SourcesToolService::getStatement(int64_t stid) {
    clock_t begin = std::clock();

    // query for statement, wrap it in a vector and return it
    try {
        std::vector<Statement> statements = { backend.getStatementByID(stid) };
        serializeStatements(statements);
    } catch(PersistenceException const &e) {
        std::cerr << "error: " << e.what() << std::endl;
        response().status(404, "Statement not found");
    }

    clock_t end = std::clock();
    std::cout << "GET /statements/" << stid << " time: "
            << 1000 * (static_cast<double>(end - begin) / CLOCKS_PER_SEC)
            << "ms" << std::endl;
}


void SourcesToolService::serializeStatements(const std::vector<Statement> &statements) {
    if(request().http_accept() == "text/vnd.wikidata+tsv"
            || request().http_accept() == "text/tsv") {
        response().content_type("text/vnd.wikidata+tsv");

        Serializer::writeTSV(statements.cbegin(), statements.cend(), response().out());
    } else if(request().http_accept() == "application/wikidata+json") {
        response().content_type("application/vnd.wikidata+json");

        Serializer::writeWikidataJSON(statements.cbegin(), statements.cend(), response().out());
    } else {
        response().content_type("application/vnd.wikidata.envelope+json");

        Serializer::writeEnvelopeJSON(statements.cbegin(), statements.cend(), response().out());
    }
}
