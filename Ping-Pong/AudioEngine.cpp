#include "GameWindow.h"
#include "AudioEngine.h"
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

//void AudioEngine::PlayClip()
//{
//	PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
//}
//
//void AudioEngine::PlayMusic(const std::wstring& musicName)
//{
//	//PlaySound(musicName.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
//	std::wstring command = L"open \"" + musicName + L"\" type mpegvideo alias music";
//	mciSendString(command.c_str(), NULL, 0, NULL);
//	mciSendString(L"play music repeat", NULL, 0, NULL); //repeat 
//}
//
//void AudioEngine::StopAllMusic() {
//	mciSendString(L"stop music", NULL, 0, NULL);
//	mciSendString(L"close music", NULL, 0, NULL);
//}
//
void AudioEngine::PlayClip(const std::wstring& clipName)
{
	PlaySound(clipName.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void AudioEngine::PreloadMusic(const std::wstring& musicName, const std::wstring& alias)
{
	std::wstring command = L"open \"" + musicName + L"\" type mpegvideo alias " + alias;
	mciSendString(command.c_str(), NULL, 0, NULL);
}

void AudioEngine::PlayMusic(const std::wstring& alias)
{
	std::wstring command = L"play " + alias + L" repeat ";
	mciSendString(command.c_str(), NULL, 0, NULL);
}

void AudioEngine::StopMusic(const std::wstring& alias)
{
	std::wstring command = L"stop " + alias;
	mciSendString(command.c_str(), NULL, 0, NULL);
}

void AudioEngine::PreloadSfx(const std::wstring& soundName, const std::wstring& alias)
{
	std::wstring command = L"open \"" + soundName + L"\" type waveaudio alias " + alias;
	mciSendString(command.c_str(), NULL, 0, NULL);
}

void AudioEngine::PlaySfx(const std::wstring& alias)
{
	std::wstring command = L"play " + alias;
	mciSendString(command.c_str(), NULL, 0, NULL);
}

void AudioEngine::CloseSfx(const std::wstring& alias)
{
	std::wstring command = L"close " + alias;
	mciSendString(command.c_str(), NULL, 0, NULL);
}