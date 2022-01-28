/// <summary>
/// �X�e�[�W�N���X
/// </summary>
#include "stdafx.h"
#include "Stage.h"
#include "StageBuilding.h"
#include "../Enemy/WeakEnemy/Mole.h"
#include "../Enemy/WeakEnemy/Slime.h"
#include "../Enemy/WeakEnemy/NeedleSlime.h"
#include "../Player/Player.h"

namespace nsKabutoubatu
{
	namespace nsStage
	{
		const Vector3 BUILDING_POS[3] = { { -300.0f, 0.0f, -1400.0f },{ -1800.0f, 0.0f, -7500.0f },{ -5850.0f, 0.0f, -7700.0f } };

		const Vector3 WEAK_ENEMY_FIRST_RESPOWN_POS[12] =
		{
			{ -1950.0f,0.0f,-740.0f },{ -1795.0f,0.0f,-740.0f },
			{ -1950.0f,0.0f,-1400.0f },{ -1950.0f,0.0f,-2850.0f },
			{ -2500.0f,0.0f,-6230.0f },{ -930.0f,100.0f,-60.0f },
			{ -3000.0f,100.0f,-6230.0f },{ -1700.0f,100.0f,-3000.0f },
			{ -3000.0f,0.0f,-5500.0f },{ -1200.0f,100.0f,-60.0f },
			{ -1400.0f,0.0f,-60.0f },{ -1800.0f,100.0f,-4060.0f }
		};
	}

	bool Stage::Start()
	{
		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
		}

		//�C���f���̏�����
		m_seaModel = NewGO<SkinModelRender>();
		m_seaModel->Init("sea");

		//���ꃂ�f���̏�����
		m_floorModel = NewGO<SkinModelRender>();
		m_floorModel->Init("FirstStage_map1_floor");
		//�����蔻���K��
		m_physicsStaticObject[0].CreateFromModel(*m_floorModel->GetModel(), m_floorModel->GetModel()->GetWorldMatrix());

		//�X�e�[�W��̓����蔻�肪���鑕�����f���̏�����
		m_hitStageAcceModel = NewGO<SkinModelRender>();
		m_hitStageAcceModel->SetShadowCaster(true);	//�V���h�E�͗��Ƃ�
		m_hitStageAcceModel->SetOutline(true);		//�֊s��������
		m_hitStageAcceModel->SetSilhouette(false);
		m_hitStageAcceModel->Init("FirstStage_map1_hitAcce");
		//�����蔻���K��
		m_physicsStaticObject[1].CreateFromModel(*m_hitStageAcceModel->GetModel(), m_hitStageAcceModel->GetModel()->GetWorldMatrix());

		//�X�e�[�W�̓����蔻��̂Ȃ��������f���̏�����
		m_dontHitStageAcceModel = NewGO<SkinModelRender>();
		m_dontHitStageAcceModel->SetShadowCaster(true);	//�V���h�E�͗��Ƃ�
		m_dontHitStageAcceModel->SetOutline(true);		//�֊s��������
		m_dontHitStageAcceModel->SetSilhouette(false);
		m_dontHitStageAcceModel->Init("FirstStage_map1_dontHitAcce");

		//�V���b�v�̌������X�e�[�W��ɐ���
		for (int shopNum = 0; shopNum < 3; shopNum++)
		{
			m_shopBuiding[shopNum] = NewGO<StageBuilding>();
			m_shopBuiding[shopNum]->SetPlayerNumber(m_playerNum);
			//�A�C�e���V���b�v���h�����ŏ��̃{�X�̉Ƃ���ݒ�
			m_shopBuiding[shopNum]->SetShopType(shopNum);
			//�ʒu���Z�b�g
			switch (shopNum)
			{
			case 0:
				m_shopBuiding[shopNum]->SetPosition(nsStage::BUILDING_POS[0]);
				break;
			case 1:
				m_shopBuiding[shopNum]->SetPosition(nsStage::BUILDING_POS[1]);
				break;
			case 2:
				m_shopBuiding[shopNum]->SetPosition(nsStage::BUILDING_POS[2]);
			}
		}

