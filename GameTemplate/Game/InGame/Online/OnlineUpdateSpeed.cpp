#include "stdafx.h"
#include "OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	bool OnlineUpdateSpeed::Start()
	{


		return true;
	}

	OnlineUpdateSpeed::~OnlineUpdateSpeed()
	{

	}

	void OnlineUpdateSpeed::Update()
	{
		//�I�����C�����ړ��ʂ�A�j���[�V�������Q�{�ɂ���
		if (m_isOnline)
		{
			m_speed = 2.0f;
		}
		else
		{
			m_speed = 1.0f;
		}
	}
}