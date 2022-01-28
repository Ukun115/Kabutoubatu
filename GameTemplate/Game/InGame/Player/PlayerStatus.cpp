/// <summary>
/// �v���C���[�̃X�e�[�^�X�N���X
/// </summary>
#include "stdafx.h"
#include "PlayerStatus.h"
#include "PlayerSound.h"

namespace nsKabutoubatu
{
	bool PlayerStatus::Start()
	{
		m_playerSound = FindGO< PlayerSound >(nsStdafx::PLAYER_SOUND_NAME[m_playerNum]);

		return true;
	}

	PlayerStatus::~PlayerStatus()
	{

	}

	void PlayerStatus::Update()
	{
		//�q�b�g�|�C���g�̕␳
		m_hitPoint = min(m_maxHitPoint, m_hitPoint);	//�q�b�g�|�C���g���ő�̗͈ȏ�ɂȂ�Ȃ��悤�ɂ���
		m_hitPoint = max(0, m_hitPoint);		//�q�b�g�|�C���g��0�ȉ��ɂȂ�Ȃ��悤�ɂ���
	}

	//�v���C���[�̃R�C���𑝂₷���\�b�h
	void PlayerStatus::AddCoin(const int coinValue)
	{
		//�����R�C�����𑝂₷
		m_possessionCoin += int(coinValue * m_addMoneyMagnification);
		//���擾�R�C�����𑝂₷
		m_totalGetCoin += int(coinValue * m_addMoneyMagnification);

		//�擾��
		m_playerSound->PlayAddCoinSound();
	};

	//�v���C���[���񕜂����郁�\�b�h
	void PlayerStatus::AddHitPoint(const int addValue)
	{
		//�񕜗͕��񕜂���
		m_hitPoint += addValue;
		//���g�̍ő�q�b�g�|�C���g���𒴂��Ă��܂�Ȃ����߂ɕ␳��������
		if (m_hitPoint > m_maxHitPoint)
		{
			m_hitPoint = m_maxHitPoint;
		}
	};
}