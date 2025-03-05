#include "pch.h"
#include "ModelEditor.h"

ModelEditor::ModelEditor()
{

}

ModelEditor::~ModelEditor()
{
	EndEditor();
}

AkBool ModelEditor::BeginEditor()
{
	return AK_TRUE;
}

AkBool ModelEditor::EndEditor()
{
	return AK_TRUE;
}

void ModelEditor::Update()
{

}

void ModelEditor::FinalUpdate()
{
	ImGui::Begin("Model Editor");
	ImGui::End();
}

void ModelEditor::Render()
{

}

void ModelEditor::RenderShadow()
{

}