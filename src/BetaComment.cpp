#include "BetaComment.h"

#include <timezoneapi.h>

#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <type_traits>

#include "Settings.h"


BetaComment* BetaComment::GetSingleton()
{
	static BetaComment singleton;
	return &singleton;
}


bool BetaComment::Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
{
	auto mm = RE::MenuManager::GetSingleton();
	auto uiStrHolder = RE::UIStringHolder::GetSingleton();
	auto console = mm->GetMenu<RE::Console>(uiStrHolder->console);
	auto selectedRefHandle = RE::Console::GetSelectedRef();
	RE::TESObjectREFRPtr selectedRef;
	RE::TESObjectREFR::LookupByHandle(selectedRefHandle, selectedRef);
	if (!selectedRef) {
		CPrint("> [BetaComment] ERROR: No selected reference");
		return true;
	}

	auto strChunk = static_cast<RE::CommandInfo::StringChunk*>(a_scriptData->GetChunk());
	auto comment = strChunk->GetString();
	auto singleton = GetSingleton();
	singleton->LogComment(selectedRef.get(), comment);

	return true;
}


void BetaComment::Register()
{
	using Type = RE::SCRIPT_PARAMETER::Type;

	auto info = RE::CommandInfo::LocateConsoleCommand("BetaComment");  // Unused
	if (info) {
		static RE::SCRIPT_PARAMETER params[] = {
			{ "Comment", Type::kString, 0 }
		};
		info->longName = "BetaComment";
		info->shortName = "BC";
		info->helpText = "<BetaComment|BC> <comment>";
		info->isRefRequired = false;
		info->SetParameters(params);
		info->execute = &Exec;
		info->eval = 0;
		info->flags = 0;

		_MESSAGE("[MESSAGE] Registered console command: %s (%s)", info->longName, info->shortName);
	} else {
		_ERROR("[ERROR] Failed to register console command!\n");
	}
}

BetaComment::BetaComment() :
	_file(),
	_userName{ 0 }
{
	_file.open(Settings::filePath.c_str());
	if (!_file.is_open()) {
		_FATALERROR("[FATAL ERROR] Failed to open output file");
		std::abort();
	}

	DWORD userNameSize = sizeof(_userName);
	if (!GetUserName(_userName, &userNameSize)) {
		_ERROR("[ERROR] Failed to get username with error code (%i)", GetLastError());
		strcpy_s(_userName, sizeof(_userName), "SEE_LOG_FOR_ERROR");
	}
}


void BetaComment::CPrint(const char* a_fmt, ...)
{
	auto console = RE::ConsoleManager::GetSingleton();
	if (console && console->IsConsoleMode()) {
		std::va_list args;
		va_start(args, a_fmt);
		console->Print(a_fmt, args);
		va_end(args);
	}
}


void BetaComment::LogComment(RE::TESObjectREFR* a_ref, const std::string& a_comment)
{
	std::stringstream line;

	auto time = std::time(0);
	std::tm localTime;
	auto err = localtime_s(&localTime, &time);
	if (err) {
		CPrint("> [BetaComment] ERROR: Failed to get local time with error code (%i)", err);
		return;
	} else {
		line << std::put_time(&localTime, "%m/%d/%Y (%H:%M)") << ' ';
	}

	auto source = a_ref->GetFinalSourceFile();
	if (!source) {
		CPrint("> [BetaComment] ERROR: Failed to get last modified source file for selected ref", err);
		return;
	} else {
		line << source->name << ' ';
	}

	SYSTEMTIME fileTime;
	std::memset(&fileTime, 0, sizeof(fileTime));
	if (!FileTimeToSystemTime(&source->fileData.ftLastWriteTime, &fileTime)) {
		CPrint("> [BetaComment] ERROR: Failed to convert file time to system time with error code (%i)", GetLastError());
		return;
	} else {
		line << fileTime.wMonth << '/' << fileTime.wDay << '/' << fileTime.wYear << " (" << fileTime.wHour << ':' << fileTime.wSecond << ") ";
	}

	line << _userName << ' ';

	auto cell = a_ref->GetParentCell();
	auto coordinates = cell->GetCoordinates();
	if (coordinates) {
		line << '(' << coordinates->x << ',' << coordinates->y << ") ";
	}

	line << std::floor(a_ref->pos.x) << ' ' << std::floor(a_ref->pos.y) << ' ' << std::floor(a_ref->pos.z) << ' ';
	line << '\"' << a_comment << '\"';

	_file << line.str() << '\n';
	_file.flush();
}
