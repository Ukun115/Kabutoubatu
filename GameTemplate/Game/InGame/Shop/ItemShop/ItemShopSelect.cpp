/// <summary>
/// �A�C�e���V���b�v�V�[���̑I���N���X
/// </summary>
#include "stdafx.h"
#include "ItemShopSelect.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "ItemShopSelectFont.h"
#include "../../Item/Weapon/Weapon.h"
#include "../../GameScreenUI.h"

namespace nsKabutoubatu
{
	bool ItemShopSelect::Start()
	{
		//�v���C���[�̃C���X�^���X������
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_playerNum]);
		m_playerStatus = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[m_playerNum]);
		m_weapon = FindGO<Weapon>(nsStdafx::WEAPON_NAME[m_playerNum]);
		m_gameScreenUi = FindGO<GameScreenUI>(nsStdafx::GAMESCREENUI_NAME);

		//�I���𕶎��\��������N���X�𐶐�
		m_itemShopSelectFont = NewGO<ItemShopSelectFont>();
		//�v���C���[�ԍ����Z�b�g
		m_itemShopSelectFont->SetPlayerNum(m_playerNum);
		if (m_playerGamePad != nullptr)
		{
			m_itemShopSelectFont->SetPlayerGamePad(*m_playerGamePad);
		}

		//PushX�A�C�e�������{�^���摜��������
		m_pushXSprite = NewGO<SpriteRender>();
		m_pushXSprite->Init("PushX",300,210);

		//�A�C�e�������w�i�摜
		m_ItemSetumeiSprite = NewGO<SpriteRender>();
		m_ItemSetumeiSprite->Init("ItemSetumeiBack", 400, 500);
		m_ItemSetumeiSprite->Deactivate();	//��\��

		switch (m_playerNum)
		{
		case 0:
			m_pushXSprite->SetPosition({-500.0f,-190.0f,0.0f});
			m_ItemSetumeiSprite->SetPosition({-450.0f,0.0f,0.0f});
			break;
		case 1:
			m_pushXSprite->SetPosition({500.0f,-190.0f,0.0f});
			m_ItemSetumeiSprite->SetPosition({450.0f,0.0f,0.0f});
			break;
		}

		return true;
	}

	ItemShopSelect::~ItemShopSelect()
	{
		//�A�C�e���V���b�v�I�𕶎��N���X���폜
		DeleteGO(m_itemShopSelectFont);
		//PushX�A�C�e�������{�^���摜���폜
		DeleteGO(m_pushXSprite);
		//�A�C�e�������w�i�摜
		DeleteGO(m_ItemSetumeiSprite);
	}

	void ItemShopSelect::Update()
	{
		switch (m_nowShoppingState)
		{
			//�����A����A�o��A�̑I�����
		case enShoppingModeSelectState:

			//�I���J�[�\���ړ�
			CursorMove(enBuy, enEnter);

			//A�{�^���������ꂽ��A
			if (m_playerGamePad != nullptr)
			{
				if (m_playerGamePad->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//�w�����[�h�A���p���[�h�A�X����o��X�^���o�C��Ԃ̂����ꂩ�ɍs���B
					NextState();
					//�����l�ɖ߂��Ă���
					m_nowShoppingMode = 0;

					//����T�E���h�̏�����
					m_decideSound = NewGO<SoundSource>();
					m_decideSound->Init(L"Assets/sound/Decide.wav");
					m_decideSound->SetVolume(0.5f);
					m_decideSound->Play(false);	//�����V���b�g�Đ�
				}
			}
			else
			{
				if (g_pad[m_playerNum]->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//�w�����[�h�A���p���[�h�A�X����o��X�^���o�C��Ԃ̂����ꂩ�ɍs���B
					NextState();
					//�����l�ɖ߂��Ă���
					m_nowShoppingMode = 0;

					//����T�E���h�̏�����
					m_decideSound = NewGO<SoundSource>();
					m_decideSound->Init(L"Assets/sound/Decide.wav");
					m_decideSound->SetVolume(0.5f);
					m_decideSound->Play(false);	//�����V���b�g�Đ�
				}
			}

			break;

			//�w�����
		case enBuyState:

			//�I���J�[�\���ړ�
			CursorMove(enFirstItem, enBackState);

			if (m_playerGamePad != nullptr)
			{
				//A�{�^���������ꂽ��A
				if (m_playerGamePad->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//�w��
					BuySelect();
				}
				//����X�{�^���������ꂽ�Ȃ�A
				if (m_playerGamePad->IsTrigger(enButtonX) && m_nowShoppingMode != enBackState)
				{
					//�\������Ă���Ƃ��A
					if (m_ItemSetumeiSprite->IsActive())
					{
						//��\���ɂ�����
						m_ItemSetumeiSprite->Deactivate();
					}
					//�\������Ă��Ȃ��Ƃ��A
					else
					{
						//�\��������
						m_ItemSetumeiSprite->Activate();
					}
				}
			}
			else
			{
				//A�{�^���������ꂽ��A
				if (g_pad[m_playerNum]->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//�w��
					BuySelect();
				}
				//����X�{�^���������ꂽ�Ȃ�A
				if (g_pad[m_playerNum]->IsTrigger(enButtonX) && m_nowShoppingMode != enBackState)
				{
					//�\������Ă���Ƃ��A
					if (m_ItemSetumeiSprite->IsActive())
					{
						//��\���ɂ�����
						m_ItemSetumeiSprite->Deactivate();
					}
					//�\������Ă��Ȃ��Ƃ��A
					else
					{
						//�\��������
						m_ItemSetumeiSprite->Activate();
					}
				}
			}

			break;

			//���p���
		case enSoldState:
			//�I���J�[�\���ړ�
			CursorMove(enStatusUpItem1, enBack);

			if (m_playerGamePad != nullptr)
			{
				//A�{�^���������ꂽ��
				if (m_playerGamePad->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//���p
					SoldSelect();
				}
				//����X�{�^���������ꂽ�Ȃ�A
				if (m_playerGamePad->IsTrigger(enButtonX) && m_nowShoppingMode != enBack)
				{
					//�\������Ă���Ƃ��A
					if (m_ItemSetumeiSprite->IsActive())
					{
						//��\���ɂ�����
						m_ItemSetumeiSprite->Deactivate();
					}
					//�\������Ă��Ȃ��Ƃ��A
					else
					{
						//�\��������
						m_ItemSetumeiSprite->Activate();
					}
				}
			}
			else
			{
				//A�{�^���������ꂽ��
				if (g_pad[m_playerNum]->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//���p
					SoldSelect();
				}
				//����X�{�^���������ꂽ�Ȃ�A
				if (g_pad[m_playerNum]->IsTrigger(enButtonX) && m_nowShoppingMode != enBack)
				{
					//�\������Ă���Ƃ��A
					if (m_ItemSetumeiSprite->IsActive())
					{
						//��\���ɂ�����
						m_ItemSetumeiSprite->Deactivate();
					}
					//�\������Ă��Ȃ��Ƃ��A
					else
					{
						//�\��������
						m_ItemSetumeiSprite->Activate();
					}
				}
			}

			break;

			//�X����o��X�^���o�C���
		case enEnterState:
			//��������ł��Ȃ�
			break;
		}
		//�A�C�e���V���b�v�Z���N�g�����N���X�̃V���b�s���O��Ԃ��X�V
		m_itemShopSelectFont->SetNowShoppingState(m_nowShoppingState);
		m_itemShopSelectFont->SetNowShoppingMode(m_nowShoppingMode);
	}

	//�I�����\�b�h
	void ItemShopSelect::CursorMove(const int selectMin, const int selectMax)
	{
		//������
		if (m_playerGamePad != nullptr)
		{
			if (m_playerGamePad->IsTrigger(enButtonDown) && !m_ItemSetumeiSprite->IsActive()) {
				//���݃Z���N�g����Ă���̂��u�o��v��������A
				if (m_nowShoppingMode == selectMax) {
					//�I������ԏ�ɖ߂�
					m_nowShoppingMode = selectMin;
				}
				else
				{
					//����ɂ��炷
					m_nowShoppingMode += 1;
				}

				//�I���T�E���h�̏�����
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//�����V���b�g�Đ�
			}
			//�����
			if (m_playerGamePad->IsTrigger(enButtonUp) && !m_ItemSetumeiSprite->IsActive()) {
				//���݃Z���N�g����Ă���̂��u�o��v��������A
				if (m_nowShoppingMode == selectMin) {
					//�I������ԉ��ɖ߂�
					m_nowShoppingMode = selectMax;
				}
				else
				{
					//���ɂ��炷
					m_nowShoppingMode -= 1;
				}

				//�I���T�E���h�̏�����
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//�����V���b�g�Đ�
			}
		}
		else
		{
			if (g_pad[m_playerNum]->IsTrigger(enButtonDown) && !m_ItemSetumeiSprite->IsActive()) {
				//���݃Z���N�g����Ă���̂��u�o��v��������A
				if (m_nowShoppingMode == selectMax) {
					//�I������ԏ�ɖ߂�
					m_nowShoppingMode = selectMin;
				}
				else
				{
					//����ɂ��炷
					m_nowShoppingMode += 1;
				}

				//�I���T�E���h�̏�����
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//�����V���b�g�Đ�
			}
			//�����
			if (g_pad[m_playerNum]->IsTrigger(enButtonUp) && !m_ItemSetumeiSprite->IsActive()) {
				//���݃Z���N�g����Ă���̂��u�o��v��������A
				if (m_nowShoppingMode == selectMin) {
					//�I������ԉ��ɖ߂�
					m_nowShoppingMode = selectMax;
				}
				else
				{
					//���ɂ��炷
					m_nowShoppingMode -= 1;
				}

				//�I���T�E���h�̏�����
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//�����V���b�g�Đ�
			}
		}
	}

	//���̏�Ԃɍs�����\�b�h
	void ItemShopSelect::NextState()
	{
		switch (m_nowShoppingMode)
		{
		case enBuy:
			//�w�����[�h�ɂ���
			m_nowShoppingState = enBuyState;
			break;
		case enSold:
			//���p���[�h�ɂ���
			m_nowShoppingState = enSoldState;
			break;
		case enEnter:
			//���X���o��X�^���o�C���[�h�ɂ���
			m_nowShoppingState = enEnterState;
			break;
		}
	}

	//�w������Ƃ��̑I��
	void ItemShopSelect::BuySelect()
	{
		switch (m_nowShoppingMode)
		{
			//�P�ڂ̃A�C�e����I�����Ă�����A
		case enFirstItem:
			//�������K�v���Ȃ��Ƃ��A
			if (m_playerStatus->GetCoinNum() < 100)
			{
				//�Ԃԁ[�T�E���h�̏�����
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//�����V���b�g�Đ�

				return;
			}
			if (m_playerStatus->GetMovePower() >= 90)
			{
				//�Ԃԁ[�T�E���h�̏�����
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//�����V���b�g�Đ�

				return;
			}

			//�ړ����x���A�b�v������
			m_playerStatus->AddMovePower();
			//���������炷
			m_playerStatus->ReduceMoney(100);

			//�A�C�e���������𑝂₷
			m_gameScreenUi->AddItemNum(m_playerNum,0);

			//�w���T�E���h�̏�����
			m_buySound = NewGO<SoundSource>();
			m_buySound->Init(L"Assets/sound/buy.wav");
			m_buySound->SetVolume(0.5f);
			m_buySound->Play(false);	//�����V���b�g�Đ�

			break;

			//�Q�ڂ̃A�C�e����I�����Ă�����A
		case enSecondItem:
			//�������K�v���Ȃ��Ƃ��A
			if (m_playerStatus->GetCoinNum() < 200)
			{
				//�Ԃԁ[�T�E���h�̏�����
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//�����V���b�g�Đ�

				return;
			}
			//�񕜃A�C�e���̐����P���₷
			m_playerStatus->AddRecoveryItemNum();
			//���������炷
			m_playerStatus->ReduceMoney(200);

			//�w���T�E���h�̏�����
			m_buySound = NewGO<SoundSource>();
			m_buySound->Init(L"Assets/sound/buy.wav");
			m_buySound->SetVolume(0.5f);
			m_buySound->Play(false);	//�����V���b�g�Đ�

			break;

			//�R�ڂ̃A�C�e����I�����Ă�����A
		case enThirdItem:
			//�������K�v���Ȃ��Ƃ��A
			if (m_playerStatus->GetCoinNum() < 500)
			{
				//�Ԃԁ[�T�E���h�̏�����
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//�����V���b�g�Đ�

				return;
			}
			if (m_playerStatus->GetHaveWeapon() ==3)
			{
				//�Ԃԁ[�T�E���h�̏�����
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//�����V���b�g�Đ�

				return;
			}
			//�����w��
			m_playerStatus->ChangeHavingWeapon(3);
			//�v���C���[�̎����Ă��镐��̃��f����ύX
			m_weapon->Init(3);
			//���������炷
			m_playerStatus->ReduceMoney(500);

			//�w���T�E���h�̏�����
			m_buySound = NewGO<SoundSource>();
			m_buySound->Init(L"Assets/sound/buy.wav");
			m_buySound->SetVolume(0.5f);
			m_buySound->Play(false);	//�����V���b�g�Đ�

			break;

			//�߂�
		case enBackState:
			//�����A����A�o��A�̑I����Ԃɖ߂�
			m_nowShoppingState = enShoppingModeSelectState;
			//�����l�ɖ߂��Ă���
			m_nowShoppingMode = 0;

			//����T�E���h�̏�����
			m_decideSound = NewGO<SoundSource>();
			m_decideSound->Init(L"Assets/sound/Decide.wav");
			m_decideSound->SetVolume(0.5f);
			m_decideSound->Play(false);	//�����V���b�g�Đ�

			break;
		}
	}

	//���p����Ƃ��̑I��
	void ItemShopSelect::SoldSelect()
	{
		//�u�߂�v���I������Ă�����A
		if (m_nowShoppingMode == enBack)
		{
			//�����A����A�o��A�̑I����Ԃɖ߂�
			m_nowShoppingState = enShoppingModeSelectState;
			//�����l�ɖ߂��Ă���
			m_nowShoppingMode = 0;

			//����T�E���h�̏�����
			m_decideSound = NewGO<SoundSource>();
			m_decideSound->Init(L"Assets/sound/Decide.wav");
			m_decideSound->SetVolume(0.5f);
			m_decideSound->Play(false);	//�����V���b�g�Đ�

			return;
		}

		//�����O�̎��A���p�����Ȃ��B
		if (m_gameScreenUi->GetStatusUpItemNum(m_playerNum, m_nowShoppingMode) <= 0)
		{
			//�Ԃԁ[�T�E���h�̏�����
			m_cantDecideSound = NewGO<SoundSource>();
			m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
			m_cantDecideSound->SetVolume(0.5f);
			m_cantDecideSound->Play(false);	//�����V���b�g�Đ�

			return;
		}

		//�w���T�E���h�̏�����
		m_buySound = NewGO<SoundSource>();
		m_buySound->Init(L"Assets/sound/buy.wav");
		m_buySound->SetVolume(0.5f);
		m_buySound->Play(false);	//�����V���b�g�Đ�

		//�\�̓A�b�v�������̂����ɖ߂�
		switch (m_nowShoppingMode)
		{
		case enStatusUpItem1:
			//�v���C���[�̈ړ��͂�������
			m_playerStatus->ReduceMovePower();
			break;

		case enStatusUpItem2:
			//�v���C���[�̃R�C���𑝂₷�{�������炷
			m_playerStatus->ReduceMoneyMagnification();
			break;

		case enStatusUpItem3:
			//�v���C���[�̃W�����v�͂�������
			m_playerStatus->ReduceJumpPower();
			break;

		case enStatusUpItem4:
			//�񕜃A�C�e���̐������炷
			m_playerStatus->ReduceRecoveryItemNum();
			break;

		case enStatusUpItem5:
			//�񕜃A�C�e���̉񕜗͂�������
			m_playerStatus->ReduceRecoveryPower();
			break;
		}

		//�����P���炷
		m_gameScreenUi->ReduceStatusUpItemNum(m_playerNum, m_nowShoppingMode);
		//���p�̒l�i�����炦��
		m_playerStatus->AddMoney(100);

	}
}


//���i�ꗗ//

//�r���V���[�Y
//m_player->AddMovePower();					//�ړ����x�A�b�v

//���^����
//m_player->AddMoneyMagnification();		//�擾�}�l�[�̎擾�{���A�b�v

//�V�g�̉H
//m_player->AddJumpPower();					//�W�����v�̓A�b�v

//�񕜔�
//m_player->AddRecoveryItemNum();			//�񕜃A�C�e��+1�R

//�n�[�g�N�b�L�[
//m_player->AddRecoveryPower();				//�񕜗͂�+1

//���햼
//m_player->ChangeHavingWeapon(����ԍ�);	//����w��(���łɎ����Ă��镐��Ɠ���ւ�)