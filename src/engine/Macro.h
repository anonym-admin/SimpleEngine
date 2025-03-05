#pragma once

#define DT GTimer->GetDeltaTime()
#define DT_F64 GTimer->GetF64DeltaTime()

#define LBTN_DOWN GGameInput->LeftBtnDown()
#define RBTN_DOWN GGameInput->RightBtnDown()
#define LBTN_UP GGameInput->LeftBtnUp()
#define RBTN_UP GGameInput->RightBtnUp()
#define LBTN_HOLD GGameInput->LeftBtnHold()
#define RBTN_HOLD GGameInput->RightBtnHold()
#define MOUSE_X GGameInput->GetMouseX()
#define MOUSE_Y GGameInput->GetMouseY()
#define MOUSE_ACC_X GGameInput->GetAccumulatedMouseX()
#define MOUSE_ACC_Y GGameInput->GetAccumulatedMouseY()
#define KEY_DOWN(type) GGameInput->KeyFirstDown(type)
#define KEY_HOLD(type) GGameInput->KeyHoldDown(type)
#define KEY_UP(type) GGameInput->KeyUp(type)