		//�U�R�G�𐶐�
		for (int weakEnemyNum = 0; weakEnemyNum < 4; weakEnemyNum++)
		{
			m_mole[weakEnemyNum] = NewGO<Mole>();
			m_mole[weakEnemyNum]->SetPlayerNum(m_playerNum);
			m_slime[weakEnemyNum] = NewGO<Slime>();
			m_slime[weakEnemyNum]->SetPlayerNum(m_playerNum);
			m_needleSlime[weakEnemyNum] = NewGO<NeedleSlime>(nsStdafx::PRIORITY_0,nsStdafx::NEEDLE_SLIME_NAME);
			m_needleSlime[weakEnemyNum]->SetPlayerNum(m_playerNum);
		}
		//��������������X�|�[���ʒu�ɃZ�b�g
		m_mole[0]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[0]);
		m_mole[1]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[1]);
		m_mole[2]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[2]);
		m_mole[3]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[3]);
		//�X���C�����������X�|�[���ʒu�ɃZ�b�g

		m_slime[0]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[4]);
		m_slime[1]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[5]);
		m_slime[2]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[6]);
		m_slime[3]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[7]);
		//�Ƃ��X���C�����������X�|�[���ʒu�ɃZ�b�g
		m_needleSlime[0]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[8]);
		m_needleSlime[1]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[9]);
		m_needleSlime[2]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[10]);
		m_needleSlime[3]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[11]);

		return true;
	}

	Stage::~Stage()
	{
		//�C���f���̍폜
		DeleteGO(m_seaModel);
		//���ꃂ�f�����폜
		DeleteGO(m_floorModel);
		//�X�e�[�W��̓����蔻�肪���鑕�����f�����폜
		DeleteGO(m_hitStageAcceModel);
		//�X�e�[�W�̓����蔻��̂Ȃ��������f�����폜
		DeleteGO(m_dontHitStageAcceModel);
		//�V���b�v�̌������폜
		for (int shopNum = 0; shopNum < 3; shopNum++)
		{
			//�V���b�v�������폜
			DeleteGO(m_shopBuiding[shopNum]);
		}
		//��G���폜
		for (int weakEnemyNum = 0; weakEnemyNum < 4; weakEnemyNum++)
		{
			DeleteGO(m_mole[weakEnemyNum]);
			DeleteGO(m_slime[weakEnemyNum]);
			DeleteGO(m_needleSlime[weakEnemyNum]);
		}
	}

	void Stage::Update()
	{
		//�U�R�G�̃��X�|�[������
		WeakEnemyResporn();

		PlayerAttackOk();
	}

	//�U�R�G�̃��X�|�[���������\�b�h
	void Stage::WeakEnemyResporn()
	{
		for (int weakEnemyNum = 0; weakEnemyNum < 4; weakEnemyNum++)
		{
			//�����炪���񂾂Ƃ��A
			if (m_mole[weakEnemyNum]->IsDead() || m_mole[weakEnemyNum]->GetFall())
			{
				DeleteGO(m_mole[weakEnemyNum]);
				//�V����������𐶐�������B
				m_mole[weakEnemyNum] = NewGO<Mole>();
				//�Q�[���ɎQ�����Ă���v���C���[�̐l����n���Ă����B
				m_mole[weakEnemyNum]->SetPlayerNum(m_playerNum);
				//�X�|�[���ʒu��ݒ肷��B
				switch (weakEnemyNum)
				{
				case 0:
					m_mole[0]->SetInitPosition(WeakEnemyRandomRespownPos(-2100, -177, -1537, -6828));
					break;
				case 1:
					m_mole[1]->SetInitPosition(WeakEnemyRandomRespownPos(-2100, -177, -1537, -6828));
					break;
				case 2:
					m_mole[2]->SetInitPosition(WeakEnemyRandomRespownPos(-2100, -177, -1537, -6828));
					break;
				case 3:
					m_mole[3]->SetInitPosition(WeakEnemyRandomRespownPos(-2100, -177, -1537, -6828));
					break;
				}
			}
			//�X���C�������񂾂Ƃ��A
			if (m_slime[weakEnemyNum]->IsDead() || m_slime[weakEnemyNum]->GetFall())
			{
				DeleteGO(m_slime[weakEnemyNum]);
				//�V�����X���C���𐶐�������B
				m_slime[weakEnemyNum] = NewGO<Slime>();
				//�Q�[���ɎQ�����Ă���v���C���[�̐l����n���Ă����B
				m_slime[weakEnemyNum]->SetPlayerNum(m_playerNum);
				//�X�|�[���ʒu��ݒ肷��B
				switch (weakEnemyNum)
				{
				case 0:
					m_slime[0]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 1:
					m_slime[1]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 2:
					m_slime[2]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 3:
					m_slime[3]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				}
			}
			//�Ƃ��X���C�������񂾂Ƃ��A
			if (m_needleSlime[weakEnemyNum]->IsDead()|| m_needleSlime[weakEnemyNum]->GetFall())
			{
				DeleteGO(m_needleSlime[weakEnemyNum]);
				//�V�����Ƃ��X���C���𐶐�������B
				m_needleSlime[weakEnemyNum] = NewGO<NeedleSlime>();
				//�Q�[���ɎQ�����Ă���v���C���[�̐l����n���Ă����B
				m_needleSlime[weakEnemyNum]->SetPlayerNum(m_playerNum);
				//�X�|�[���ʒu��ݒ肷��B
				switch (weakEnemyNum)
				{
				case 0:
					m_needleSlime[0]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 1:
					m_needleSlime[1]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 2:
					m_needleSlime[2]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 3:
					m_needleSlime[3]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				}
			}
		}
	}

	void Stage::PlayerAttackOk()
	{
		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			for (int shopNum = 0; shopNum < 3; shopNum++)
			{
				if (m_shopBuiding[shopNum]->GetDoorTatchFlg(playerNum))
				{
					return;
				}
				if (shopNum == 2)
				{
					m_player[playerNum]->SetNowDoorTatch(false);
				}
			}
		}
	}

	Vector3 Stage::WeakEnemyRandomRespownPos(const int x1, const int z1,const int x2,const int z2)
	{
		m_weakEnemyRandomRespownPos.y = 500.0f;
		m_weakEnemyRandomRespownPos.x = x1 + (x2 - x1) * ((rand()%100) / 100.0f);
		m_weakEnemyRandomRespownPos.z = z1 + (z2 - z1) * ((rand()%100) / 100.0f);
		return m_weakEnemyRandomRespownPos;

		//�l�p�`�̊p�̈ʒu
		//(-798,-567)��(-2100,177)��(-1537,-6828)��(-6810,-6095)
	}
}