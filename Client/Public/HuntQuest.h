#pragma once

#include "Client_Defines.h"
#include "Quest.h"

BEGIN(Client)

class CHuntQuest final : public CQuest
{
public:
	typedef struct tagQuestInfo
	{
		_uint iGoalCount;

	}QINFO;
private:
	CHuntQuest(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHuntQuest(const CHuntQuest& rhs);
	virtual ~CHuntQuest() = default;

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(void) override;

private:
	_uint iCount = 0;

public:
	static CHuntQuest* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CHuntQuest* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

END