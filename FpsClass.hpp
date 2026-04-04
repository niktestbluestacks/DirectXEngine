#pragma once
#ifndef _FPSCLASS_HPP_
#define _FPSCLASS_HPP_

// Linking
#pragma comment(lib, "winmm.lib")

// Includes
#include <Windows.h>
#include <mmsystem.h>
#include <cstdint>
// Class name : FpsClass

class FpsClass {
public:
	FpsClass();
	FpsClass(const FpsClass& other);
	~FpsClass();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	uint64_t m_startTime;
};
#endif // _FPSCLASS_HPP_

