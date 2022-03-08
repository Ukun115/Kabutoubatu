/// <summary>
/// �A�C�e���V���b�v�V�[���N���X
/// </summary>
#include "stdafx.h"
#include "ItemShopScene.h"
#include "ItemShopSelect.h"
#include "../../Player/Player.h"
#include "../../Camera/PlayerCamera.h"
#include "../../AccompanyAI/AccompanyAI.h"
#include "../../GameScene.h"
#include "../../GameLimitTime.h"
#include "../../../Graphics/Fade.h"


namespace nsKabutoubatu
{
	namespace nsItemShopScene
	{
		Vector2   POS = { -160.0f,170.0f };			//�������ԃt�H���g�̈ʒu
		const Vector4   FONT_COLOR = { 0.0f,0.0f,0.0f,1.0f };	//�������ԃt�H���g�̐F
		const float     FONT_ROT = 0.0f;			//�t�H���g�̌X��
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//�t�H���g�̊�_
		const float     FONT_SCA = 0.75f;			//�t�H���g�̊g�嗦

		const char ITEM_FILEPATH[3][256] = { "Bow_Item","RecoveryItem","Shoes" };		//�t�@�C���p�X
	}

	bool ItemShopScene::Start()
	{
		//�v���C���[�̃C���X�^���X������
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
		}
		//�J�����̃C���X�^���X������
		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		m_fade[0] = NewGO<Fade>();
		m_fade[0]->Init(1.0f,0,0);

		m_gameLimitTime = FindGO<GameLimitTime>(nsStdafx::GAMELIMITTIME);
		m_gameLimitTime->SetCanMeasure(false);

		//�A�C�e���V���b�v�̓������f����������
		ItemShopInteriorModelInit();

		//�A�C�e���V���b�v�̏��i���f��
		for (int shopItemNum = 0; shopItemNum < 3; shopItemNum++)
		{
			m_shopItem[shopItemNum] = NewGO<SkinModelRender>();
			m_shopItem[shopItemNum]->SetShadowCaster(false);	//�V���h�E�͗��Ƃ��Ȃ�
			m_shopItem[shopItemNum]->SetOutline(true);		//�֊s���͂���
			m_shopItem[shopItemNum]->Init(nsItemShopScene::ITEM_FILEPATH[shopItemNum]);
			//�ʒu��ݒ�
			m_itemPos = m_pos;
			m_itemPos.y += 40.0f;
			m_itemPos.x -= 150.0f;
			m_itemPos.x += shopItemNum * 150.0f;
			m_shopItem[shopItemNum]->SetPosition(m_itemPos);
		}

		m_coinSpritePos = -295.0f;
		for (int i = 0; i < 3; i++)
		{
			//�R�C���摜��������
			m_coinSprite[i] = NewGO<SpriteRender>();
			m_coinSprite[i]->Init("Coin", 25, 25);
			m_coinSprite[i]->SetPosition({ m_coinSpritePos,-177.0f,0.0f });
			m_coinSpritePos += 270.0f;
		}

		m_hukidasi = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_hukidasi->Init("Hukidasi", 450, 180);
		m_hukidasi->SetPosition({ 0.0f, 110.0f, 0.0f });

		//�X�����f����������
		SalespersonModelInit();

		//�X���̉�b��
		m_salespersonTalk = NewGO<FontRender>();
		m_salespersonTalk->Init
		(
			L"",							//�e�L�X�g
			nsItemShopScene::POS,			//�ʒu
			nsItemShopScene::FONT_COLOR,	//�F
			nsItemShopScene::FONT_ROT,		//�X��
			nsItemShopScene::FONT_SCA,		//�g�嗦
			nsItemShopScene::FONT_PIV		//��_
		);

		//�v���C���[�Q�l������
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//����I�����Ă��邩�̃N���X
			m_ItemShopSelect[playerNum] = NewGO<ItemShopSelect>();
			//�v���C���[�̔ԍ����Z�b�g
			m_ItemShopSelect[playerNum]->SetPlayerNum(playerNum);

