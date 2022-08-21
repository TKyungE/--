#pragma once

#include "Base.h"

/* 게임객체들의 부모가되는 클래스다. */

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
public:
	typedef struct tagInfo
	{
		CGameObject* pTarget;
		_float3		 vPos;
		_float       fX; // (객체의 지름) 객체 생성할때 이니셜라이즈에서 스케일 맥인거 대입해줘야함.
		_int		 iMaxHp;
		_int		 iHp;
		_int		 iMp;
		_int		 iDmg;
		_int		 iExp;
		_int		 iMaxExp;
		_int		 iMoney;
	}INFO;
	typedef struct tagFrameInfo
	{
		_int				iFrameStart;
		_int				iFrameEnd;
		_float				fFrameSpeed;
	}FrameInfo;
protected:
	INFO m_tInfo;
	FrameInfo				m_tFrame;
public: // 여기에 갯 셋 만들어서 인포 +- 관리하면될듯
	INFO	Get_Info() { return m_tInfo; }
protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;

protected:
	map<const _tchar*, class CComponent*>				m_Components;

protected:
	HRESULT Add_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);

protected:
	class CComponent* Find_Component(const _tchar* pComponentTag);

public:	
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual _float4x4 Get_World(void) = 0;
	virtual void Free() override;
};

END

