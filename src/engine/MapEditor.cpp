#include "pch.h"
#include "MapEditor.h"

MapEditor::MapEditor()
{

}

MapEditor::~MapEditor()
{
	EndEditor();
}

AkBool MapEditor::BeginEditor()
{
	return AK_TRUE;
}

AkBool MapEditor::EndEditor()
{
	return AK_TRUE;
}

void MapEditor::Update()
{

}

void MapEditor::FinalUpdate()
{
	ImGui::Begin("Map Editor");
	ImGui::End();
}

void MapEditor::Render()
{

}

void MapEditor::RenderShadow()
{

}