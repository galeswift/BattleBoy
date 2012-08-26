#pragma once

// Standard headers
#include <assert.h>
#include <string>
#include <process.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>

// Windows specific headers
#if defined (_WIN32)
#include <WinSock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
#endif 

// Generic
#include "CommandOptions.h"

// Networking
#include "Networking.h"

// BoyLib
#include "BoyLib/BoyUtil.h"
#include "BoyLib/Vector2.h"

// Boy
#include "Boy/Environment.h"
#include "Boy/Font.h"
#include "Boy/Game.h"
#include "Boy/Graphics.h"
#include "Boy/KeyboardListener.h"
#include "Boy/Mouse.h"
#include "Boy/ResourceManager.h"
#include "Boy/SoundPlayer.h"

// Battleboy
#include "Actor.h"
#include "Board.h"
#include "BattleBoy.h"