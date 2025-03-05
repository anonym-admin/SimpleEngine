#include "pch.h"
#include "Swat.h"
#include "AssetManager.h"
#include "SkinnedModel.h"
#include "Animator.h"
#include "Transform.h"
#include "Controller.h"
#include "Collider.h"
#include "Camera.h"

Swat::Swat()
{
    if (!Initialize())
    {
        __debugbreak();
    }
}

Swat::~Swat()
{
    CleanUp();
}

AkBool Swat::Initialize()
{
    // Create Model.
    AssetMeshDataContainer_t* pMeshDataContainer = GAssetManager->GetMeshDataContainer(ASSET_MESH_DATA_TYPE::ASSET_MESH_DATA_TYPE_SWATGUY);
    Vector3 vAlbedo = Vector3(1.0f);
    AkF32 fMetallic = 0.0f;
    AkF32 fRoughness = 1.0f;
    Vector3 vEmissive = Vector3(0.0f);
    _pModel = CreateModel(pMeshDataContainer, &vAlbedo, fMetallic, fRoughness, &vEmissive, AK_TRUE);
    GAssetManager->DeleteMeshData(ASSET_MESH_DATA_TYPE::ASSET_MESH_DATA_TYPE_SWATGUY);

    // Bind Animation.
    SkinnedModel* pSkinnedModel = (SkinnedModel*)_pModel;
    pSkinnedModel->BindAnimation(GAnimator->GetAnimation(GAME_ANIMATION_TYPE::GAME_ANIM_TYPE_PLAYER));

    // Create Controller.
    _pController = CreateController();

    // Create Trnasform.
    _pTransform = CreateTransform();

    // Create Collider.
    _pCollider = CreateCollider(COLLIDER_TYPE::COLLIDER_TYPE_SPHERE);

    // Create Camera.
    Vector3 vCamPos = Vector3(0.0f, 0.0f, -5.0f);
    _pCamera = CreateCamera(&vCamPos);

    return AK_TRUE;
}

void Swat::Update()
{
    _pController->Update();


}

void Swat::FinalUpdate()
{
    _pTransform->Update();

    _pCollider->Update();

    ((SkinnedModel*)_pModel)->PlayAnimation(GAME_ANIM_PLAYER_ANIM_FILE_NAME[0], AK_FALSE);
    
    _pModel->UpdateWorldRow(&_pTransform->WorldTransform);

    _pCamera->Update();
}

void Swat::Render()
{
    _pModel->Render();
}

void Swat::RenderShadow()
{
    _pModel->RenderShadow();
}

void Swat::OnCollisionEnter(Collider* pOther)
{
}

void Swat::OnCollision(Collider* pOther)
{
}

void Swat::OnCollisionExit(Collider* pOther)
{
}

void Swat::CleanUp()
{
}
