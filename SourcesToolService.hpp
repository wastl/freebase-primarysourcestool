#ifndef HAVE_SOURCESTOOLBACKEND_HPP
#define HAVE_SOURCESTOOLBACKEND_HPP 1

#include <string>

#include <cppcms/application.h>
#include <map>


class SourcesToolService : public cppcms::application {

public:

    /**
    * Initialise SourcesToolService. Registers URL mappers and initialises a
    * database connection to access the entity data.
    */
    SourcesToolService(cppcms::service &srv);

    /**
    * Return the entity identified by the given QID and write it to the
    * response in Wikidata JSON format.
    *
    * Request:
    *     GET /entity/<QID>
    *
    * Status Codes:
    *     200: entity found, returned as JSON
    *     204: entity found, but already approved (no content)
    *     404: entity not found
    *     500: server error
    */
    void getEntityByQID(std::string qid);

    /**
    * Return a random non-approved entity from the database, optionally
    * narrowing down the selection by topic and user. If topic is given, the
    * backend will select only entities with the given topic. If a user is
    * given, the backend will take into account information about the user
    * when selecting an entity (e.g. topics (s)he previously worked on).
    * The result is written to the response in Wikidata JSON format.
    *
    * Request:
    *     GET /entity?topic=<TOPIC>&user=<WikidataUser>
    *
    * Status Codes:
    *     200: entity found, returned as JSON
    *     404: entity not found
    *     500: server error
    */
    void getEntityByTopicUser();

    /**
    * Mark the entity with the given ID as approved. Approved entities will no
    * longer be offered in the getEntity methods.
    *
    * Request:
    *     POST /entity/<QID>?approved=true&user=<WikidataUser>
    *
    * Status Codes:
    *     200: entity found and marked as approved by user <WikidataUser>
    *     409: entity found but was already marked as approved by another user
    *     500: server error
    */
    void approveEntity(std::string qid);

private:

    void handleGetPostEntity(std::string);
};

#endif