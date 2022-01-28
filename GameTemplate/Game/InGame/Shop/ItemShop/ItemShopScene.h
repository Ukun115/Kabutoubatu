/// <summary>
/// �A�C�e���V���b�v�V�[���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class ItemShopSelect;	//�A�C�e���V���b�v�V�[���̑I��
	class Player;			//�v���C���[
	class PlayerCamera;		//�v���C���[�̃J����
	class AccompanyAI;		//���sAI
	class GameScene;		//�Q�[���V�[��
	class GameLimitTime;	//�Q�[���̐�������
	class Fade;				//�t�F�[�h

	class ItemShopScene : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�v���C���[�ԍ�
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//�v���C���[�̑���
		};

		SpriteRender* m_coinSprite[3] = { nullptr };
		SkinModelRender* m_itemShopInterior = nullptr;	//�A�C�e���V���b�v�̓������f��
		SkinModelRender* m_shopItem[3] = { nullptr };	//�A�C�e���V���b�v�̏��i���f��
		SpriteRender* m_hukidasi = nullptr;
		AccompanyAI* m_accompanyAI = nullptr;
		PlayerCamera* m_playerCamera = nullptr;
		Player* m_player[enPlayerNum] = { nullptr };
		GameScene* m_gameScene = nullptr;
		GameLimitTime* m_gameLimitTime = nullptr;
		SoundSource* m_shopBGM = nullptr;
		Fade* m_fade[2] = { nullptr };
		SkinModelRender* m_salesperson = nullptr;	//�X�����f��
		FontRender* m_salespersonTalk = nullptr;	//�X���̉�b��
		ItemShopSelect* m_ItemShopSelect[enPlayerNum] = { nullptr };

		PhysicsStaticObject m_physicsStaticObject;		//�ÓI�����I�u�W�F�N�g�N���X
		AnimationClip m_animationClips[1];

		int m_playerNum = enPlayerNum;
		float m_coinSpritePos = 0.0f;
		Vector3 m_shopPos;	//�X�e�[�W��̃V���b�v�̈ʒu
		wchar_t m_talkText[256];	//�X���̉�b��
		Vector3 m_pos;		//�V���b�v�̈ʒu
		Vector3 m_itemPos;		//�A�C�e���̈ʒu
		Quaternion m_rot;	//��]

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~ItemShopScene();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//�A�C�e���V���b�v�̓������f���̏��������\�b�h
		void ItemShopInteriorModelInit();

		//�X�����f���̏��������\�b�h
		void SalespersonModelInit();

		//�A�C�e���V���b�v����o�郁�\�b�h
		void EnterFromShop();

	public:
		//�X�e�[�W��̃V���b�v�̃|�W�V����
		void SetItemShopPosition(Vector3& shopPosition) { m_shopPos = shopPosition; };

		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };
	};
}