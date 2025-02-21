#pragma once
#include <thread>
#include <atomic>


class AudioEngine
{
public:
	//AudioEngine();
	//~AudioEngine();

private:
	std::wstring preloadedMusicName;
	std::atomic<bool> isMusicPlaying{ false };
	std::thread musicThread;

public:
	/*void PlayMusic(const std::wstring& musicName);
	void StopAllMusic();*/
	void PlayClip(const std::wstring& clipName);

	///
	void PreloadMusic(const std::wstring& musicName, const std::wstring& alias);
	void PlayMusic(const std::wstring& alias);
	void StopMusic(const std::wstring& alias);
	void CloseMusic();

	void PreloadSfx(const std::wstring& soundName, const std::wstring& alias);
	void PlaySfx(const std::wstring& alias);
	void CloseSfx(const std::wstring& alias);
};

