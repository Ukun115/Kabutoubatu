/// <summary>
/// �R�C���N���X
/// </summary>
#include "stdafx.h"
#include "DropCoin.h"
#include "../Player/Player.h"
#include "../Player/PlayerStatus.h"
#include "../Camera/PlayerCamera.h"
#include "../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	bool DropCoin::Start()
	{
		m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed> (nsStdafx::ONLINEUPDATESPEED_NAME);

		//�v���C���[�̃C���X�^���X������
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}

		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		//�R�C�����f����������
		CoinModelInit();

		return true;
	}

	DropCoin::~DropCoin()
	{
		//���f����j��
		DeleteGO(m_coin);
	}

	void DropCoin::Update()
	{
		//�R�C���̃o�E���h����
		Bound();
		//�R�C���̉�]����
		Turn();
		//�R�C���擾����
		CoinGet();
	}

	//�R�C�����f���̏��������\�b�h
	void DropCoin::CoinModelInit()
	{
		//���f����������
		m_coin = NewGO<SkinModelRender>();
		m_coin->Init("Coin");
		//���߂̃R�C���̃o�E���h�p���[
		m_boundPower = m_boundPowerDown * 100.0f;
		//�������Ȃ���Ԃ��ƃ��f�����n�ʂɖ����ꂽ�̂ŁAY���W�𒲐�
		m_pos.y += 50.0f;
		//�X�^�[�g�ʒu��Y���W��ۑ�
		m_boundStartPosY = m_pos.y;
		//�����ʒu���X�V
		m_coin->SetPosition(m_pos);
	}

	//�R�C�����o�E���h���鏈�����\�b�h
	void DropCoin::Bound()
	{
		m_pos.y += m_boundPower * g_gameTime->GetFrameDeltaTime() * m_onlineUpdateSpeed->GetSpeed();
		m_boundPower -= 980.0f * g_gameTime->GetFrameDeltaTime() * m_onlineUpdateSpeed->GetSpeed();

		//�n�ʂɗ��������A
		if (m_boundStartPosY > m_pos.y)
		{
			m_getOkFlg = true;

			//�o�E���h�p���[��0�ɂȂ�����A
			if (m_boundPowerDown == 0)
			{
				//�N���X��j��
				DeleteGO(this);
			}

			//�o�E���h���̏�����
			if ((m_playerCamera->GetCameraTarget() - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
			{
				m_coinSound = NewGO<SoundSource>();
				m_coinSound->Init(L"Assets/sound/Coin_Bound.wav");
				m_coinSound->SetVolume(0.2f);
				//�o�E���h�����Đ�
				m_coinSound->Play(false);
			}

			//�o�E���h�p���[���_�E��������
			m_boundPowerDown--;
			//���̍����̃o�E���h�p���[�ɂ���
			m_boundPower = m_boundPowerDown * 100.0f;
		}
		//�ʒu���X�V
		m_coin->SetPosition(m_pos);
	}

	//�R�C���̉�]�������\�b�h
	void DropCoin::Turn()
	{
		//��]���x��x�����Ă���
		m_firstRotSpeed -= 0.2f;
		//m_firstRotSpeed�����̐��ɂȂ��ăR�C�����t��]���Ȃ��悤�ɉ�]���x�̕␳
		m_firstRotSpeed = max(0.0f, m_firstRotSpeed);
		//��]�p�x���X�V
		m_rotAngle += m_firstRotSpeed;
		//��]�p�x���N�H�[�^�j�I���ɃZ�b�g
		m_rot.SetRotationDeg(Vector3::AxisY, m_rotAngle);
		//���f������]������
		m_coin->SetRotation(m_rot);
	}

	//�R�C���擾�������\�b�h
	void DropCoin::CoinGet()
	{
		if (m_getOkFlg)
		{

			for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
			{
				//�v���C���[�ƃR�C���Ƃ̋������v�Z
				m_playerToCoinDistancelength = (m_player[playerNum]->GetPosition() - m_pos).Length();
				//�v���C���[�ƃR�C���̋����𑪂�A�R�C���Ƀv���C���[���G��Ă�����A
				if (m_playerToCoinDistancelength < 250.0f)
				{
					//�v���C���[�̏������𑝂₷
					m_playerStatus[playerNum]->AddCoin(m_coinValue);
					//�N���X��j��
					DeleteGO(this);
				}
			}
		}
	}
};