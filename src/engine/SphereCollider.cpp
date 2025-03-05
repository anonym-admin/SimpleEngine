#include "pch.h"
#include "SphereCollider.h"
#include "GeometryGenerator.h"

SphereCollider::SphereCollider(AkF32 fRadius, AkU32 uStack, AkU32 uSlice)
{
    if (!Initialize(fRadius, uStack, uSlice))
    {
        __debugbreak();
    }
}

SphereCollider::~SphereCollider()
{
}

AkBool SphereCollider::Initialize(AkF32 fRadius, AkU32 uStack, AkU32 uSlice)
{
    _eType = COLLIDER_TYPE::COLLIDER_TYPE_SPHERE;
    
    // Create Mesh.
    AkU32 uMeshDataNum = 0;
    MeshData_t* pMeshData = UGeometryGenerator::MakeSphere(&uMeshDataNum, fRadius, uStack, uSlice);
    _pMeshObj = GRenderer->CreateBasicMeshObject();
    _pMeshObj->CreateMeshBuffers(pMeshData, uMeshDataNum);
    _pMeshObj->EnableWireFrame();
    UGeometryGenerator::DestroyGeometry(pMeshData, uMeshDataNum);

    // Update Material.
    Vector3 vAlbedo = Vector3(0.0f);
    _pMeshObj->UpdateMaterialBuffers(&vAlbedo, 0.0f, 1.0f, &_vColor);

    return AK_TRUE;
}

void SphereCollider::OnCollisionEnter(Collider* pOther)
{
}

void SphereCollider::OnCollision(Collider* pOther)
{
}

void SphereCollider::OnCollisionExit(Collider* pOther)
{
}
