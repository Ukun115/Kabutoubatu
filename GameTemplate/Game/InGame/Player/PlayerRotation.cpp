/// <summary>
/// �v���C���[�̉�]�N���X
/// </summary>
#include "stdafx.h"
#include "PlayerRotation.h"
#include "Player.h"

namespace nsKabutoubatu
{
	bool PlayerRotation::Start()
	{
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_playerNum]);

		return true;
	}

	void PlayerRotation::Update()
	{
		//�V���b�s���O������Ȃ��Ƃ��͈ړ����x�����]�p�x�����߂�
		if (m_player->GetNowState() != 3)
		{
			//��]�p�x�����߂�
			m_rotAngle = atan2(m_player->GetMoveSpeed().x, m_player->GetMoveSpeed().z);
		}
		//��]���X�V
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
	}
}