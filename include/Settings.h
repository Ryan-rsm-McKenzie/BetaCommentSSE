#pragma once

#include "Json2Settings.h"  // Json2Settings


class Settings : public Json2Settings::Settings
{
public:
	Settings() = delete;
	static bool loadSettings(bool a_dumpParse = false);


	static sSetting filePath;

private:
	static inline constexpr char FILE_NAME[] = "Data\\SKSE\\Plugins\\BetaCommentSSE.json";
};