			m_ItemShopSelect[playerNum]->SetPlayerGamePad(*m_playerGamePad[playerNum]);
		}

		return true;
	}

	ItemShopScene::~ItemShopScene()
	{
		//�R�C���摜���폜
		for (int i = 0; i < 3; i++)
		{
			DeleteGO(m_coinSprite[i]);
		}
		DeleteGO(m_hukidasi);
		//�A�C�e���V���b�v�������f�����폜
		DeleteGO(m_itemShopInterior);
		//�X�����f�����폜
		DeleteGO(m_salesperson);
		//�X���̉�b�폜
		DeleteGO(m_salespersonTalk);
		//�v���C���[�Q�l���폜
		for (int playerNum = enPlayer1; playerNum < enPlayerNum; playerNum++)
		{
			DeleteGO(m_ItemShopSelect[playerNum]);
		}

		//�V���b�vBGM���폜
		DeleteGO(m_shopBGM);

		DeleteGO(m_fade[1]);
	}

	void ItemShopScene::Update()
	{
		//�����V���I�������A
		if (!m_salesperson->IsPlaying())
		{
			//�A�C�h����Ԃɂ���
			m_salesperson->PlayAnimation(enIdle,1.0f);
		}

		if (m_fade[0] != nullptr && m_fade[0]->GetNowState() == 2)
		{
			DeleteGO(m_fade[0]);
			//�k���|�C���^�����Ă����B
			m_fade[0] = nullptr;
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
		swprintf_s(m_talkText, L"����ɂ���!\n�����~�����ł���?");
		m_salespersonTalk->SetText(m_talkText);

		//�v���C���[�Q�l������
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//��l�Ƃ����X���o���ԂɂȂ��Ă�����A
			if (m_ItemShopSelect[enPlayer1]->GetNowShoppingState() == 3)
			{
				//�\���v���C�̂Ƃ��A
				if (m_ItemShopSelect[enPlayer2] == nullptr)
				{
					//�A�C�e���V���b�v����o��
					EnterFromShop();
				}
				//�I�����C��or�}���`�v���C�̎��A
				else if (m_ItemShopSelect[enPlayer2]->GetNowShoppingState() == 3)
				{
					//�A�C�e���V���b�v����o��
					EnterFromShop();
				}
			}
		}
	}

	//�A�C�e���V���b�v�̓������f���̏��������\�b�h
	void ItemShopScene::ItemShopInteriorModelInit()
	{
		m_itemShopInterior = NewGO<SkinModelRender>();
		m_itemShopInterior->SetShadowCaster(false);	//�V���h�E�͗��Ƃ��Ȃ�
		m_itemShopInterior->SetOutline(false);		//�֊s�������Ȃ�
		m_itemShopInterior->Init("ItemShopInterior");
		//�A�C�e���V���b�v�̈ʒu���w��
		m_pos = { 0.0f, 10000.0f, 0.0f };
		m_itemShopInterior->SetPosition(m_pos);	//�A�C�e���V���b�v���X�e�[�W�̏��ɂ����ăX�e�[�W���؂�ւ���Ă���悤�Ɍ�����
		//�����蔻���K��
		m_physicsStaticObject.CreateFromModel(*m_itemShopInterior->GetModel(), m_itemShopInterior->GetModel()->GetWorldMatrix());
		//�����蔻����X�V
		m_physicsStaticObject.Update(m_pos, m_rot);
	}

	//�X�����f���̏��������\�b�h
	void ItemShopScene::SalespersonModelInit()
	{
		m_salesperson = NewGO<SkinModelRender>();
		//���f���̏������Z�A�b�v�Ɏw��
		m_salesperson->SetModelUpAxis(enModelUpAxisZ);
		m_salesperson->SetOutline(true);		//�֊s��������
		//�A�j���[�V���������[�h
		m_animationClips[enIdle].Load("SalesPerson_Idle");			//�A�C�h��
		m_animationClips[enGreething].Load("SalesPerson_Greething");		//�����V
		//���[�v�A�j���[�V�������ǂ�����ݒ�
		m_animationClips[enIdle].SetLoopFlag(true);
		m_animationClips[enGreething].SetLoopFlag(false);
		m_salesperson->Init("SalesPerson","SalesPerson", m_animationClips, enAnimationNum);
		m_salesperson->SetAnimationSpeed(1.5f);
		//�A�j���[�V�����̏�ԍX�V
		m_salesperson->PlayAnimation(enGreething,1.0f);
		//�ʒu���Z�b�g
		m_salesperson->SetPosition({ m_pos.x,m_pos.y,m_pos.z - 100.0f });
	}

	//�A�C�e���V���b�v����o�郁�\�b�h
	void ItemShopScene::EnterFromShop()
	{
		//�����V�A�j���[�V����
		m_salesperson->PlayAnimation(enGreething,1.0f);

		if (m_fade[1] == nullptr)
		{
			m_fade[1] = NewGO<Fade>();
			m_fade[1]->Init(0.0f,1,0);
		}
		if (m_fade[1]->GetNowState() == 2)
		{
			//�v���C���[�̏�Ԃ�ʏ��Ԃɂ���
			m_player[enPlayer1]->SetNowState(0);
			//�v���C���[�̈ʒu���V���b�v�̑O�Ɉړ�������
			m_player[enPlayer1]->Warp({ m_shopPos.x - 50.0f,m_shopPos.y,m_shopPos.z + 500.0f });
			if (m_playerNum == 1)
			{
				m_accompanyAI = FindGO<AccompanyAI>(nsStdafx::ACCOMPANYAI_NAME);
				m_accompanyAI->SetNowState(1);
				m_accompanyAI->SetPosition({ m_shopPos.x + 50.0f,m_shopPos.y,m_shopPos.z + 500.0f });
			}
			if (m_playerNum == 2)
			{
				m_player[enPlayer2]->SetNowState(0);
				m_player[enPlayer2]->Warp({ m_shopPos.x + 50.0f,m_shopPos.y,m_shopPos.z + 500.0f });
			}
			//�J�����̈ړ����@���Z�b�g
			m_playerCamera->SetCameraState(0);

			//�Q�[����BGM���ēx�Đ�
			m_gameScene->GameBGMPlay();

			//�A�C�e���V���b�v�V�[�����폜
			DeleteGO(this);

			//�������Ԃ��o�߂���悤�ɖ߂�
			m_gameLimitTime->SetCanMeasure(true);
		}
	}
}