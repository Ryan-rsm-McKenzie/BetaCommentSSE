#include "settings.h"

#include "Json2Settings.h"  // Json2Settings


bool Settings::loadSettings(bool a_dumpParse)
{
	return Json2Settings::Settings::loadSettings(FILE_NAME, a_dumpParse);
}


decltype(Settings::filePath) Settings::filePath("filePath", "betacomments.txt");
