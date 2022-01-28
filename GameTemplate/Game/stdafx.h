#pragma once
#define _CRT_SECURE_NO_WARNINGS

/// <summary>
/// エンジン
/// </summary>
#include "MiniEngine.h"		//ミニエンジン
#include "ExEngine.h"		//Exエンジン

#include "Utilities/Noncopyable.h"				// コピー禁止ポリシーを提供するためのクラス。
#include "Sound/tkSoundSource.h"				//サウンド
#include "Graphics/SkinModelRender.h"			//モデル
#include "Graphics/SpriteRender.h"				//画像
#include "Graphics/SpriteRenderSub.h"			//画像
#include "Graphics/FontRender.h"				//文字
#include "Graphics/FontRenderSub.h"				//文字
#include "Graphics/Light.h"						//ライト
#include "../../ExEngine/effect/Effect.h"		//エフェクト

namespace nsStdafx
{
	/// <summary>
	/// 優先度
	/// 0が最も優先度が低く、
	/// 数が増えるにつれて優先度は高くなる。
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
	/// 色
	/// </summary>
	const Vector3 WHITE = { 1.0f, 1.0f ,1.0f };	//白色
	const Vector3 GRAY = { 0.7f,0.7f,0.7f };	//灰色
	const Vector3 BLACK = { 0.0f,0.0f,0.0f };	//黒色
	const Vector3 RED = { 1.0f, 0.0f ,0.0f };	//赤色
	const Vector3 GREEN = { 0.0f, 1.0f ,0.0f };	//緑色
	const Vector3 BLUE = { 0.0f, 0.0f ,1.0f };	//青色
	const Vector3 YELLOW = { 1.0f, 1.0f ,0.0f };//黄色
	/// <summary>
	///	クラス(オブジェクト)の名前
	/// </summary>
	const char* const PAUSE_NAME		= { "pause" };
	const char* const GAMEUPDATESTOP_NAME = { "gameovergameclear" };
	const char* const LOADSCENE_NAME	= { "loadscene" };
	const char* const ONLINE_NAME		= { "online" };
	const char* const TITLESCENE_NAME	= { "titlescene" };
	const char* const GAMESCENE_NAME	= { "gamescene" };
	const char* const ACCOMPANYAI_NAME	= { "accompanyai" };
	const char* const GAMELIMITTIME = { "gamelimittime" };
	//プレイヤー関係
	const char* const PLAYER_NAME[2]	= { "player1","player2" };
	const char* const PLAYER_CAMERA_NAME = { "playercamera" };
	const char* const PLAYER_ROTATION_NAME[2]	= { "playerrotation1","playertotation2" };
	const char* const PLAYER_ANIMATION_NAME[2]	= { "playeranimation1","playeranimation2" };
	const char* const PLAYER_CHARACON_NAME[2]	= { "playercharacon1","playercharacon2" };
	const char* const PLAYER_SOUND_NAME[2]	= { "playersound1","playersound2" };
	const char* const WEAPON_NAME[2]	= { "player1weapon","player2weapon" };
	const char* const PLAYER_STATUS_NAME[2]	= { "player1status","player2status" };
	//グラフィック関係
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
	///	定数
	/// </summary>
	const float GRAVITY = 0.3f;		//重力
	const int SOUND_CAN_HEARD_RANGE = 800.0f;	//音を聞くことができる範囲
}

/// <summary>
///	メモ
/// </summary>

//出力にデバック表示する
//OutputDebugStringA("〇〇\n");