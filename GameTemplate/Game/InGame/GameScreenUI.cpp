/// <summary>
/// �Q�[�����UI�N���X
/// </summary>
#include "stdafx.h"
#include "GameScreenUI.h"
#include "Player/PlayerStatus.h"

//UI�̃��C���[
//�O.�t���[���̔w�i�@���@�P.�t���[���@���@�Q.�A�C�R���@���@�R.�q�b�g�|�C���gUI

namespace nsKabutoubatu
{
	namespace nsGameScreenUI
	{
		const char* KOTEI_UI_NAME[3] = { "GameScreenUI_Back","GameScreenUI_flame","GameScreenUI_Icon_second" };
		const char* PLAYER_ICON_UI_NAME[2] = { "Player1_icon","Player2_icon" };

		const Vector2   ITEM_NUM_POS[2] = { { -650.0f,-310.0f }, { -15.0f,-310.0f } };			//�������ԃt�H���g�̈ʒu
		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//�������ԃt�H���g�̐F
		const float     FONT_ROT = 0.0f;			//�t�H���g�̌X��
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//�t�H���g�̊�_
		const float     FONT_SCA = 0.5f;			//�t�H���g�̊g�嗦
	}

	bool GameScreenUI::Start()
	{
		//�v���C���[�Q�l��
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//�v���C���[�̃C���X�^���X������
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);

			//�q�b�g�|�C���gUI��������
			HitPointUiInit(playerNum);

			//����UI��������
			HaveWeaponUiInit(playerNum);

			//�A�C�e��UI��������
			ItemUiInit(playerNum);
			//�A�C�e�����t�H���g��������
			ItemFontInit(playerNum);

