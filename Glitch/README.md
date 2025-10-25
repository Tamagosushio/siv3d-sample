# Glitch Render Texture
![Glitch](./Glitch.gif)

## Parameters
ハイパーパラメーターは以下の5つです。  
これらの値を調整することで、グリッチエフェクトを調整することができます。  
- `double ChromaticAberrationShift`  
  色収差で描画をずらす距離の最大値。  
  実際にずらす距離はグリッチ強度との乗算。
- `double ScanProbability`  
  スキャンラインを引き起こす確率の最大値。  
  実際の確率はグリッチ強度との乗算。
- `double ScanShift`  
  スキャンラインで描画をずらす距離の最大値。  
  実際の距離はグリッチ強度と[-1.0, 1.0]区間乱数の乗算。
- `int32 NumScanLinesMin`  
  スキャンライン本数の最小値。
- `int32 NumScanLinesMax`  
  スキャンライン本数の最大値。
