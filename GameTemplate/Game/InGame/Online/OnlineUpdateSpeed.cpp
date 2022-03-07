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
		//オンライン時移動量やアニメーションを２倍にする
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