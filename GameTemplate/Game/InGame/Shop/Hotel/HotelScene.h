/// <summary>
/// �h���V�[���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class HotelSelect;		//�h���V�[���̑I��
	class Player;			//�v���C���[
	class PlayerCamera;		//�v���C���[�̃J����
	class PlayerStatus;		//�v���C���[�̃X�e�[�^�X
	class AccompanyAI;		//���sAI
	class GameScene;		//�Q�[���V�[��
	class GameLimitTime;	//�Q�[���̐�������
	class Fade;				//�t�F�[�h

	class HotelScene : public IGameObject
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

		//�A�j���[�V����
		enum enAnimation
		{
			enIdle,			//�A�C�h��
			enGreething,	//���A
			enAnimationNum	//�A�j���[�V�����̑���
		};


		PlayerCamera* m_playerCamera = nullptr;
		Player* m_player[enPlayerNum] = { nullptr };
		PlayerStatus* m_playerStatus[enPlayerNum] = { nullptr };
		AccompanyAI* m_accompanyAI = nullptr;
		GameLimitTime* m_gameLimitTime = nullptr;
		Fade* m_fade[2] = { nullptr };
		GameScene* m_gameScene = nullptr;
		SoundSource* m_shopBGM = nullptr;
		SpriteRender* m_hukidasi = nullptr;
		SkinModelRender* m_hotelInterior = nullptr;	//�h���̓������f��
		FontRender* m_salespersonTalk = nullptr;	//�X���̉�b��
		SkinModelRender* m_salesperson = nullptr;	//�X�����f��
		HotelSelect* m_hotelSelect[enPlayerNum] = { nullptr };
		SpriteRender* m_coinSprite[2] = { nullptr };
		GamePad* m_playerGamePad[enPlayerNum] = { nullptr };

		PhysicsStaticObject m_physicsStaticObject;		//�ÓI�����I�u�W�F�N�g�N���X
		AnimationClip m_animationClips[enAnimationNum];

		int m_playerNum = enPlayerNum;
		wchar_t m_talkText[256];	//�X���̉�b��
		Vector3 m_hotelPos;	//�X�e�[�W��̏h���̈ʒu
		Vector3 m_pos;		//�h���̈ʒu
		Quaternion m_rot;	//��]
		float m_coinPos = -355.0f;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~HotelScene();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//�h���̓������f���̏��������\�b�h
		void HotelInteriorModelInit();

		//�X�����f���̏��������\�b�h
		void SalespersonModelInit();

		//�h������o�郁�\�b�h
		void EnterFromHotel();

		void CoinDeactivate(const int playerNum);

	public:
		//�X�e�[�W��̏h���̃|�W�V����
		void SetHotelPosition(Vector3& hotelPosition) { m_hotelPos = hotelPosition; };

		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };

		void SetPlayerGamePad(GamePad& gamePad, const int playerNo) { m_playerGamePad[playerNo] = &gamePad; };
	};
}