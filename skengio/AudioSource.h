#pragma once

#include <string>
#include <bass/bass.h>

namespace SKEngio {
	enum AudioType { Stream = 0, MOD };

	class AudioSource {
	public:
		unsigned int type{};
		HSTREAM stream{};
		float fft[512];

		AudioSource();

		~AudioSource();

		void loadStream(const std::string& fileName, AudioType _type);

		void play();
		void stop();

		void setVolume(unsigned int _vol);

		void updateFFT();
		float* getFFT();

	};
}

