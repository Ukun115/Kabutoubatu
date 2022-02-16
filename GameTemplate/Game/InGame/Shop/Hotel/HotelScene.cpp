/// <summary>
/// �h���V�[���N���X
/// </summary>
#include "stdafx.h"
#include "HotelScene.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "../../Camera/PlayerCamera.h"
#include "HotelSelect.h"
#include "../../AccompanyAI/AccompanyAI.h"
#include "../../GameScene.h"
#include "../../GameLimitTime.h"
#include "../../../Graphics/Fade.h"


namespace nsKabutoubatu
{
	namespace nsHotelScene
	{
		Vector2   POS = { -160.0f,170.0f };			//�t�H���g�̈ʒu
		const Vector4   FONT_COLOR = { 0.0f,0.0f,0.0f,1.0f };	//�������ԃt�H���g�̐F
		const float     FONT_ROT = 0.0f;			//�t�H���g�̌X��
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//�t�H���g�̊�_
		const float     FONT_SCA = 0.75f;			//�t�H���g�̊g�嗦
	}

	bool HotelScene::Start()
	{
		//�v���C���[�̃C���X�^���X������
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}
		m_gameLimitTime = FindGO<GameLimitTime>(nsStdafx::GAMELIMITTIME);
		m_gameLimitTime->SetCanMeasure(false);

		m_fade[0] = NewGO<Fade>();
		m_fade[0]->Init(1.0f,0,0);

		//�J�����̃C���X�^���X������
		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		//�h���̓������f����������
		HotelInteriorModelInit();

		m_hukidasi = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_hukidasi->Init("Hukidasi", 450, 180);
		m_hukidasi->SetPosition({0.0f, 110.0f, 0.0f});

		//�X�����f����������
		SalespersonModelInit();

		//�X���̉�b��
		m_salespersonTalk = NewGO<FontRender>();
		m_salespersonTalk->Init
		(
			L"",							//�e�L�X�g
			nsHotelScene::POS,			//�ʒu
			nsHotelScene::FONT_COLOR,	//�F
			nsHotelScene::FONT_ROT,		//�X��
			nsHotelScene::FONT_SCA,		//�g�嗦
			nsHotelScene::FONT_PIV		//��_
		);

		//�v���C���[�Q�l������
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//����I�����Ă��邩�̃N���X
			m_hotelSelect[playerNum] = NewGO<HotelSelect>();
			//�v���C���[�̔ԍ����Z�b�g
			m_hotelSelect[playerNum]->SetPlayerNum(playerNum);

