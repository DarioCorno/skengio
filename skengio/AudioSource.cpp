#include "audiosource.h"

namespace SKEngio {
	AudioSource::AudioSource() {
		type = AudioType::Stream;
		BASS_Init(-1, 44100, 0, nullptr, nullptr);
	}

	AudioSource::~AudioSource() {
		BASS_StreamFree(stream);
	}

	void AudioSource::loadStream(const std::string& fileName, AudioType _type) {
		type = _type;
		stream = BASS_StreamCreateFile(FALSE, fileName.c_str(), 0, 0, BASS_SAMPLE_FLOAT);
	}

	void AudioSource::play() {
		BASS_ChannelPlay(stream, FALSE);
	}

	void AudioSource::stop() {
		BASS_ChannelStop(stream);
	}

	void AudioSource::setVolume(unsigned int _vol) {
		//volume range:  0 - 10000
		if (_vol > 10000)
			_vol = 10000;
		if (_vol < 0)
			_vol = 0;
		BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, _vol);
	}

	void AudioSource::updateFFT() {
		BASS_ChannelGetData(stream, fft, BASS_DATA_FFT1024);
	}

	float* AudioSource::getFFT() {
		return &fft[0];
	}
}