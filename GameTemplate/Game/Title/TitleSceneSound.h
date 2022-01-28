/// <summary>
/// タイトルシーンのサウンドクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class TitleScene;	//タイトルシーン

	class TitleSceneSound : public IGameObject
	{
	private:
		SoundSource* m_titleSceneSound = nullptr;
		SoundSource* m_selectSound = nullptr;
		SoundSource* m_decideSound = nullptr;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~TitleSceneSound();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

	public:
		//選択音再生メソッド
		void PlaySelectSound();

		//決定音再生メソッド
		void PlayDecideSound();

		/// <summary>
		/// 決定音が鳴っているかどうかを取得
		/// </summary>
		/// <returns>決定音が鳴っているかどうか</returns>
		bool GetIsPlayingDecideSound()const { return m_decideSound->IsPlaying(); }
	};
}