			m_coinSprite[playerNum] = NewGO<SpriteRender>();
			m_coinSprite[playerNum]->Init("Coin", 50, 50);
			m_coinSprite[playerNum]->SetPosition({ m_coinPos,115.0f,0.0f });
			m_coinPos += 835.0f;
		}

		return true;
	}

	HotelScene::~HotelScene()
	{
		//�h���������f�����폜
		DeleteGO(m_hotelInterior);
		//�X�����f�����폜
		DeleteGO(m_salesperson);
		//�X���̉�b�폜
		DeleteGO(m_salespersonTalk);
		//�v���C���[�Q�l���폜
		for (int playerNum = 0; playerNum < 2; playerNum++)
		{
			DeleteGO(m_hotelSelect[playerNum]);
			//�R�C���摜���폜
			DeleteGO(m_coinSprite[playerNum]);
		}

		//�V���b�vBGM���폜
		DeleteGO(m_shopBGM);

		DeleteGO(m_hukidasi);

		DeleteGO(m_fade[1]);
	}

	void HotelScene::Update()
	{
		//�����V���I�������A
		if (!m_salesperson->IsPlaying())
		{
			//�A�C�h����Ԃɂ���
			m_salesperson->PlayAnimation(enIdle);
		}

		if (m_fade[0] != nullptr && m_fade[0]->GetNowState() == 2)
		{
			DeleteGO(m_fade[0]);
		}

		if (m_gameScene == nullptr)
		{
			//�Q�[���V�[���̃C���X�^���X������
			m_gameScene = FindGO< GameScene>(nsStdafx::GAMESCENE_NAME);
			//�Q�[��BGM���~�߂�
			m_gameScene->GameBGMStop();

			//�V���b�vBGM�̏�����
			m_shopBGM = NewGO<SoundSource>();
			m_shopBGM->Init(L"Assets/sound/Shop_BGM.wav");
			m_shopBGM->SetVolume(0.2f);
			//�V���b�vBGM���Đ�
			m_shopBGM->Play(true);	//���[�v�Đ�
		}

		//�X���̉�b
		swprintf_s(m_talkText, L"�悤����!\n���܂��Ă����܂���?");
		m_salespersonTalk->SetText(m_talkText);

		//�v���C���[�Q�l������
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			CoinDeactivate(playerNum);
		}

		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			if (m_hotelSelect[playerNum]->GetNowSelectState() == 0)
			{
				return;
			}
		}

		//�h������o��
		EnterFromHotel();
		//���܂�l�͑S�񕜂�����
		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			if (m_hotelSelect[playerNum]->GetNowSelectState() == 1)
			{
				//���^���񕜂���
				m_playerStatus[playerNum]->AddHitPoint(10);
			}
		}
	}

	//�h���̓������f���̏��������\�b�h
	void HotelScene::HotelInteriorModelInit()
	{
		m_hotelInterior = NewGO<SkinModelRender>();
		m_hotelInterior->SetShadowCaster(false);	//�V���h�E�͗��Ƃ��Ȃ�
		m_hotelInterior->SetOutline(false);		//�֊s�������Ȃ�
		m_hotelInterior->Init("ItemShopInterior");
		//�h���̈ʒu���w��
		m_pos = { 0.0f, 10000.0f, 0.0f };
		m_hotelInterior->SetPosition(m_pos);	//�h�����X�e�[�W�̏��ɂ����ăX�e�[�W���؂�ւ���Ă���悤�Ɍ�����
		//�����蔻���K��
		m_physicsStaticObject.CreateFromModel(*m_hotelInterior->GetModel(), m_hotelInterior->GetModel()->GetWorldMatrix());
		//�����蔻����X�V
		m_physicsStaticObject.Update(m_pos, m_rot);
	}

	//�X�����f���̏��������\�b�h
	void HotelScene::SalespersonModelInit()
	{
		m_salesperson = NewGO<SkinModelRender>();
		//���f���̏������Z�A�b�v�Ɏw��
		m_salesperson->SetModelUpAxis(enModelUpAxisZ);
		m_salesperson->SetOutline(true);		//�֊s��������
		//�A�j���[�V���������[�h
		m_animationClips[enIdle].Load("SalesPerson_Idle");
		m_animationClips[enGreething].Load("SalesPerson_Greething");		//�����V
		//���[�v�A�j���[�V�������ǂ�����ݒ�
		m_animationClips[enIdle].SetLoopFlag(true);
		m_animationClips[enGreething].SetLoopFlag(false);
		m_salesperson->Init("SalesPerson", "SalesPerson", m_animationClips, enAnimationNum);
		m_salesperson->SetAnimationSpeed(1.5f);
		//�X���̃A�j���[�V�����̏�ԍX�V
		m_salesperson->PlayAnimation(enGreething);
		//�ʒu���Z�b�g
		m_salesperson->SetPosition({ m_pos.x,m_pos.y,m_pos.z - 100.0f });
	}

	//�h������o�郁�\�b�h
	void HotelScene::EnterFromHotel()
	{
		//�����V�A�j���[�V����
		m_salesperson->PlayAnimation(enGreething);

		if (m_fade[1] == nullptr)
		{
			m_fade[1] = NewGO<Fade>();
			m_fade[1]->Init(0.0f,1,0);
		}
		if (m_fade[1]->GetNowState() == 2)
		{
			//�v���C���[�̏�Ԃ�ʏ��Ԃɂ���
			m_player[enPlayer1]->SetNowState(0);
			//�v���C���[�̈ʒu���h���̑O�Ɉړ�������
			m_player[enPlayer1]->Warp({ m_hotelPos.x - 50.0f,m_hotelPos.y,m_hotelPos.z + 500.0f });
			if (m_playerNum == 2)
			{
				m_player[enPlayer2]->SetNowState(0);
				m_player[enPlayer2]->Warp({ m_hotelPos.x + 50.0f,m_hotelPos.y,m_hotelPos.z + 500.0f });
			}

			if (m_playerNum == 1)
			{
				m_accompanyAI = FindGO<AccompanyAI>(nsStdafx::ACCOMPANYAI_NAME);
				m_accompanyAI->SetPosition({ m_hotelPos.x + 50.0f,m_hotelPos.y,m_hotelPos.z + 500.0f });
				m_accompanyAI->SetNowState(1);
			}

			//�J�����̈ړ����@���Z�b�g
			m_playerCamera->SetCameraState(0);

			//�Q�[��BGM���ēx�Đ�
			m_gameScene->GameBGMPlay();

			//�h���V�[�����폜
			DeleteGO(this);

			//�������Ԃ��o�߂���悤�ɖ߂�
			m_gameLimitTime->SetCanMeasure(true);
		}
	}

	void HotelScene::CoinDeactivate(const int playerNum)
	{
		if (m_hotelSelect[playerNum]->GetNowSelectState() != 0)
		{
			m_coinSprite[playerNum]->Deactivate();
		}
	}
}