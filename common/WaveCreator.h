#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

class WaveCreator {
private:
	std::string mBinFileName;
	std::ifstream mBinStream;
	std::ofstream mWaveStream;

	int mSampleRate;
	int mChannelCount;
	int mSampleSize;

	char* toByteArray(char* array, uint32_t value);

public:
	WaveCreator(std::string& binFileName);
	~WaveCreator();

	void setSampleRate(int sampleRate);
	void setChannelCount(int count);
	void setSampleSize(int sampleSize);

	void writeWaveFile();
};