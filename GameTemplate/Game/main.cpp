#include "stdafx.h"
#include "system/system.h"
#include "../Game/Title/TitleScene.h"
#include "StopWatch.h"
#include "../Game/Graphics/RenderingEngine.h"
#include "../Game/InGame/Camera/PlayerCamera.h"
#include "StopWatch.h"
#include "../Game/InGame/Pause.h"
#include "Utilities/GameUpdateStop.h"
#include "../Game/InGame/Online/Online.h"
#include "ResourceBankManager.h"

/// <summary>
/// �W�����o�̓R���\�[����������
/// </summary>
void InitStandardIOConsole();

/// <summary>
/// ���f�����O���[�h
/// </summary>
void PreLoad();

//////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
//////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//FPS�l�\����OnOff�͂����Őݒ�B
	bool m_fpsValueOn = true;

	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("�ӂ����!! ���ԂƂ��΂�"));


	// �W�����o�̓R���\�[���̏�����
	//�R�}���h�v�����v�g���N��
	if (m_fpsValueOn)
	{
		InitStandardIOConsole();
	}

	//////////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////////

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	//�t�B�W�N�X���[���h�̃C���X�^���X���쐬����B
	PhysicsWorld::CreateInstance();
	//���\�[�X�}�l�[�W���̃C���X�^���X���쐬
	ResourceBankManager::CreateInstance();
	//�v�����[�h
	PreLoad();
	//�G�t�F�N�g�G���W���̃C���X�^���X���쐬����B
	EffectEngine::CreateInstance();
	//�T�E���h�G���W���̃C���X�^���X���쐬����B
	SoundEngine::CreateInstance();
	SoundEngine::GetInstance()->Init();

	//�I�����C���ʐM�G���W��
	nsKabutoubatu::Online* online = NewGO<nsKabutoubatu::Online>(nsStdafx::PRIORITY_0, nsStdafx::ONLINE_NAME);

	//���C�g
	nsKabutoubatu::Light* light = NewGO<nsKabutoubatu::Light>(nsStdafx::PRIORITY_0,nsStdafx::LIGHT_NAME);
	//�f�B���N�V�������C�g���Z�b�g
	light->SetDirectionLightData();
	//�������C�g���Z�b�g
	light->SetHemiSphereLightData();

	//�����_�����O�G���W��
	nsKabutoubatu::RenderingEngine* renderingEngine = NewGO<nsKabutoubatu::RenderingEngine>(nsStdafx::PRIORITY_0, nsStdafx::RENDERINGENGINE_NAME);
	//ZPrepass�p�̃����_�����O�^�[�Q�b�g��������
	renderingEngine->Init();

	//�|�[�Y
	nsKabutoubatu::Pause* pause = NewGO<nsKabutoubatu::Pause>(nsStdafx::PRIORITY_0,nsStdafx::PAUSE_NAME);

	//�Q�[�����Ԃ̐i�s���X�g�b�v������N���X
	nsKabutoubatu::GameUpdateStop* gameUpdateStop = NewGO<nsKabutoubatu::GameUpdateStop>(nsStdafx::PRIORITY_0,nsStdafx::GAMEUPDATESTOP_NAME);
	enum enMoveTarget
	{
		enUI,
		enCameraMove
	};

	//�X�g�b�v�E�H�b�`�𐶐�����
	StopWatch stopWatch;

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//�^�C�g���V�[��
	NewGO<nsKabutoubatu::TitleScene>(nsStdafx::PRIORITY_0,nsStdafx::TITLESCENE_NAME);

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�X�g�b�v�E�H�b�`�̌v���J�n
		stopWatch.Start();

		//�����_�����O�J�n�B
		g_engine->BeginFrame();

		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////

		//�|�[�Y���łȂ��Ƃ��͒ʏ�X�V���������s
		if(!pause->GetIsPause() && !gameUpdateStop->GetIsStop())
		{
			//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�B
			GameObjectManager::GetInstance()->ExecuteUpdate();
		}
		//�|�[�Y���̎��̓|�[�Y��p�̍X�V���������s
		else if(pause->GetIsPause())
		{
			//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃|�[�Y�X�V�B
			GameObjectManager::GetInstance()->ExecutePauseUpdate();
		}
		//�Q�[���̐i�s���~�߂�UI�̓����݂̂𓮂����B
		else if (gameUpdateStop->GetIsStop())
		{
			switch (gameUpdateStop->GetMoveTarget())
			{
			//UI�݂̂𓮂���
			case enUI:
				//�Q�[���I�u�W�F�N�g�}�l�[�W���[��UI���o�X�V�B
				GameObjectManager::GetInstance()->ExecuteUiUpdate();

				break;

			//�J�����ړ��݂̂𓮂���
			case enCameraMove:
				//�J�����A�j���[�V�������������鎞�͍X�V�֐��𑝂₷�B

				break;
			}
		}

		//�������[���h�̍X�V�B
		PhysicsWorld::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());
		//�G�t�F�N�g�G���W���̍X�V�B
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

		//�����_�����O�G���W�������s
		renderingEngine->Execute(renderContext);

		//////////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////////
		g_engine->EndFrame();

		//�X�s�����b�N���s���B
		int restTime = 0;
		do {
			stopWatch.Stop();
			restTime = 16 - (int)stopWatch.GetElapsedMillisecond();
		} while (restTime > 0);

		//�X�g�b�v�E�H�b�`�̌v���I��
		stopWatch.Stop();

		//FPS�l���f�o�b�N�\��
		if (m_fpsValueOn)
		{
			printf("fps = %0.2f\n\n", 1.0f / stopWatch.GetElapsed());
		}

		//�f���^�^�C�����X�g�b�v�E�H�b�`�̌v�����Ԃ���A�v�Z����
		GameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());
	}::FreeConsole();

	return 0;
}

