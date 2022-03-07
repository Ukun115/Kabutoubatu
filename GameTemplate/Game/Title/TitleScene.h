/// <summary>
/// �^�C�g���V�[���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class GameScene;			//�Q�[���V�[��
	class Online;				//�I�����C���ʐM
	class Pause;				//�|�[�Y�@�\
	class TitleSceneSprite;		//�^�C�g���V�[���̉摜
	class TitleSceneSound;		//�^�C�g���V�[���̃T�E���h

	class TitleScene : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�I�����
		enum enStartOrEndState
		{
			enSelectStart,	//�͂��߂�
			enSelectEnd		//�����
		};
		//�I�����[�h
		enum enSelectGameMode
		{
			enSelectOnline,		//�I�����C�����[�h
			enSelectLocalMult,	//���[�J���}���`���[�h
			enSelectSolo,		//�\�����[�h
			enSelectBack		//���ǂ�
		};

		//�I�����C�����[���쐬�E�Q�����
		enum enOnlineRoomCreateAndJoinState
		{
			enRoomCreate,				//���[���쐬
			enRoomJoin,					//���[���Q��
			enSelectBackGameMode,		//�Q�[�����[�h�I���ɂ��ǂ�
		};


		enum enPlayerNo
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//�v���C���[�̑���
		};

		//�Q�[���V�[��
		GameScene* m_gameScene = nullptr;
		TitleSceneSprite* m_titleSceneSprite = nullptr;
		TitleSceneSound* m_titleSceneSound = nullptr;
		Pause* m_pause = nullptr;
		Online* m_online = nullptr;

		int m_cursorSelect = 0;
		int m_selectState = 0;
		char m_gamePadNo[256];
		int m_playerNo = 0;
		int m_otherPlayerNo = 0;
		/// <summary>
		/// �t���O
		/// </summary>
		bool m_isRoomCreate = false;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~TitleScene();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//�Q�[���V�[���ɑJ�ڂ��郁�\�b�h
		void ChangeGameScene(const int mode);

		//�J�[�\���̈ړ�����
		void CursorMove(const int lastSelect);

	public:
		/// <summary>
		/// �I����Ԃ��擾
		/// </summary>
		/// <returns>�I�����</returns>
		int GetState()const { return m_selectState; };

		/// <summary>
		/// �J�[�\���I����Ԃ��擾
		/// </summary>
		/// <returns>�J�[�\���I�����</returns>
		int GetCursorSelect()const { return m_cursorSelect; };

		/// <summary>
		/// �I�����C���ʐM�̕������쐬�������ǂ������擾
		/// </summary>
		/// <returns>�I�����C���ʐM�̕������쐬�������ǂ���</returns>
		bool GetIsRoomCreate()const { return m_isRoomCreate; };
	};
}