			//�v���C���[�̃A�C�R���摜
			m_playerIconUI[playerNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_3);
			m_playerIconUI[playerNum]->Init(nsGameScreenUI::PLAYER_ICON_UI_NAME[playerNum], 1280, 720);
		}
		//�Œ�UI������
		for (int fixedUiNum = enFlame; fixedUiNum < enUiNum; fixedUiNum++)
		{
			m_fixedUI[fixedUiNum] = NewGO<SpriteRender>(fixedUiNum);
			m_fixedUI[fixedUiNum]->Init(nsGameScreenUI::KOTEI_UI_NAME[fixedUiNum], 1280, 720);
		}

		return true;
	}

	GameScreenUI::~GameScreenUI()
	{
		//�Œ�UI�폜
		for (int uiNum = enFlame; uiNum < enUiNum; uiNum++)
		{
			DeleteGO(m_fixedUI[uiNum]);
		}
		//�q�b�g�|�C���gUI���폜
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			for (int hitPointNum = 0; hitPointNum < 10; hitPointNum++)
			{
				DeleteGO(m_HitPointUI[playerNum][hitPointNum]);
			}
			//��������UI���폜
			for (int weaponNum = enWoodStick; weaponNum < enWeaponNum; weaponNum++)
			{
				DeleteGO(m_haveWeaponUI[playerNum][weaponNum]);
			}
		}

		//�v���C���[�̃A�C�R���摜���폜
		for (int playerNum = enPlayer1; playerNum < enPlayerNum; playerNum++)
		{
			DeleteGO(m_playerIconUI[playerNum]);
		}

		//�X�e�[�^�X�A�b�v�A�C�e���̐��t�H���g���폜
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			for (int itemNum = enItem1; itemNum < enItemNum; itemNum++)
			{
				DeleteGO(m_itemUI[playerNum][itemNum]);
				DeleteGO(m_haveItemNum[playerNum][itemNum]);
			}
		}
	}

	void GameScreenUI::Update()
	{
		//�v���C���[�̃q�b�g�|�C���g��\����Ԃɂ��邩��\����Ԃɂ��邩���X�V���鏈��
		HitPointUiUpdate();

		//�q�b�g�|�C���g�����Ȃ��Ƃ��Ƀn�[�g��_�ł����鏈��
		BlinkingHeart();

		//��������̉摜�\���X�V����
		HaveWeapon();

		//�����A�C�e�����X�V
		ItemNumUpdate();
	}

	//�q�b�g�|�C���gUI�̏��������\�b�h
	void GameScreenUI::HitPointUiInit(const int playerNum)
	{
		//�q�b�g�|�C���gUI��������
		for (int hitPointNum = 0; hitPointNum < 10; hitPointNum++)
		{
			m_HitPointUI[playerNum][hitPointNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_3);
			switch (playerNum)
			{
			case enPlayer1:
				sprintf(m_hitPointFileName, "Player1_HP_%d_second", (hitPointNum + 1));
				break;
			case enPlayer2:
				sprintf(m_hitPointFileName, "Player2_HP_%d_second", (hitPointNum + 1));
				break;
			}
			m_HitPointUI[playerNum][hitPointNum]->Init(m_hitPointFileName, 1280, 720);
		}
	}

	void GameScreenUI::HaveWeaponUiInit(const int playerNum)
	{
		for (int weaponNum = enWoodStick; weaponNum < enWeaponNum; weaponNum++)
		{
			m_haveWeaponUI[playerNum][weaponNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_3);
			switch (weaponNum)
			{
			//�؂̖_
			case enWoodStick:
				sprintf(m_weaponFileName, "Player%d_woodStick", playerNum + 1);
				break;
			//��
			case enSword:
				sprintf(m_weaponFileName, "Player%d_sword", playerNum + 1);
				break;
			//�匕
			case enLargeSword:
				sprintf(m_weaponFileName, "Player%d_largeSword", playerNum + 1);
				break;
			case enBow:
				sprintf(m_weaponFileName, "Player%d_Bow", playerNum + 1);
				break;
			}
			m_haveWeaponUI[playerNum][weaponNum]->Init(m_weaponFileName, 1280, 720);
			m_haveWeaponUI[playerNum][weaponNum]->Deactivate();	//��\��
		}
	}

	void GameScreenUI::ItemUiInit(const int playerNum)
	{
		for (int itemNum = enItem1; itemNum < enItemNum; itemNum++)
		{
			m_itemUI[playerNum][itemNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_3);
			switch (itemNum)
			{
				//�A�C�e���P
			case enItem1:
				sprintf(m_itemFileName, "Player%d_Item1_have", playerNum + 1);
				break;
				//�A�C�e���Q
			case enItem2:
				sprintf(m_itemFileName, "Player%d_Item2_have", playerNum + 1);
				break;
				//�A�C�e���R
			case enItem3:
				sprintf(m_itemFileName, "Player%d_Item3_have", playerNum + 1);
				break;
				//�A�C�e���S
			case enItem4:
				sprintf(m_itemFileName, "Player%d_Item4_have", playerNum + 1);
				break;
				//�A�C�e���T
			case enItem5:
				sprintf(m_itemFileName, "Player%d_Item5_have", playerNum + 1);
				break;
			}
			m_itemUI[playerNum][itemNum]->Init(m_itemFileName, 1280, 720);
		}
	}

	//�v���C���[�̃q�b�g�|�C���g��\����Ԃɂ��邩��\����Ԃɂ��邩���X�V���鏈�����\�b�h
	void GameScreenUI::HitPointUiUpdate()
	{
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//�\��
			for (int nowHitPoint = 0; nowHitPoint < m_playerStatus[playerNum]->GetHitPoint(); nowHitPoint++)
			{
				m_HitPointUI[playerNum][nowHitPoint]->Activate();
			}
			//��\��
			for (int nowHitPoint = m_playerStatus[playerNum]->GetHitPoint(); nowHitPoint < m_playerStatus[playerNum]->GetMaxHitPoint(); nowHitPoint++)
			{
				if (nowHitPoint < 0)
				{
					continue;
				}
				m_HitPointUI[playerNum][nowHitPoint]->Deactivate();
			}
		}
	}

	//�q�b�g�|�C���g�����Ȃ��Ƃ��n�[�g���_�ł��鏈��
	void GameScreenUI::BlinkingHeart()
	{
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//�c��q�b�g�|�C���g���P�̂Ƃ��A
			if (m_playerStatus[playerNum]->GetHitPoint() == 1)
			{
				//�_��
				if (m_blinkingTimer[playerNum] < 30)
				{
					//�\��
					m_HitPointUI[playerNum][0]->Activate();
				}
				else if(m_blinkingTimer[playerNum] < 60)
				{
					//��\��
					m_HitPointUI[playerNum][0]->Deactivate();
				}
				else
				{
					//�^�C�}�[��������
					m_blinkingTimer[playerNum] = 0;
				}
					m_blinkingTimer[playerNum]++;
			}
			else if(m_playerStatus[playerNum]->GetHitPoint() != 0)
			{
				//�^�C�}�[��������
				m_blinkingTimer[playerNum] = 0;
				//�\��
				m_HitPointUI[playerNum][0]->Activate();
			}
		}
	}

	//��������̉摜�\���X�V����
	void GameScreenUI::HaveWeapon()
	{
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//��U�S����\��
			for (int weaponNum = enWoodStick; weaponNum < enWeaponNum; weaponNum++)
			{
				m_haveWeaponUI[playerNum][weaponNum]->Deactivate();
			}
			//�����Ă��镐��UI�����\��
			m_haveWeaponUI[playerNum][m_playerStatus[playerNum]->GetHaveWeapon()]->Activate();
		}
	}

	//�A�C�e�������������̏��������\�b�h
	void GameScreenUI::ItemFontInit(const int playerNum)
	{
		m_itemFontPos = nsGameScreenUI::ITEM_NUM_POS[playerNum];

		for (int itemNum = enItem1; itemNum < enItemNum; itemNum++)
		{
			m_itemFontPos.x += 91.5f;

			m_haveItemNum[playerNum][itemNum] = NewGO<FontRender>();
			m_haveItemNum[playerNum][itemNum]->Init
			(
				L"",			//�e�L�X�g
				m_itemFontPos,	//�ʒu
				nsGameScreenUI::FONT_COLOR,	//�F
				nsGameScreenUI::FONT_ROT,		//�X��
				nsGameScreenUI::FONT_SCA,		//�g�嗦
				nsGameScreenUI::FONT_PIV		//��_
			);
			//�����̋��E���\��
			m_haveItemNum[playerNum][itemNum]->SetShadowParam(true, 1.5f, Vector4::Black);
		}
	}

	//�\�̓A�b�v�A�C�e���̏����������̐��X�V
	void GameScreenUI::ItemNumUpdate()
	{
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			for (int itemNum = enItem1; itemNum < enItemNum; itemNum++)
			{
				if (m_itemNum[playerNum][itemNum] > 0)
				{
					m_itemUI[playerNum][itemNum]->SetMulColor(1.0f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					m_itemUI[playerNum][itemNum]->SetMulColor(1.0f, 1.0f, 1.0f, 1.0f);
					//m_itemUI[playerNum][itemNum]->SetMulColor(0.15f, 0.15f, 0.15f, 1.0f);
				}
				//�v���C���[�̏������Ă���񕜃A�C�e�������Z�b�g���A�X�V�B
				swprintf_s(m_itemText, L"%d", m_itemNum[playerNum][itemNum]);
				m_haveItemNum[playerNum][itemNum]->SetText(m_itemText);
			}
		}
	}
}