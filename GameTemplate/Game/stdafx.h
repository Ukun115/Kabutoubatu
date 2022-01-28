#pragma once
#define _CRT_SECURE_NO_WARNINGS

/// <summary>
/// �G���W��
/// </summary>
#include "MiniEngine.h"		//�~�j�G���W��
#include "ExEngine.h"		//Ex�G���W��

#include "Utilities/Noncopyable.h"				// �R�s�[�֎~�|���V�[��񋟂��邽�߂̃N���X�B
#include "Sound/tkSoundSource.h"				//�T�E���h
#include "Graphics/SkinModelRender.h"			//���f��
#include "Graphics/SpriteRender.h"				//�摜
#include "Graphics/SpriteRenderSub.h"			//�摜
#include "Graphics/FontRender.h"				//����
#include "Graphics/FontRenderSub.h"				//����
#include "Graphics/Light.h"						//���C�g
#include "../../ExEngine/effect/Effect.h"		//�G�t�F�N�g

namespace nsStdafx
{
	/// <summary>
	/// �D��x
	/// 0���ł��D��x���Ⴍ�A
	/// ����������ɂ�ėD��x�͍����Ȃ�B
	/// </summary>
	const int PRIORITY_0 = 0;
	const int PRIORITY_1 = 1;
	const int PRIORITY_2 = 2;
	const int PRIORITY_3 = 3;
	const int PRIORITY_4 = 4;
	const int PRIORITY_5 = 5;
	const int PRIORITY_6 = 6;
	const int PRIORITY_7 = 7;
	const int PRIORITY_8 = 8;
	const int PRIORITY_9 = 9;
	const int PRIORITY_10 = 10;
	const int PRIORITY_11 = 11;
	const int PRIORITY_12 = 12;
	/// <summary>
	/// �F
	/// </summary>
	const Vector3 WHITE = { 1.0f, 1.0f ,1.0f };	//���F
	const Vector3 GRAY = { 0.7f,0.7f,0.7f };	//�D�F
	const Vector3 BLACK = { 0.0f,0.0f,0.0f };	//���F
	const Vector3 RED = { 1.0f, 0.0f ,0.0f };	//�ԐF
	const Vector3 GREEN = { 0.0f, 1.0f ,0.0f };	//�ΐF
	const Vector3 BLUE = { 0.0f, 0.0f ,1.0f };	//�F
	const Vector3 YELLOW = { 1.0f, 1.0f ,0.0f };//���F
	/// <summary>
	///	�N���X(�I�u�W�F�N�g)�̖��O
	/// </summary>
	const char* const PAUSE_NAME		= { "pause" };
	const char* const GAMEUPDATESTOP_NAME = { "gameovergameclear" };
	const char* const LOADSCENE_NAME	= { "loadscene" };
	const char* const ONLINE_NAME		= { "online" };
	const char* const TITLESCENE_NAME	= { "titlescene" };
	const char* const GAMESCENE_NAME	= { "gamescene" };
	const char* const ACCOMPANYAI_NAME	= { "accompanyai" };
	const char* const GAMELIMITTIME = { "gamelimittime" };
	//�v���C���[�֌W
	const char* const PLAYER_NAME[2]	= { "player1","player2" };
	const char* const PLAYER_CAMERA_NAME = { "playercamera" };
	const char* const PLAYER_ROTATION_NAME[2]	= { "playerrotation1","playertotation2" };
	const char* const PLAYER_ANIMATION_NAME[2]	= { "playeranimation1","playeranimation2" };
	const char* const PLAYER_CHARACON_NAME[2]	= { "playercharacon1","playercharacon2" };
	const char* const PLAYER_SOUND_NAME[2]	= { "playersound1","playersound2" };
	const char* const WEAPON_NAME[2]	= { "player1weapon","player2weapon" };
	const char* const PLAYER_STATUS_NAME[2]	= { "player1status","player2status" };
	//�O���t�B�b�N�֌W
	const char* const LIGHT_NAME		= { "light" };
	const char* const DEPTHSHADOW_NAME	= { "depthshadow" };
	const char* const DOF_NAME			= { "dof" };
	const char* const RENDERINGENGINE_NAME		= { "zprepass" };
	const char* const SILHOUETTE_NAME	= { "silhouette" };
	const char* const MINIMAP_NAME		= { "minimap" };
	const char* const SKYCUBE_NAME		= { "skycube" };
	const char* const ARROW_NAME = { "arrow" };
	const char* const NEEDLE_SLIME_NAME = { "needleslime" };
	const char* const GAMESCREENUI_NAME = { "gamescreenui" };

	/// <summary>
	///	�萔
	/// </summary>
	const float GRAVITY = 0.3f;		//�d��
	const int SOUND_CAN_HEARD_RANGE = 800.0f;	//���𕷂����Ƃ��ł���͈�
}

/// <summary>
///	����
/// </summary>

//�o�͂Ƀf�o�b�N�\������
//OutputDebugStringA("�Z�Z\n");