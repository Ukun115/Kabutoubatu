/// <summary>
/// �X�e�[�W��̌����N���X
/// </summary>
#include "stdafx.h"
#include "StageBuilding.h"
#include "../Player/Player.h"
#include "../Player/PlayerRotation.h"
#include "../Player/PlayerAnimation.h"
#include "../Shop/Hotel/HotelScene.h"
#include "../Shop/ItemShop/ItemShopScene.h"
#include "FirstBossStage.h"
#include "../AccompanyAI/AccompanyAI.h"
#include "../../Graphics/Fade.h"
#include "../Camera/PlayerCamera.h"
#include "../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	bool StageBuilding::Start()
	{
		m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//�v���C���[�̃C���X�^���X������
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerRotation[playerNum] = FindGO<PlayerRotation>(nsStdafx::PLAYER_ROTATION_NAME[playerNum]);
			m_playerAnimation[playerNum] = FindGO<PlayerAnimation>(nsStdafx::PLAYER_ANIMATION_NAME[playerNum]);

			m_hukidasiSprite[playerNum] = NewGO<SpriteRender>();
			m_hukidasiSprite[playerNum]->Init("Hukidasi_PushA",100.0f,75.0f);
		}

		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		//�������f����������
		BuildingModelInit();
		//�h�A���f����������
		DoorModelInit();
		//�Èœ������f����������
		DarknessEntranceInit();

		switch (m_shopType)
		{
		case enHotel:
		case enItemShop:
			m_buildingEnterLength = 250.0f;
			m_canDoorTatchLength = 250.0f;
			break;

		case enFirstBossHouse:
			m_buildingEnterLength = 200.0f;
			m_canDoorTatchLength = 450.0f;
			break;
		}

		return true;
	}

	StageBuilding::~StageBuilding()
	{
		//�������폜
		DeleteGO(m_building);
		//�h�A���f�����폜
		DeleteGO(m_door);
		//�Èœ������f�����폜
		DeleteGO(m_darknessEntrance);

		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			DeleteGO(m_hukidasiSprite[playerNum]);
		}

		if (m_hotelScene != nullptr)
		{
			DeleteGO(m_hotelScene);
		}
		if (m_itemShopScene != nullptr)
		{
			DeleteGO(m_itemShopScene);
		}
		if (m_firstBossStage != nullptr)
		{
			DeleteGO(m_firstBossStage);
		}
	}

	void StageBuilding::Update()
	{
		//�����ɓ����Ă��Ȃ��Ƃ��A
		if (m_sceneChangeDelayTimer == 0)
		{
			//�����ɓ��������ǂ����̔��菈��
			JudgmentEntered();
		}
		//�����ɓ������Ƃ��A
		else
		{
			if (m_sceneChangeDelayTimer == 10/ m_onlineUpdateSpeed->GetSpeed())
			{
				//�����ɓ��鉹�̏�����
				m_buildingSound = NewGO<SoundSource>();
				m_buildingSound->Init(L"Assets/sound/Building_Entrance.wav");
				m_buildingSound->SetVolume(0.2f);
				//�����ɓ��鉹�����Đ�
				m_buildingSound->Play(false);
			}
			//�x���^�C�}�[�����Z
			m_sceneChangeDelayTimer++;
			//40�t���[���o������A
			if (m_sceneChangeDelayTimer == 40/ m_onlineUpdateSpeed->GetSpeed() && m_fade->GetNowState() == 2)
			{
				//�V�[���J��
				SceneTransition();

				DeleteGO(m_fade);
				//�k���|�C���^�����Ă����B
				m_fade = nullptr;
			}
		}
	}

	//�������f���̏��������\�b�h
	void StageBuilding::BuildingModelInit()
	{
		//�������f���̏�����
		m_building = NewGO<SkinModelRender>();
		m_building->SetShadowCaster(true);	//�V���h�E���Ƃ�
		m_building->SetOutline(true);		//�֊s��������
		switch (m_shopType)
		{
		//�h��
		case enHotel:
			//�F�̉�
			m_building->Init("House");
			break;
		//�A�C�e���V���b�v
		case enItemShop:
			//�ԐF�̉�
			m_building->Init("HouseRed");
			break;
		//�ŏ��̃{�X�̉�
		case enFirstBossHouse:
			//�{�X�̉�
			m_building->Init("FirstBossHouse");
			break;
		}
		m_building->SetPosition(m_pos);	//�ʒu���Z�b�g
		//�����蔻���K��
		m_physicsStaticObject[0].CreateFromModel(*m_building->GetModel(), m_building->GetModel()->GetWorldMatrix());
		//�ʒu���ړ��������̂ňʒu���X�V
		m_physicsStaticObject[0].Update(m_pos, m_rot);
	}

	//�h�A���f���̏��������\�b�h
	void StageBuilding::DoorModelInit()
	{
		m_door = NewGO<SkinModelRender>();
		m_door->SetShadowCaster(true);	//�V���h�E���Ƃ�
		m_door->SetOutline(true);		//�֊s��������
		switch (m_shopType)
		{
			//�h���̃h�A
		case enHotel:
			//�F�̉�
			m_door->Init("House_Door");
			m_doorPos = m_pos;
			m_doorPos.z += 340.0f;
			m_doorPos.x += 60.0f;
			break;
			//�A�C�e���V���b�v�̃h�A
		case enItemShop:
			//�ԐF�̉�
			m_door->Init("HouseRed_Door");
			m_doorPos = m_pos;
			m_doorPos.z += 340.0f;
			m_doorPos.x += 60.0f;
			break;
			//�ŏ��̃{�X�̉Ƃ̃h�A
		case enFirstBossHouse:
			m_door->Init("FirstBossHouse_Door");
			m_doorPos = m_pos;
			m_doorPos.z += 220.0f;
			break;
		}

		m_door->SetPosition(m_doorPos);		//�ʒu���Z�b�g
		//�����蔻���K��
		m_physicsStaticObject[1].CreateFromModel(*m_door->GetModel(), m_door->GetModel()->GetWorldMatrix());
		//�ʒu���ړ��������̂ňʒu���X�V
		m_physicsStaticObject[1].Update(m_doorPos, m_rot);
	}

	//�����ɓ��������ǂ����̔��胁�\�b�h
	void StageBuilding::JudgmentEntered()
	{
		//�v���C���[�̐l�������s
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//�v���C���[����h�A�ɂ̂т�x�N�g�����擾
			m_newPlayerMoveDirection = m_doorPos - m_player[playerNum]->GetPosition();
			//�h�A�Ƃ̋������߂�������A
			if (m_newPlayerMoveDirection.Length() < m_canDoorTatchLength&&m_player[playerNum]->GetNowState() != enGhost)
			{
				m_doorTatchFlg[playerNum] = true;
				//�t���O�𗧂Ă�A�{�^���������ꂽ�Ƃ��Ƀh�A�J�Ƀv���X���čU�����Ȃ��悤�ɂ��Ă���B
				m_player[playerNum]->SetNowDoorTatch(true);

				//�����o����\��������
				HukidasiActivate(playerNum);

				if (m_gamePad[playerNum] != nullptr)
				{
					//A�{�^���������ꂽ��A
					if (m_gamePad[playerNum]->IsTrigger(enButtonA))
					{
						//�h�A�̏�Ԃ̓���ւ�
						switch (m_doorState)
						{
						case enClose:
							m_doorState = enOpen;

							//�h�A���J���鉹
							m_doorOpenSound = NewGO<SoundSource>();
							m_doorOpenSound->Init(L"Assets/sound/Door_Open.wav");
							m_doorOpenSound->SetVolume(0.2f);
							m_doorOpenSound->Play(false);

							break;
						case enOpen:
							m_doorState = enClose;

							//�h�A����鉹
							m_doorCloseSound = NewGO<SoundSource>();
							m_doorCloseSound->Init(L"Assets/sound/Door_Close.wav");
							m_doorCloseSound->SetVolume(0.2f);
							m_doorCloseSound->Play(false);

							break;
						}
						m_doorSlideTimer = 0;
					}
				}
				else
				{
					//A�{�^���������ꂽ��A
					if (g_pad[playerNum]->IsTrigger(enButtonA))
					{
						//�h�A�̏�Ԃ̓���ւ�
						switch (m_doorState)
						{
						case enClose:
							m_doorState = enOpen;

							//�h�A���J���鉹
							m_doorOpenSound = NewGO<SoundSource>();
							m_doorOpenSound->Init(L"Assets/sound/Door_Open.wav");
							m_doorOpenSound->SetVolume(0.2f);
							m_doorOpenSound->Play(false);

							break;
						case enOpen:
							m_doorState = enClose;

							//�h�A����鉹
							m_doorCloseSound = NewGO<SoundSource>();
							m_doorCloseSound->Init(L"Assets/sound/Door_Close.wav");
							m_doorCloseSound->SetVolume(0.2f);
							m_doorCloseSound->Play(false);

							break;
						}
						m_doorSlideTimer = 0;
					}
				}
			}
			else
			{
				//PushA!!�����o�����\��
				m_hukidasiSprite[playerNum]->Deactivate();
				m_doorTatchFlg[playerNum] = false;

			}

			//�h�A���J��
			switch (m_doorState)
			{
			case enClose:
				switch (m_shopType)
				{
				case enHotel:
				case enItemShop:
					m_rotationAngle += 0.05f* m_onlineUpdateSpeed->GetSpeed();
					m_rotationAngle = min(m_rotationAngle, 2.0f);
					//��]���X�V
					m_rot.SetRotation(Vector3::AxisY, m_rotationAngle);
					m_door->SetRotation(m_rot);

					break;

				case enFirstBossHouse:
					if (m_doorSlideTimer < 60/ m_onlineUpdateSpeed->GetSpeed())
					{
						m_doorPos.x -= 3.0f* m_onlineUpdateSpeed->GetSpeed();
						m_door->SetPosition(m_doorPos);
					}
					m_doorSlideTimer++;

					break;
				}

				break;

			case enOpen:
				switch (m_shopType)
				{
				case enHotel:
				case enItemShop:
					m_rotationAngle -= 0.05f* m_onlineUpdateSpeed->GetSpeed();
					m_rotationAngle = max(m_rotationAngle, 0.0f);
					//��]���X�V
					m_rot.SetRotation(Vector3::AxisY, m_rotationAngle);
					m_door->SetRotation(m_rot);

					break;

				case enFirstBossHouse:
					if (m_doorSlideTimer < 60/ m_onlineUpdateSpeed->GetSpeed() && m_doorPos.x != m_pos.x)
					{
						m_doorPos.x += 3.0f* m_onlineUpdateSpeed->GetSpeed();
						m_door->SetPosition(m_doorPos);
					}
					m_doorSlideTimer++;

					break;
				}

				break;
			}
			//�h�A�̈ʒu�A��]�̓����蔻����X�V
			m_physicsStaticObject[1].Update(m_doorPos, m_rot);


			//�v���C���[����h���ɂ̂т�x�N�g�����擾
			m_newPlayerMoveDirection = m_pos - m_player[playerNum]->GetPosition();
			//�����Ƃ̋������߂�������A�i�����ɓ�������A�j
			//���A�v���C���[���W�����v��Ԃł͂Ȃ��Ƃ��A
			if (m_newPlayerMoveDirection.Length() < m_buildingEnterLength && m_playerAnimation[playerNum]->GetAnimationState() != 3)
			{
				//�����ɓ����Ԃɂ���
				m_player[playerNum]->SetNowState(enDontMove);
				//�v���C���[�̈ړ������������̉������ɂ��Ĉړ�������
				m_player[playerNum]->SetMoveSpeed({ 0.0f,0.0f,-1.0f });

				//�x���^�C�}�[�����Z
				m_sceneChangeDelayTimer++;

				m_fade = NewGO<Fade>();
				m_fade->Init(0.0f,1,0);
			}
		}
	}

	//�V�[���J�ڃ��\�b�h
	void StageBuilding::SceneTransition()
	{
		//�V�[���J��
		switch (m_shopType)
		{
		//�h��
		case enHotel:
			m_hotelScene = NewGO<HotelScene>();
			m_hotelScene->SetPlayerNumber(m_playerNum);
			//�����̏ꏊ���V�[���ɓn���Ă���
			m_hotelScene->SetHotelPosition(m_pos);
			for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
			{
				if (m_gamePad[playerNum] != nullptr)
				{
					m_hotelScene->SetPlayerGamePad(*m_gamePad[playerNum], playerNum);
				}
			}
			break;
		//�A�C�e���V���b�v
		case enItemShop:
			m_itemShopScene = NewGO<ItemShopScene>();
			m_itemShopScene->SetPlayerNumber(m_playerNum);
			//�����̏ꏊ���V�[���ɓn���Ă���
			m_itemShopScene->SetItemShopPosition(m_pos);
			break;
		//�ŏ��̃{�X�Ɛ키�V�[��
		case enFirstBossHouse:
			m_firstBossStage = NewGO<FirstBossStage>();
			m_firstBossStage->SetPlayerNumber(m_playerNum);
		}
		//�v���C���[�̐l�������s
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//�{�X��̂Ƃ��A
			if (m_shopType == enFirstBossHouse)
			{
				m_gyaku = -1;
			}

			//1P,2P�̈ʒu���V�[���̈ʒu�Ɉړ�������
			switch (playerNum)
			{
			case enPlayer1:
				m_player[playerNum]->Warp({ 130.0f, 10000.0f* m_gyaku, 250.0f });
				break;
			case enPlayer2:
				m_player[playerNum]->Warp({ -130.0f, 10000.0f* m_gyaku, 250.0f });
				break;
			}

			//�ړ����x���O�ɂ���
			m_player[playerNum]->SetMoveSpeed(Vector3::Zero);
			//���Ɍ�������
			m_playerRotation[playerNum]->SetRotationAngle(-3.0f);
			//�{�X��ȊO�̂Ƃ��A
			if (m_shopType != enFirstBossHouse)
			{
				//����s��Ԃɂ���
				m_player[playerNum]->SetNowState(enDontMove);
			}
			//�{�X��ɍs�����S�[�X�g��Ԃ���Ȃ��Ƃ��A
			else if (m_player[playerNum]->GetNowState() != enGhost)
			{
				//�ʏ��Ԃɂ���
				m_player[playerNum]->SetNowState(0);
			}
			//�A�C�h���A�j���[�V�����ɂ���
			m_playerAnimation[playerNum]->SetAnimation(enNormal);
			//�񕜃A�C�e���������Ă����炵�܂��B
			m_player[playerNum]->RecoveryItemDelete();
		}

		//�\�����[�h�̎��A
		if (m_playerNum == 1)
		{
			m_accompaniAI = FindGO<AccompanyAI>(nsStdafx::ACCOMPANYAI_NAME);
			m_accompaniAI->SetNowState(enNormal);
			m_accompaniAI->SetPosition({ -130.0f, 10000.0f * m_gyaku, 250.0f });
		}

		//�V�[���ɓ���܂ł̒x���^�C�}�[�̏�����
		m_sceneChangeDelayTimer = 0;

		//�h�A���J���Ă����Ȃ�߂Ă���
		if (m_doorState == enOpen)
		{
			m_doorState = enClose;
		}
		else
		{
			m_doorState = enOpen;
		}
	}

	//�Èœ������f���̏��������\�b�h
	void StageBuilding::DarknessEntranceInit()
	{
		m_darknessEntrance = NewGO<SkinModelRender>();
		m_darknessEntrance->SetShadowCaster(false);	//�V���h�E���Ƃ��Ȃ�
		m_darknessEntrance->SetOutline(false);		//�֊s�������Ȃ�
		switch (m_shopType)
		{
		case enHotel:
		case enItemShop:
			m_darknessEntrance->Init("DarknessEntrance");
			break;
		case enFirstBossHouse:
			m_darknessEntrance->Init("DarknessEntrance_FirstBossHouse");
			break;
		}
			m_darknessEntrance->SetPosition(m_pos);		//�ʒu���Z�b�g
	}

	//�����o����\�������郁�\�b�h
	void StageBuilding::HukidasiActivate(const int playerNum)
	{
		//�v���C���[�̃X�N���[�����W���擾
		g_camera3D->CalcScreenPositionFromWorldPosition(m_playerScreenPos, m_player[playerNum]->GetPosition());
		//�J�����̒����_�ɂȂ��Ă���v���C���[�ɋ߂��Ƃ��̂ݐ����o����\��������
		if (((m_playerCamera->GetCameraTarget() - m_player[playerNum]->GetPosition()).Length() > 3000.0f))
		{
			//PushA!!�����o�����\��
			m_hukidasiSprite[playerNum]->Deactivate();

			return;
		}
		//�ʒu�𒲐�
		m_playerScreenPos.x -= 50.0f;
		m_playerScreenPos.y += 75.0f;
		//�����o���̈ʒu���Z�b�g
		m_hukidasiSprite[playerNum]->SetPosition({ m_playerScreenPos.x,m_playerScreenPos.y,0.0f });
		//PushA!!�����o����\��
		m_hukidasiSprite[playerNum]->Activate();
	}
}