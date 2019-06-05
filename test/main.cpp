
#include <vector>
#include <string>
#include <chrono>
#include <thread>

#include "OpenSoundMixer.h"

inline void Sleep(int32_t ms)
{
	std::chrono::milliseconds d(ms);
	std::this_thread::sleep_for(d);
}

template <class T>
void SafeAddRef(T& t)
{
	if (t != NULL)
	{
		t->AddRef();
	}
}

template <class T>
void SafeRelease(T& t)
{
	if (t != NULL)
	{
		t->Release();
		t = NULL;
	}
}


#if _WIN32
#include <Windows.h>
std::wstring ToWide(const char* pText);
void GetDirectoryName(char* dst, char* src);
#endif

int main(int argc, char **argv)
{
#if _WIN32
	char current_path[MAX_PATH + 1];
	GetDirectoryName(current_path, argv[0]);
	SetCurrentDirectoryA(current_path);
#endif

	osm::Sound* staticSound = nullptr;
	osm::Sound* streamSound = nullptr;

	auto manager = osm::Manager::Create();

	if( manager->Initialize() )
	{
		printf("Scceed in initializing manager.\n");
	}
	else
	{
		printf("Failed to initialize manager.\n");
		manager->Release();
		return 0;
	}

	{
		FILE* fp = nullptr;
		
#if _WIN32
		fopen_s(&fp, "se1.wav", "rb");
#else
		fp = fopen("se1.wav", "rb");
#endif
		if (fp == nullptr) return 0;

		fseek(fp, 0, SEEK_END);
		auto size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		std::vector<uint8_t> data;
		data.resize(size);
		fread(data.data(), size, 1, fp);
		fclose(fp);

		staticSound = manager->CreateSound(data.data(), data.size(), true);
	}

	{
		FILE* fp = nullptr;
#if _WIN32
		fopen_s(&fp, "bgm1.ogg", "rb");
#else
		fp = fopen("bgm1.ogg", "rb");
#endif
		if (fp == nullptr) return 0;

		fseek(fp, 0, SEEK_END);
		auto size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		std::vector<uint8_t> data;
		data.resize(size);
		fread(data.data(), size, 1, fp);
		fclose(fp);

		streamSound = manager->CreateSound(data.data(), data.size(), true);
	}

	printf("Loaded resources.\n");

	auto id1 = manager->Play(streamSound);
	manager->FadeIn(id1, 3);
	Sleep(1000);
	auto id2 = manager->Play(staticSound);
	
	while (manager->IsPlaying(id1) || manager->IsPlaying(id2))
	{
		Sleep(1);
	}

	/*
	int32_t time = 0;
	while (true)
	{
		if (time % 500 == 0)
		{
			manager->Play(streamSound);
		}
		Sleep_(1);
		time++;
	}
	*/

	manager->Finalize();
	staticSound->Release();
	streamSound->Release();
	manager->Release();

	return 0;
}



#if _WIN32
static std::wstring ToWide(const char* pText)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
	std::wstring Out(pOut);
	delete[] pOut;

	return Out;
}

void GetDirectoryName(char* dst, char* src)
{
	auto Src = std::string(src);
	int pos = 0;
	int last = 0;
	while (Src.c_str()[pos] != 0)
	{
		dst[pos] = Src.c_str()[pos];

		if (Src.c_str()[pos] == L'\\' || Src.c_str()[pos] == L'/')
		{
			last = pos;
		}

		pos++;
	}

	dst[pos] = 0;
	dst[last] = 0;
}
#endif
