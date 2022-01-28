/// <summary>
/// 武器クラス
/// </summary>
#include "stdafx.h"
#include "Weapon.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"

namespace nsKabutoubatu
{
	namespace nsWeapon
	{
		const char WEAPON_FILEPATH[4][256] = { "WoodStick","Sword","LargeSword","Bow" };
		const int WEAPON_ATTACK_POWER[4] = {1,2,4,2};
		const int ATTACK_JUDGE_TIME[4] = {30,30,70,99999};
	}

	bool Weapon:: Start()
	{
		//プレイヤーのインスタンスを検索
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_playerNum]);
		m_playerStatus = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[m_playerNum]);

		//武器モデルの初期化
		Init(m_weaponNum);

		//右手ボーンを検索
		m_rightHandBoneId = m_player->GetSkeleton()->FindBoneID(L"Character1_RightHandMiddle1");
		// 検索したボーンIDを使って、ボーンを取得する。
		m_rightHandBone = m_player->GetSkeleton()->GetBone(m_rightHandBoneId);

		//左手ボーンを検索
		m_leftHandBoneId = m_player->GetSkeleton()->FindBoneID(L"Character1_LeftHandMiddle1");
		// 検索したボーンIDを使って、ボーンを取得する。
		m_leftHandBone = m_player->GetSkeleton()->GetBone(m_leftHandBoneId);

		return true;
	}

	Weapon::~Weapon()
	{
		//モデルを削除
		DeleteGO(m_model);
	}

	void Weapon::Update()
	{
		//ボーンのワールド座標・回転・拡大率を更新
		for (int boneNum = 0; boneNum < m_player->GetSkeleton()->GetNumBones(); boneNum++)
		{
			m_player->GetSkeleton()->GetBone(boneNum)->CalcWorldTRS(m_pos, m_rot, m_sca);
		}

		if (m_weaponNum == enBow)
		{
			//取得したボーンのワールド座標を取得
			m_pos = m_leftHandBone->GetPosition();
			//取得したボーンの回転を取得
			m_rot = m_leftHandBone->GetRotation();
		}
		else
		{
			//取得したボーンのワールド座標を取得
			m_pos = m_rightHandBone->GetPosition();
			//取得したボーンの回転を取得
			m_rot = m_rightHandBone->GetRotation();
		}

		//位置更新
		m_model->SetPosition(m_pos);
		//回転更新
		m_model->SetRotation(m_rot);

		m_playerStatus->ChangeHavingWeapon(m_weaponNum);

		//武器ごとの攻撃ヒットタイミングを設定
		m_attackJudgeTimer = nsWeapon::ATTACK_JUDGE_TIME[m_weaponNum];

		//火の玉中は武器のモデルを隠す
		if (m_player->GetFireBallActive())
		{
			//TODO:モデルを見えないようにする。
			m_model->SetFireBall(true);
		}
		else
		{
			//TODO:モデルを見えるようにする。
			m_model->SetFireBall(false);
		}
	}

	void Weapon::UiUpdate()
	{
		//ボーンのワールド座標・回転・拡大率を更新
		for (int boneNum = 0; boneNum < m_player->GetSkeleton()->GetNumBones(); boneNum++)
		{
			m_player->GetSkeleton()->GetBone(boneNum)->CalcWorldTRS(m_pos, m_rot, m_sca);
		}

		if (m_weaponNum == enBow)
		{
			//取得したボーンのワールド座標を取得
			m_pos = m_leftHandBone->GetPosition();
			//取得したボーンの回転を取得
			m_rot = m_leftHandBone->GetRotation();
		}
		else
		{
			//取得したボーンのワールド座標を取得
			m_pos = m_rightHandBone->GetPosition();
			//取得したボーンの回転を取得
			m_rot = m_rightHandBone->GetRotation();
		}

		//位置更新
		m_model->SetPosition(m_pos);
		//回転更新
		m_model->SetRotation(m_rot);

		m_playerStatus->ChangeHavingWeapon(m_weaponNum);

		//武器ごとの攻撃ヒットタイミングを設定
		m_attackJudgeTimer = nsWeapon::ATTACK_JUDGE_TIME[m_weaponNum];
	}

	void Weapon::Init(const int weaponNum)
	{
		//前のモデルを削除
		if (m_model != nullptr)
		{
			DeleteGO(m_model);
		}

		m_weaponNum = weaponNum;

		//モデルを初期化
		m_model = NewGO<SkinModelRender>();
		m_model->SetShadowCaster(true);	//影を落とす
		m_model->SetOutline(false);		//輪郭線をつけない
		m_model->SetSilhouette(true);		//シルエットをつける
		m_model->SetPlayerMode(m_playerNum);	//プレイヤー１か２かを渡す

		//武器ファイルパス
		m_weaponModelName = nsWeapon::WEAPON_FILEPATH[m_weaponNum];
		//攻撃力
		m_attackDamage = nsWeapon::WEAPON_ATTACK_POWER[m_weaponNum];

		m_model->Init(m_weaponModelName);
	}
}