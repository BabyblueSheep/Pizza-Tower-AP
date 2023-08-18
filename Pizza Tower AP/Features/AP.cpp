#include "AP.hpp"
#include "Utils.hpp"

#include <string>


#ifdef CLR_DEFAULT
#undef CLR_DEFAULT
#endif


APClient* AP::Client;
#define apClient AP::Client

bool AP::IsDeathLink;


YYTKStatus AP::APStartConnection() {
	std::string ip = Utils::CallBuiltinWrapper(nullptr, "variable_global_get", { "ap_ip" }).As<RValue>().String->Get();
	std::string name = Utils::CallBuiltinWrapper(nullptr, "variable_global_get", { "ap_name" }).As<RValue>().String->Get();
	std::string password = Utils::CallBuiltinWrapper(nullptr, "variable_global_get", { "ap_pass" }).As<RValue>().String->Get();

	apClient = new APClient(ap_get_uuid(name), "Pizza Tower", BuildURI(ip));
	apClient->set_room_info_handler(AP::APOnRoomInfo);
	apClient->set_slot_connected_handler(AP::APOnConnected);
	apClient->set_slot_refused_handler(AP::APOnConnectError);
	apClient->set_items_received_handler(AP::APOnGetItems);
	apClient->set_print_json_handler(AP::APOnPrintJSON);

	return YYTK_OK;
}

void AP::APOnConnectError(const std::list<std::string>& reason) {
	for (const std::string& i : reason) {
		PrintMessage(CLR_RED, "Didn't manage to connect. Reason: %s", i);
	}
}

void AP::APOnRoomInfo() {
	std::string ip = Utils::CallBuiltinWrapper(nullptr, "variable_global_get", { "ap_ip" }).As<RValue>().String->Get();
	std::string name = Utils::CallBuiltinWrapper(nullptr, "variable_global_get", { "ap_name" }).As<RValue>().String->Get();
	std::string password = Utils::CallBuiltinWrapper(nullptr, "variable_global_get", { "ap_pass" }).As<RValue>().String->Get();

	std::list<std::string> tags;
	if (AP::IsDeathLink)
		tags.push_back("DeathLink");

	apClient->ConnectSlot(name, password, 5, tags, { 0,4,2 });
}

void AP::APOnConnected(const nlohmann::json& data) {
	data.contains("death_link") ? data.at("death_link").get_to(AP::IsDeathLink) : AP::IsDeathLink = false;

	std::list<std::string> tags;
	if (AP::IsDeathLink) {
		tags.push_back("DeathLink");
		apClient->ConnectUpdate(false, 1, true, tags);
	}
}

void AP::APOnGetItems(const std::list<APClient::NetworkItem>& items) {
	for (const APClient::NetworkItem& item : items) {
		PrintMessage(CLR_DEFAULT, "test");
		Utils::CallBuiltinWrapper(nullptr, "activate_item", { std::to_string(item.item), "temp"});
	}
}

void AP::APOnPrintJSON(const std::list<APClient::TextNode>& msg) {
	PrintMessage(CLR_AQUA, apClient->render_json(msg).c_str());
}



std::string AP::BuildURI(std::string& server)
{
	std::string uri = server;

	if (uri.find(":") == std::string::npos)
		uri = uri + ":38281";
	else if (uri.back() == ':')
		uri = uri + "38281";

	return uri;
}