#pragma once

#include <string>

namespace Tmpl8
{

	struct TimeScore {
		char level[8];
		float time;
		bool unlocked;
};
	// Setters
	void writeHighScores(const std::string& levelName, float newScore);

	//getters
	float getHighScore(const std::string& levelName);
	bool isLocked(const std::string& levelName);
	void readHighScores(int level);
}