/// <summary>
/// ゲームの制限時間クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{

	/// <summary>
	/// 前方宣言
	/// </summary>
	class Fade;	//フェード

	class GameLimitTime : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//時計の針
		enum enTimeNeedle
		{
			enTimeNeedleLong,	//長針
			enTimeNeedleShort,	//短針
			enTimeNeedleNum		//時計の針の総数
		};

		//制限時間
		enum enLimitTime
		{
			enSeconds,				//秒
			enMinutes,				//分
			enSecondsMinutesNum		//制限時間の総数
		};

		SpriteRender* m_timeBackSprite = nullptr;
		SpriteRender* m_timeNeedleSprite[enTimeNeedleNum] = { nullptr };
		Fade* m_fade = nullptr;
		//制限時間フォント
		FontRender* m_limitTimeFont[enSecondsMinutesNum] = { nullptr };
		FontRender* m_timeColonFont = nullptr;

		Quaternion m_timeNeedleRot[enTimeNeedleNum];
		double m_timeNeedleRotAngle[enTimeNeedleNum] = { 0.0f };
		wchar_t m_limitTimeText[256];
		int m_measureTime = 0;
		int m_nowLimitTime[enSecondsMinutesNum] = { 0 };
		int m_seconds = 60;
		/// <summary>
		/// フラグ
		/// </summary>
		bool m_minuteChangeFlg = false;
		bool m_canMeasure = true;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start() override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~GameLimitTime();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update() override final;

	public:
		//制限時間が無くなったかどうかを取得するメソッド
		bool TimeOver();

		void SetCanMeasure(const bool canMeasure);
	};
}