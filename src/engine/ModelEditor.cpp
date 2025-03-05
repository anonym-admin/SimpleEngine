#include "pch.h"
#include "ModelEditor.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Actor.h"
#include "SkinnedModel.h"
#include "Animator.h"
#include "Animation.h"
#include "Camera.h"

ModelEditor::ModelEditor()
{

}

ModelEditor::~ModelEditor()
{
	EndEditor();
}

AkBool ModelEditor::BeginEditor()
{
	Scene* pScene = GSceneManager->GetScene(SCENE_TYPE::SCENE_TYPE_INGANE);
	GameObjContainer_t* pGameObjContainer = pScene->GetGroupObject(GAME_OBJECT_GROUP_TYPE::GAME_OBJ_GROUP_TYPE_PLAYER);

	List_t* pCur = pGameObjContainer->pGameObjHead;
	while (pCur != nullptr)
	{
		if (!wcscmp(((Actor*)pCur->pData)->Name, L"Swat"))
			break;

		pCur = pCur->pNext;
	}

	if (pCur != nullptr)
	{
		_pTarget = ((Actor*)pCur->pData)->GetModel();

		((SkinnedModel*)_pTarget)->BindAnimation(GAnimator->GetAnimation(GAME_ANIMATION_TYPE::GAME_ANIM_TYPE_PLAYER));

		_vSceneCamPos = ((Actor*)pCur->pData)->GetCamera()->GetPosition();
	}

	Vector3 vCamPos = Vector3(0.0f, 0.0f, -2.0f);
	Vector3 vYawPitchRoll = Vector3(0.0f);
	_pCamera = new Camera(&vCamPos, &vYawPitchRoll);
	_pCamera->Mode = CameraMode::EDITOR;

	return AK_TRUE;
}

AkBool ModelEditor::EndEditor()
{
	if (_pCamera)
	{
		delete _pCamera;
		_pCamera = nullptr;
	}

	_pTarget = nullptr;

	return AK_TRUE;
}

void ModelEditor::Update()
{
	((SkinnedModel*)_pTarget)->GetAnimation()->PlayAnimationBlending(GAME_ANIM_PLAYER_ANIM_FILE_NAME[0], GAME_ANIM_PLAYER_ANIM_FILE_NAME[1]);
	// ((SkinnedModel*)_pTarget)->GetAnimation()->PlayAnimationWithFrame(GAME_ANIM_PLAYER_ANIM_FILE_NAME[0], _uFrame);

	_pCamera->Update();
}

void ModelEditor::FinalUpdate()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;

	ImGui::Begin("Model Editor");
	if (ImGui::Button("Load Model File"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".md3d");
	}
	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
		}
		ImGuiFileDialog::Instance()->Close();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Anim File"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".anim");
	}
	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
		}
		ImGuiFileDialog::Instance()->Close();
	}

	// ListBox 생성 (Wide Char 항목 지원)
	static int SelectedID = 0;
	if (ImGui::ListBoxHeader("Fruits", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (AkU32 i = 0; i < IM_ARRAYSIZE(GAME_ANIM_PLAYER_ANIM_FILE_NAME); i++) {
			const AkBool uIsSelected = (SelectedID == i);
			if (ImGui::Selectable(reinterpret_cast<const char*>(GAME_ANIM_PLAYER_ANIM_FILE_NAME[i]), uIsSelected)) {
				SelectedID = i;
				_uSelectedAnimClip = SelectedID;
			}
			if (uIsSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::ListBoxFooter();
	}

	ImGui::SliderInt("Frame", (int*)&_uFrame, 0, 126);
	ImGui::End();
}

void ModelEditor::Render()
{
	_pTarget->Render();
}

void ModelEditor::RenderShadow()
{
	_pTarget->RenderShadow();
}

void ModelEditor::Load()
{

}

void ModelEditor::Save()
{
}
