#pragma once

// Standard header
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <process.h>
#include <conio.h>

// Inner inc
#include "common/AkType.h"
#include "common/AkEnum.h"
#include "common/AkMeshData.h"
#include "common/AkMath.h"

#include "utils/LinkedList.h"
#include "utils/HashTable.h"
#include "utils/UtilFunc.h"
#include "utils/RedBlackTree.h"
#include "utils/Queue.h"

#include "interface/IRenderer.h"

#include "Global.h"
#include "Macro.h"
#include "Type.h"
#include "Enum.h"

// Vender inc
#include "fmod.hpp"
#include "fmod_errors.h"
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#include <ImGuizmo.h>

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif