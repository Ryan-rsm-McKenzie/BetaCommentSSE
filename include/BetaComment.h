#pragma once

#include <lmcons.h>

#include <fstream>
#include <string>

#include "RE/Skyrim.h"


class BetaComment
{
public:
	static BetaComment* GetSingleton();
	static bool Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr);

	void Register();

private:
	BetaComment();
	BetaComment(const BetaComment&) = delete;
	BetaComment(BetaComment&&) = delete;
	~BetaComment() = default;

	BetaComment& operator=(const BetaComment&) = delete;
	BetaComment& operator=(BetaComment&&) = delete;

	static void CPrint(const char* a_fmt, ...);

	void LogComment(RE::TESObjectREFR* a_ref, const std::string& a_comment);


	std::ofstream _file;
	CHAR _userName[UNLEN + 1];
};
