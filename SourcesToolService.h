// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#ifndef HAVE_SOURCESTOOLSERVICE_H_
#define HAVE_SOURCESTOOLSERVICE_H_

#include <cppcms/application.h>

#include <string>
#include <map>

#include "SourcesToolBackend.h"

class SourcesToolService : public cppcms::application {
public:
   /**
   * Initialise SourcesToolService. Registers URL mappers and initialises a
   * database connection to access the entity data.
   */
   SourcesToolService(cppcms::service &srv);

   /**
   * Return the entity identified by the given QID and write it to the
   * response. The format is determined by content negotiation (either
   * text/tsv, application/vnd.wikidata+json or application/json).
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
   * The result is written to the response in the format determined by
   * content negotiation (either text/tsv, application/vnd.wikidata+json
   * or application/json).
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
   * Return the statement with the given ID and write it to the response.
   * The format is determined by content negotiation (either
   * text/tsv, application/vnd.wikidata+json or application/json).
   *
   * Request:
   *     GET /statements/<ID>
   *
   * Status Codes:
   *     200: statement found and returned
   *     404: statement not found
   *     500: server error
   */
   void getStatement(int64_t stid);

   /**
   * Mark the statement with the given ID as approved or wrong. Approved
   * statements will no longer be offered in the getEntity methods.
   *
   * Request:
   *     POST /statements/<ID>?state=<STATE>&user=<WikidataUser>
   *
   * where <STATE> one of "approved", "wrong", "othersource"
   *
   * Status Codes:
   *     200: statement found and marked as approved by user <WikidataUser>
   *     404: statement not found
   *     409: statement found but was already marked as approved by another user
   *     500: server error
   */
   void approveStatement(int64_t stid);

private:
   void handleGetPostStatement(std::string);

   // serialize list of statements according to content negotiation
   void serializeStatements(const std::vector<Statement>& stmts);

   SourcesToolBackend backend;

};

#endif  // HAVE_SOURCESTOOLSERVICE_H_
