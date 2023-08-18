#pragma once
#define YYSDK_PLUGIN
#include "../SDK/SDK.hpp"
#include "apclient.hpp"
#include "apuuid.hpp"

#include <string>

namespace AP
{
	extern APClient* Client;

	extern bool IsDeathLink;


	YYTKStatus APStartConnection();

	void APOnRoomInfo();
	void APOnConnected(const nlohmann::json&);
	void APOnConnectError(const std::list<std::string>&);

	void APOnGetItems(const std::list<APClient::NetworkItem>&);

	void APOnPrintJSON(const std::list<APClient::TextNode>&);


	std::string BuildURI(std::string& server);
}