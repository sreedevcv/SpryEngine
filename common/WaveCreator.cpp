#include "WaveCreator.h"

WaveCreator::WaveCreator(std::string& fileName)
	: mBinFileName(fileName)
{
	int pos = fileName.find_last_of('.');
	std::string waveFileName = fileName.substr(0, pos) + ".wav";
	std::cout << waveFileName << "\n";

	mBinStream.open(fileName, std::ios::in | std::ios::binary);
	mWaveStream.open(waveFileName, std::ios::out | std::ios::binary);
}

WaveCreator::~WaveCreator()
{
	mBinStream.close();
	mWaveStream.close();
}

// Sample rate in hertz
void WaveCreator::setSampleRate(int sampleRate)
{
	mSampleRate = sampleRate;
}

// Number of channels (mono or stereo)
// 0x01 - Mono
// 0x02 - Stereo
void WaveCreator::setChannelCount(int count)
{
	if (count != 1 && count != 2) {
		// Handle Error
	}

	mChannelCount = count;
}

// The size of each sample in bits
void WaveCreator::setSampleSize(int sampleSize)
{
	if (sampleSize % 8 != 0) {
		// Handle Error
	}
	mSampleSize = sampleSize;
}

void WaveCreator::writeWaveFile()
{
	int fileSize = std::filesystem::file_size(mBinFileName);
	int bytesPerSecond = mSampleSize * mSampleRate / 8;
	int bytesPerSample = mSampleSize * mChannelCount / 8;

	char byteArray[4];

	mWaveStream.seekp(0, std::ios_base::beg);

	// RIFF Chunk
	mWaveStream.write("RIFF", 4);
	mWaveStream.write(toByteArray(byteArray, static_cast<uint32_t>(36 + fileSize)), 4);
	mWaveStream.write("WAVE", 4);

	// FORMAT Chunk
	mWaveStream.write("fmt ", 4);
	mWaveStream.write(toByteArray(byteArray, static_cast<uint32_t>(16)), 4);			// Size of Format Chunk
	mWaveStream.write(toByteArray(byteArray, static_cast<uint32_t>(1)), 2);				// Always one
	mWaveStream.write(toByteArray(byteArray, static_cast<uint32_t>(mChannelCount)), 2);
	mWaveStream.write(toByteArray(byteArray, static_cast<uint32_t>(mSampleRate)), 4);
	mWaveStream.write(toByteArray(byteArray, static_cast<uint32_t>(bytesPerSecond)), 4);
	mWaveStream.write(toByteArray(byteArray, static_cast<uint32_t>(bytesPerSample)), 2);
	mWaveStream.write(toByteArray(byteArray, static_cast<uint32_t>(bytesPerSample * 8)), 2);

	// DATA Chunk
	mWaveStream.write("data", 4);
	mWaveStream.write(toByteArray(byteArray, static_cast<uint32_t>(fileSize)), 4);
	mWaveStream << mBinStream.rdbuf();
	mWaveStream.flush();
}

char* WaveCreator::toByteArray(char* array, uint32_t value)
{
	array[0] = value & 0xFF;
	array[1] = (value >> 8) & 0xFF;
	array[2] = (value >> 16) & 0xFF;
	array[3] = (value >> 24) & 0xFF;
	return array;
}