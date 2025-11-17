#include "JSONAdapter.h"

#include "JSONDocument.h"
// MODIFICATION: Commented out - see vendor/MODIFICATIONS.md
// #include "JSONSchemaValidator.h"


namespace systelab { namespace json { namespace rapidjson {

	JSONAdapter::JSONAdapter()
	{
	}

	JSONAdapter::~JSONAdapter() = default;

	std::unique_ptr<IJSONDocument> JSONAdapter::buildEmptyDocument() const
	{
		std::unique_ptr<::rapidjson::Document> rapidjsonDocument = std::make_unique<::rapidjson::Document>();
		return std::make_unique<JSONDocument>(std::move(rapidjsonDocument));
	}

	std::unique_ptr<IJSONDocument> JSONAdapter::buildDocumentFromString(const std::string& content) const
	{
		std::unique_ptr<::rapidjson::Document> rapidjsonDocument = std::make_unique<::rapidjson::Document>();
		rapidjsonDocument->Parse(content);
		if (!rapidjsonDocument->HasParseError())
		{
			return std::make_unique<JSONDocument>(std::move(rapidjsonDocument));
		}
		else
		{
			return std::unique_ptr<IJSONDocument>();
		}
	}

	// MODIFICATION: Schema validation methods return nullptr - see vendor/MODIFICATIONS.md
	std::unique_ptr<IJSONSchemaValidator> JSONAdapter::buildSchemaValidator(const IJSONDocument&) const
	{
		return nullptr;  // Not implemented - schema validation not needed
	}

	std::unique_ptr<IJSONSchemaValidator> JSONAdapter::buildSchemaValidator(const IJSONDocument&,
																			const IJSONRemoteSchemaProvider&) const
	{
		return nullptr;  // Not implemented - schema validation not needed
	}

}}}
