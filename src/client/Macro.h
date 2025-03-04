#pragma once

// Timer.
#define DT GTimer->GetDeltaTime()
#define DT_F64 GTimer->GetF64TotalTime()

// GameInput.
#define MOUSE_X GGameInput->GetMouseX()
#define MOUSE_y GGameInput->GetMouseY()
#define R_MOUSE_X GGmaeInput->GetReleasedMouseX()
#define R_MOUSE_Y GGameInput->GetReleasedMouseY()
#define KEY_DOWN(input) GGameInput->KeyFirstDown(input)
#define KEY_HOLD(input) GGameInput->KeyHoldDown(input)
#define KEY_UP(input) GGameInput->KeyUp(input)
#define LBTN_DOWN GGameInput->LeftBtnDown();
#define LBTN_HOLD GGameInput->LeftBtnHold();
#define LBTN_UP GGameInput->LeftBtnUp();
#define RBTN_DOWN GGameInput->RightBtnDown();
#define RBTN_HOLD GGameInput->RightBtnHold();
#define RBTN_UP GGameInput->RightBtnUp();

// Renderer.
#define RENDERER GRenderer