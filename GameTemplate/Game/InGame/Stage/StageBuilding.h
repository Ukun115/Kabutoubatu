/// <summary>
/// �X�e�[�W��̌����N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;			//�v���C���[
	class PlayerRotation;	//�v���C���[�̉�]
	class PlayerAnimation;	//�v���C���[�̃A�j���[�V����
	class PlayerCamera;		//�v���C���[�J����
	class AccompanyAI;		//���sAI
	class HotelScene;		//�h���V�[��
	class ItemShopScene;	//�A�C�e���V���b�v�V�[��
	class FirstBossStage;	//�ŏ��̃{�X
	class Fade;				//�t�F�[�h
	class OnlineUpdateSpeed;

	class StageBuilding : public IGameObject
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

		//�v���C���[�̏��
		enum enPlayerNowState
		{
			enNormal,			//�ʏ�
			enSufferAnAttack,	//�U�����󂯂����
			enGhost,			//�S�[�X�g���
			enDontMove			//�������Ȃ����
		};

		//�h�A�̏��
		enum enDoorState
		{
			enClose,	//�J����
			enOpen		//����
		};

		//�V���b�v�̃^�C�v
		enum enShopType
		{
			enHotel,			//�z�e��
			enItemShop,			//�A�C�e���V���b�v
			enFirstBossHouse	//�ŏ��̃{�X�̌���
		};

		SkinModelRender* m_building = nullptr;		//�������f��
		SkinModelRender* m_door = nullptr;	//�h�A���f��
		SkinModelRender* m_darknessEntrance = nullptr;	//�Èœ������f��
		Fade* m_fade = nullptr;
		SpriteRender* m_hukidasiSprite[enPlayerNum] = { nullptr };
		Player* m_player[enPlayerNum] = { nullptr };
		PlayerRotation* m_playerRotation[enPlayerNum] = { nullptr };
		PlayerAnimation* m_playerAnimation[enPlayerNum] = { nullptr };
		PlayerCamera* m_playerCamera = nullptr;
		AccompanyAI* m_accompaniAI = nullptr;
		SoundSource* m_buildingSound = nullptr;
		SoundSource* m_doorOpenSound = nullptr;
		SoundSource* m_doorCloseSound = nullptr;
		//�h���V�[��
		HotelScene* m_hotelScene = nullptr;
		//�A�C�e���V���b�v�V�[��
		ItemShopScene* m_itemShopScene = nullptr;
		//�ŏ��̃{�X�X�e�[�W
		FirstBossStage* m_firstBossStage = nullptr;
		OnlineUpdateSpeed* m_onlineUpdateSpeed = nullptr;
		GamePad* m_gamePad[enPlayerNum] = { nullptr };

		PhysicsStaticObject m_physicsStaticObject[2];		//�ÓI�����I�u�W�F�N�g�N���X

		int m_playerNum = enPlayerNum;
		Vector2 m_playerScreenPos;
		//�v���C���[���猚���ɂ̂т�x�N�g��
		Vector3 m_newPlayerMoveDirection;
		Vector3 m_pos;		//�ʒu
		Vector3 m_doorPos;		//�h�A�̈ʒu
		Quaternion m_rot;	//��]
		//�V�[���ɓ���܂ł̒x���^�C�}�[
		int m_sceneChangeDelayTimer = 0;
		int m_doorSlideTimer = 0;
		int m_gyaku = 1;
		int m_doorState = enOpen;
		float m_rotationAngle = 0.0f;
		float m_buildingEnterLength = 0.0f;
		float m_canDoorTatchLength = 0.0f;
		//�V���b�v�^�C�v
		int m_shopType = enHotel;
		/// <summary>
		/// �t���O
		/// </summary>
		//�h�A�ɐG��Ă��邩�ǂ���
		bool m_doorTatchFlg[enPlayerNum] = { false };

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~StageBuilding();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//���f���̏��������\�b�h
		void BuildingModelInit();

		//�h�A���f���̏��������\�b�h
		void DoorModelInit();

		//�Èœ������f���̏��������\�b�h
		void DarknessEntranceInit();

		//�����ɓ��������ǂ����̔��胁�\�b�h
		void JudgmentEntered();

		//�V�[���J�ڃ��\�b�h
		void SceneTransition();

		//�����o����\�������郁�\�b�h
		void HukidasiActivate(const int playerNum);

	public:
		//�ʒu�Z�b�g���\�b�h
		void SetPosition(const Vector3& pos) { m_pos = pos; };

		//�V���b�v�̃^�C�v���Z�b�g���郁�\�b�h
		void SetShopType(const int shopType) { m_shopType = shopType; };

		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };

		/// <summary>
		/// �h�A�ɐG��Ă��邩�ǂ������擾
		/// </summary>
		/// <returns>�h�A�ɐG��Ă��邩�ǂ���</returns>
		bool GetDoorTatchFlg(const int playerNum){ return m_doorTatchFlg[playerNum]; };

		void SetPlayerGamePad(GamePad& gamePad,const int playerNo) { m_gamePad[playerNo] = &gamePad; };
	};
}