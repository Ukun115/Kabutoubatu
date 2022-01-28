/// <summary>
/// �X�e�[�W�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class StageBuilding;	//�X�e�[�W��̌���
	class Player;			//�v���C���[
	class Mole;				//���O��(�U�R�G)
	class Slime;			//�X���C��(�U�R�G)
	class NeedleSlime;		//�g�Q�X���C��(�U�R�G)

	class Stage : public IGameObject
	{
	private:
		SkinModelRender* m_floorModel = nullptr;		//�X�e�[�W�̑��ꃂ�f��
		SkinModelRender* m_hitStageAcceModel = nullptr;		//�X�e�[�W��̓����蔻��̂��鑕�����f��
		SkinModelRender* m_dontHitStageAcceModel = nullptr;	//�����蔻��̂Ȃ��X�e�[�W�̑������f��
		SkinModelRender* m_seaModel = nullptr;	//�C���f��
		Player* m_player[2] = { nullptr };
		StageBuilding* m_shopBuiding[4] = { nullptr };
		Mole* m_mole[4] = { nullptr };
		Slime* m_slime[4] = { nullptr };
		NeedleSlime* m_needleSlime[4] = { nullptr };

		PhysicsStaticObject m_physicsStaticObject[2];		//�ÓI�����I�u�W�F�N�g�N���X

		Vector3 m_weakEnemyRandomRespownPos;
		int m_playerNum = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Stage();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//�U�R�G�̃��X�|�[���������\�b�h
		void WeakEnemyResporn();

		void PlayerAttackOk();

		Vector3 WeakEnemyRandomRespownPos(const int x1, const int z1, const int x2, const int z2);

	public:
		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };
	};
}