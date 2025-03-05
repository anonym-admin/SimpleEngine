#pragma once

class UMotionGraph
{
public:
	struct AnimClipEdge_t;

	struct AnimClipNode_t
	{
		const wchar_t* wcCurClipName = nullptr;
		AnimClipNode_t* pNext = nullptr;
		AnimClipEdge_t* pAdjList = nullptr;
		AkI32 iID = -1;
		AkBool bVisit = AK_FALSE;
	};

	struct AnimClipEdge_t
	{
		AnimClipNode_t* pFrom = nullptr;
		AnimClipNode_t* pTo = nullptr;
		AnimClipEdge_t* pNext = nullptr;
		AkF32 fBeginTime = 0.0f;
		AkF32 fEndTime = 0.0f;
	};

	static AnimClipNode_t* CreateNode(const wchar_t* wcClipName);
	static AnimClipEdge_t* Connect(AnimClipNode_t* pFrom, AnimClipNode_t* pTo, AkF32 fBeginTime, AkF32 fEndTime);

	void AddNode(AnimClipNode_t* pNode);
	void AddEdge(AnimClipNode_t* pNode, AnimClipEdge_t* pEdge);

	void Find(AnimClipNode_t* pStart, AnimClipNode_t* pEnd, AnimClipEdge_t** ppOutEdgeList, AkU32* pOutEdgeNum);

	AkU32 GetNodeCount() { return _uNodeCount++; }

private:
	AnimClipNode_t* _pNode = nullptr;
	AnimClipEdge_t* _pEdge = nullptr;
	AkU32 _uNodeCount = 0;
};

