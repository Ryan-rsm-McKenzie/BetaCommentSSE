#include "skse64_common/skse_version.h"

#include "BetaComment.h"
#include "Settings.h"
#include "version.h"

#include "SKSE/API.h"


namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kDataLoaded:
			{
				auto betaComment = BetaComment::GetSingleton();
				betaComment->Register();
			}
			break;
		}
	}
}


extern "C" {
	bool SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
	{
		SKSE::Logger::OpenRelative(FOLDERID_Documents, L"\\My Games\\Skyrim Special Edition\\SKSE\\BetaCommentSSE.log");
		SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kDebugMessage);
		SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kDebugMessage);

		_MESSAGE("BetaCommentSSE v%s", BTCM_VERSION_VERSTRING);

		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = "BetaCommentSSE";
		a_info->version = BTCM_VERSION_MAJOR;

		if (a_skse->IsEditor()) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
			return false;
		}

		switch (a_skse->RuntimeVersion()) {
		case RUNTIME_VERSION_1_5_73:
		case RUNTIME_VERSION_1_5_80:
			break;
		default:
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->RuntimeVersion());
			return false;
		}

		return true;
	}


	bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
	{
		_MESSAGE("[MESSAGE] BetaCommentSSE loaded");

		if (!SKSE::Init(a_skse)) {
			return false;
		}

		if (!Settings::loadSettings()) {
			_FATALERROR("[FATAL ERROR] Failed to load settings");
			return false;
		} else {
			_MESSAGE("[MESSAGE] Loaded settings");
		}

		auto messaging = SKSE::GetMessagingInterface();
		messaging->RegisterListener("SKSE", MessageHandler);

		return true;
	}
};
