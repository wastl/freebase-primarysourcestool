# REST API

## Get Entity by QID

The following HTTP request retrieves an entity by Wikidata QID:

    GET /entity/<QID>.json
      
The service is modelled after the Wikidata REST API, but currently only supports retrieving JSON format.
    
Status Codes:

  * 200: entity found, returned as JSON
  * 404: entity not found
  * 500: server error


## Get Random Entity (by Topic)

The following HTTP request retrieves a random entity, optionally by topic. The selection procedure is
left to the backend and might be improved in the future based on user profile etc.

    GET /entity?topic=<TOPIC>&user=<WikidataUser>
      
Both topic and user are optional, in which case the backend selects a random unapproved entity.
    
Status Codes:

  * 200: entity found, returned as JSON
  * 404: entity not found
  * 500: server error

## Mark Entity as Approved

The following HTTP request marks an entity identified by a Wikidata QID as approved:

    POST /entity/<QID>?approved=true&user=<WikidataUser>
    
The WikidataUser passed as argument is used for tracking purposes only and stored in the database together with
the approval flag.
   
Status Codes:

  * 200: entity found and marked as approved by user <WikidataUser>
  * 409: entity found but was already marked as approved by another user
  * 500: server error
  
 