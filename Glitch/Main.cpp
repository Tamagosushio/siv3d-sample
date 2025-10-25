#include <Siv3D.hpp>

// 描画された最大のアルファ成分を保持するブレンドステートを作成する
BlendState MakeBlendState()
{
	BlendState blendState = BlendState::Default2D;
	blendState.srcAlpha = Blend::SrcAlpha;
	blendState.dstAlpha = Blend::DestAlpha;
	blendState.opAlpha = BlendOp::Max;
	return blendState;
}

// シーンの描画内容
void Draw(const Font& font, const Texture& texture)
{
	Rect{ 100, 400, 100 }.rotated(Scene::Time() * 60_deg).draw(Palette::White);
	Circle{ 450 + Sin(Scene::Time() * 3.0) * 100, 400, 50 }.draw(Palette::White);
	Shape2D::Star(50, Vec2{ 700, 450 + Sin(Scene::Time() * 5.0) * 50 }).draw(Palette::Yellow);
	font(U"Glitch Effect Sample").drawAt(Scene::CenterF());
	texture.rotated(Scene::Time() * 30_deg).drawAt(100, 100);
}

void Main()
{
	// グリッチの強さ (0.0: なし 〜 1.0: 最大)
	double glitchIntensity = 0.0;

	// アセットの準備
	const Font font{ 75 };
	const Texture emoji{ U"🐈"_emoji };

	// レンダーテクスチャの初期化色
	ColorF textureInitColorF{ 0.0, 0.0 };
	// グリッチをかけたいシーンを描画するレンダーテクスチャ
	MSRenderTexture sceneTexture{ Scene::Size(), textureInitColorF };

	// 色収差で描画をずらす距離の最大値
	constexpr double ChromaticAberrationShift = 10.0;
	// ラインごとにずらす確率とずらす距離の最大値、ライン数の範囲
	constexpr double ScanProbability = 0.3;
	constexpr double ScanShift = 30.0;
	constexpr int32 NumScanLinesMin = 20;
	constexpr int32 NumScanLinesMax = 40;

	bool isChromaticAberration = true;
	bool isScanLines = true;

	while (System::Update())
	{
		// グリッチ強度の調整用スライダー
		SimpleGUI::Slider(U"強度: {:.2f}"_fmt(glitchIntensity), glitchIntensity, 0.0, 1.0, Vec2{500, 50}, 100, 200);
		// 色収差の選択
		SimpleGUI::CheckBox(isChromaticAberration, U"色収差", Vec2{ 500, 100 });
		// スキャンラインの選択
		SimpleGUI::CheckBox(isScanLines, U"スキャンライン", Vec2{ 500, 150 });

		// レンダーテクスチャへの描画
		{
			const ScopedRenderTarget2D target{ sceneTexture.clear(textureInitColorF)};
			const ScopedRenderStates2D blend{ MakeBlendState() };
			Draw(font, emoji);
		}

		// グリッチエフェクトの描画
		Graphics2D::Flush();
		sceneTexture.resolve();
		if (glitchIntensity > 0.0)
		{
			// 色収差
			if (isChromaticAberration)
			{
				const double shift = glitchIntensity * ChromaticAberrationShift;
				sceneTexture.draw(RandomVec2(-shift, shift), ColorF(1.0, 0.0, 0.0, 1.0)); // Red
				sceneTexture.draw(RandomVec2(-shift, shift), ColorF(0.0, 1.0, 0.0, 1.0)); // Green
				sceneTexture.draw(RandomVec2(-shift, shift), ColorF(0.0, 0.0, 1.0, 1.0)); // Blue
			}

			// スキャンライン
			if (isScanLines)
			{
				const int numLines = Random(NumScanLinesMin, NumScanLinesMax);
				const double lineHeight = Scene::Height() / static_cast<double>(numLines);
				for (int i : step(numLines))
				{
					// 描画元の四角形
					const RectF sourceRect{ 0, i * lineHeight, Scene::Width(), lineHeight };
					// 強さに応じた確率で横にずらす
					double lineShift = 0.0;
					if (RandomBool(glitchIntensity * ScanProbability))
					{
						lineShift = Random(-1.0, 1.0) * glitchIntensity * ScanShift;
					}
					sceneTexture(sourceRect).draw(sourceRect.pos.movedBy(lineShift, 0));
				}
			}
			else
			{
				sceneTexture.draw();
			}
		}
		else
		{
			// グリッチ強度が0.0なら通常通り描画
			sceneTexture.draw();
		}
	}
}
