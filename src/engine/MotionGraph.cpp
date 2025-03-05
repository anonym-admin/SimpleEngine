#include "pch.h"
#include "MotionGraph.h"

UMotionGraph::AnimClipNode_t* UMotionGraph::CreateNode(const wchar_t* wcClipName)
{
    UMotionGraph::AnimClipNode_t* pNode = new UMotionGraph::AnimClipNode_t;
    pNode->wcCurClipName = wcClipName;
    pNode->pNext = nullptr;
    pNode->pAdjList = nullptr;
    pNode->iID = -1;
    pNode->bVisit = AK_FALSE;
    return pNode;
}

UMotionGraph::AnimClipEdge_t* UMotionGraph::Connect(UMotionGraph::AnimClipNode_t* pFrom, UMotionGraph::AnimClipNode_t* pTo, AkF32 fBeginTime, AkF32 fEndTime)
{
    UMotionGraph::AnimClipEdge_t* pEdge = new UMotionGraph::AnimClipEdge_t;
    pEdge->pFrom = pFrom;
    pEdge->pTo = pTo;
    pEdge->fBeginTime = fBeginTime;
    pEdge->fEndTime = fEndTime;
    pEdge->pNext = nullptr;
    return pEdge;
}

void UMotionGraph::AddNode(UMotionGraph::AnimClipNode_t* pNode)
{
    AnimClipNode_t* pCur = _pNode;
    if (nullptr == pCur)
    {
        _pNode = pNode;
    }
    else
    {
        while (pCur->pNext != nullptr)
        {
            pCur = pCur->pNext;
        }
        pCur->pNext = pNode;
    }
    pNode->iID = (AkI32)_uNodeCount++;
}

void UMotionGraph::AddEdge(UMotionGraph::AnimClipNode_t* pNode, UMotionGraph::AnimClipEdge_t* pEdge)
{
    if (nullptr == pNode->pAdjList)
    {
        pNode->pAdjList = pEdge;
    }
    else
    {
        AnimClipEdge_t* pCur = pNode->pAdjList;
        while (pCur->pNext != nullptr)
        {
            pCur = pCur->pNext;
        }
        pCur->pNext = pEdge;
    }
}

void UMotionGraph::Find(AnimClipNode_t* pStart, AnimClipNode_t* pEnd, AnimClipEdge_t** ppOutEdgeList, AkU32* pOutEdgeNum)
{
    
}
