#include "ScoreFileEditor.hpp"

#include <fstream>
#include <iostream>

namespace Tmpl8 {

    void writeHighScores(const std::string& levelName, float newScore)
    {
        readHighScores(0);

        std::fstream highscores("assets/LevelScoreData.bin", std::ios::in | std::ios::out | std::ios::binary);
        if (!highscores)
        {
            std::cerr << "ERROR could not open LevelScoreData.bin for updating" << std::endl;
            return;
        }

        size_t numEntries = 0;
        highscores.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries)); // Here the numEntries is actually set

        for (size_t i = 0; i < numEntries; ++i)
        {
            std::streampos pos = highscores.tellg(); // tellg returns the position of the input stream
            TimeScore score;
            highscores.read(reinterpret_cast<char*>(&score), sizeof(TimeScore));

            
            // Compare level names (ensure null-termination)
            if (std::string(score.level) == levelName)

            {
                // Only update if the new score is higher than the old one
                if (newScore < static_cast<int>(score.time) || score.time < 10.f ) // if score is 0 then it means that this is the players first time playing the level and maybe for debug purpuses its faster then 10 sec.
                {
                    score.time = static_cast<float>(newScore);

                    highscores.seekp(pos);
                    highscores.write(reinterpret_cast<char*>(&score), sizeof(TimeScore));
                }
                break;
            }
        }
        highscores.close();

        readHighScores(0);
    }

    float getHighScore(const std::string& levelName)
    {
        std::ifstream playerScores("LevelScoreData.bin", std::ios::binary);
        if (!playerScores)
        {
            std::cerr << "ERROR could not open LevelScoreData.bin for reading" << std::endl;
            return 1.0f; // 1.0f for error handling
        }
        size_t numEntries = 0;

        playerScores.read(reinterpret_cast<char*>(&numEntries), numEntries * sizeof(TimeScore));

        TimeScore* scores = new TimeScore[numEntries]; // dynamically allocate an array of HighScore structs  

        playerScores.read(reinterpret_cast<char*>(scores), numEntries * sizeof(TimeScore)); // read the entire array as binary data  


        if (playerScores.bad())
        {
            std::cerr << "ERROR while reading the LevelScoreData.bin file" << std::endl;
            delete[] scores;
            playerScores.close();
            return 1.0f;
        }

        for (size_t i = 0; i < numEntries; ++i)
        {
            if (levelName.c_str() == scores[i].level)
            {

                float score = scores[i].time;
                delete[] scores;
                playerScores.close();
                return score;
            }
        }
        delete[] scores;
        playerScores.close();
        return 0.0f;
    }

    bool isLocked(const std::string& levelName)
    {
        std::ifstream playerScores("LevelScoreData.bin", std::ios::binary);
        if (!playerScores)
        {
            std::cerr << "ERROR could not open LevelScoreData.bin for reading" << std::endl;
            return false; 
        }
        size_t numEntries = 0;

        playerScores.read(reinterpret_cast<char*>(&numEntries), numEntries * sizeof(TimeScore));

        TimeScore* scores = new TimeScore[numEntries]; // dynamically allocate an array of HighScore structs  

        playerScores.read(reinterpret_cast<char*>(scores), numEntries * sizeof(TimeScore)); // read the entire array as binary data  


        if (playerScores.bad())
        {
            std::cerr << "ERROR while reading the LevelScoreData.bin file" << std::endl;
            delete[] scores;
            playerScores.close();
            return false;
        }

        for (size_t i = 0; i < numEntries; ++i)
        {
            if (levelName.c_str() == scores[i].level)
            {

                bool unlocked = scores[i].unlocked;
                delete[] scores;
                playerScores.close();
                return unlocked;
            }
        }
        delete[] scores;
        playerScores.close();
        return false;

    }

    void readHighScores(int level) // level to set oldTime to  
    {
        std::ifstream highscores("assets/LevelScoreData.bin", std::ios::binary); // binary mode  
        if (!highscores)
        {
            std::cerr << "ERROR could not open LevelScoreData.bin for reading" << std::endl;
            return;
        }

        size_t numEntries = 0; // Initialize the variable to fix lnt-uninitialized-local error  

        highscores.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries)); // read the number of scores as binary data  

        TimeScore* scores = new TimeScore[numEntries]; // dynamically allocate an array of HighScore structs  

        highscores.read(reinterpret_cast<char*>(scores), numEntries * sizeof(TimeScore)); // read the entire array as binary data  

        if (highscores.bad())
        {
            std::cerr << "ERROR while reading LevelScoreData.bin" << std::endl;
            delete[] scores; // Ensure memory cleanup before returning  
            return;
        }
\
        for (size_t i = 0; i < numEntries; ++i)
        {
            std::cout << scores[i].level << " Time: " << scores[i].time << std::endl;
        }

        delete[] scores;
        highscores.close();
    }
}