/// <summary>
/// �W�����o�̓R���\�[����������
/// </summary>
void InitStandardIOConsole()
{
	::AllocConsole();               // �R�}���h�v�����v�g���\�������
	freopen("CON", "w", stdout);    // �W���o�͂̊��蓖��

	auto fhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rc;

	rc.Top = 0;
	rc.Left = 0;
	rc.Bottom = 30;
	rc.Right = 30;
	::SetConsoleWindowInfo(fhandle, TRUE, &rc);
}

/// <summary>
/// ���f�����O���[�h
/// </summary>
void PreLoad()
{
	//�v���C���[
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/Player1.tkm");	//1P
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/Player2.tkm");	//2P
	//�G
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/enemy_mole.tkm");		//���O��
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/enemy_needleSlime.tkm");	//�g�Q�X���C��
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/enemy_slime.tkm");		//�X���C��
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/enemy_firstBoss.tkm");	//�ŏ��̃{�X
	//�X�e�[�W
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/sea.tkm");							//�C
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/FirstStage_map1_floor.tkm");			//��
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/FirstStage_map1_hitAcce.tkm");		//�����蔻��̂��鑕���i
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/FirstStage_map1_dontHitAcce.tkm");	//�����蔻��̂Ȃ������i
	//����
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/House.tkm");				//�h��
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/HouseRed.tkm");			//�A�C�e���V���b�v
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/FirstBossHouse.tkm");	//�ŏ��̃{�X�̌���
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/SalesPerson.tkm");		//�X��
	//�����̃h�A
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/House_Door.tkm");			//�h���̃h�A
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/HouseRed_Door.tkm");			//�A�C�e���V���b�v�̃h�A
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/FirstBossHouse_Door.tkm");	//�ŏ��̃{�X�̌����̃h�A
	//�h�A�̈È�
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/DarknessEntrance.tkm");					//�h���ƃA�C�e���̃h�A�̈È�
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/DarknessEntrance_FirstBossHouse.tkm");	//�ŏ��̃{�X�̌����̃h�A�̈È�
}