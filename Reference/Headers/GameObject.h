#pragma once

#include "Base.h"

/* ���Ӱ�ü���� �θ𰡵Ǵ� Ŭ������. */

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	class CComponent* Find_Component(const _tchar* pComponentTag);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
public:
	typedef struct tagInfo
	{
		CGameObject* pTerrain;
		CGameObject* pTarget;
		_float3		 vPos;
		_float3		 vTargetPos;
		_float3		 vScale;		// �ӽÿ��̱� ������ ��ü���� ����� �ʿ��� ���� ���� ������..
		_float       fX; // (��ü�� ����) ��ü �����Ҷ� �̴ϼȶ������ ������ ���ΰ� �����������.
		_int		 iMaxHp;
		_int		 iHp;
		_int		 iMp;
		_int		 iDmg;
		_int	     iTargetDmg;
		_int		 iExp;
		_int		 iMaxExp;
		_int		 iMoney;
		_bool		 bDead;
		_bool		 bHit;
		_int		 iLevelIndex;

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
public: // ���⿡ �� �� ���� ���� +- �����ϸ�ɵ�
	INFO	Get_Info() { return m_tInfo; }
	void	Set_Info(INFO& _tInfo){ memcpy(&m_tInfo, &_tInfo, sizeof(INFO)); }
	void	Set_Dead() { m_tInfo.bDead = true; }
	void	Set_LevelIndex(_int _iLevelIndex) { m_tInfo.iLevelIndex = _iLevelIndex; };
	void	Set_Hp(_int _iDmg) { m_tInfo.iHp -= _iDmg; }
	void	Set_Hit(_int _iDmg, _float3 _vPos) { m_tInfo.bHit = true; m_tInfo.iTargetDmg = _iDmg; m_tInfo.vTargetPos = _vPos; }
protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;

protected:
	map<const _tchar*, class CComponent*>				m_Components;

protected:
	HRESULT Add_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag,class CComponent** ppOut, void* pArg = nullptr);

public:	
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual _float4x4 Get_World(void) = 0;
	virtual void Free() override;
};

END

