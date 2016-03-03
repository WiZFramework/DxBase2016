
/******************************************************************************
MathVectorEX.h

ベクトルのスタティック計算
XNAMATH のラッピングスタティッククラス群

Copyright (c) 2014 WiZ Tamura Hiroki,Yamanoi Yasushi.
DirectXTK and DirectXTex are released under Microsoft Public License (Ms-PL).
********************************************************************************/

#pragma once

#include "stdafx.h"

namespace basedx11{

	void Vector2::Transform(const Matrix4X4& m) {
		*this = XMVector2Transform(XMVECTOR(*this), XMMATRIX(m));

	}

	/**************************************************************************
	namespace Vector2EX;
	用途: 2次元XMVECTORの 関数構造体
	****************************************************************************/
	namespace Vector2EX {
		//作成
		/**************************************************************************
		  inline Vector2 WStr2ToVec2(
		wstring& x_str,
		wstring& y_str
		);
		用途: ユーティリティ関数
		戻り値: wstringからベクトルを作成する
		***************************************************************************/
		  inline Vector2 WStr2ToVec2(wstring& x_str, wstring& y_str) {
			return
				Vector2(
					(float)_wtof(x_str.c_str()),
					(float)_wtof(y_str.c_str())
					);
		}
		//比較
		/**************************************************************************
		  inline bool Equal(
		const Vector2& Vec,		//ベクトル
		const Vector2& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しいかどうかを検証する
		戻り値: 等しければtrue
		***************************************************************************/
		  inline bool Equal(const Vector2& Vec, const Vector2& other) {
			return XMVector2Equal(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  bool EqualInt(
		const Vector2& Vec,		//ベクトル
		const Vector2& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しいかどうかを検証する。
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しければtrue
		***************************************************************************/
		  inline bool EqualInt(const Vector2& Vec, const Vector2& other) {
			return XMVector2EqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline bool Greater(
		const Vector2& Vec,		//ベクトル
		const Vector2& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルより大きいかどうかを検証する。
		戻り値: 大きければtrue
		***************************************************************************/
		  inline bool Greater(const Vector2& Vec, const Vector2& other) {
			return XMVector2Greater(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline bool GreaterOrEqual(
		const Vector2& Vec,		//ベクトル
		const Vector2& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトル以上の大きさかどうかを検証する。
		戻り値: 以上の大きさならばtrue
		***************************************************************************/
		  inline bool GreaterOrEqual(const Vector2& Vec, const Vector2& other) {
			return XMVector2GreaterOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline bool IsInfinite(
		const Vector2& Vec		//ベクトル
		);
		用途: ベクトルが値のいずれかが正か負の無限大かどうかを検証する。
		戻り値: 値のいずれかが正か負の無限大ならばtrue
		***************************************************************************/
		  inline bool IsInfinite(const Vector2& Vec) {
			return XMVector2IsInfinite(XMVECTOR(Vec));
		}
		/**************************************************************************
		  bool IsNaN(
		const Vector2& Vec		//ベクトル
		);
		用途: ベクトルの値のいずれかが非数（無効値）かどうかを検証する。
		戻り値: 値のいずれかが非数（無効値）ならばtrue
		***************************************************************************/
		  inline bool IsNaN(const Vector2& Vec) {
			return XMVector2IsNaN(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline bool Less(
		const Vector2& Vec,		//ベクトル
		const Vector2& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルより小さいかどうかを検証する。
		戻り値: 小さければtrue
		***************************************************************************/
		  inline bool Less(const Vector2& Vec, const Vector2& other) {
			return XMVector2Less(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline bool LessOrEqual(
		const Vector2& Vec,		//ベクトル
		const Vector2& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトル以下の大きさかどうかを検証する。
		戻り値: 以下の大きさならばtrue
		***************************************************************************/
		  inline bool LessOrEqual(const Vector2& Vec, const Vector2& other) {
			return XMVector2LessOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline bool NearEqual(
		const Vector2& Vec,		//ベクトル
		const Vector2& other,	//比較するベクトル
		float Epsilon			//比較する最大許容差
		);
		用途: ベクトルがもう一つのベクトルが近い大きさかどうかを検証する。
		戻り値: 近い大きさならばtrue
		***************************************************************************/
		  inline bool NearEqual(const Vector2& Vec, const Vector2& other, float Epsilon) {
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector2NearEqual(XMVECTOR(Vec), XMVECTOR(other), VecEp);
		}
		/**************************************************************************
		  inline bool NotEqual(
		const Vector2& Vec,		//ベクトル
		const Vector2& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しくないかどうかを検証する
		戻り値: 等しくなければtrue
		***************************************************************************/
		  inline bool NotEqual(const Vector2& Vec, const Vector2& other) {
			return XMVector2NotEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline bool NotEqualInt(
		const Vector2& Vec,		//ベクトル
		const Vector2& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しくないかどうかを検証する
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しくなければtrue
		***************************************************************************/
		  inline bool NotEqualInt(const Vector2& Vec, const Vector2& other) {
			return XMVector2NotEqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		//ジオメトリ
		/**************************************************************************
		  inline float AngleBetweenNormals(
		const Vector2& Vec1,	//ベクトル1
		const Vector2& Vec2		//ベクトル2
		);
		用途: 正規化したベクトル同士のラジアン角度を計算する
		戻り値: ラジアン角
		＊正規化はしないので、引数ははあらかじめ正規化しておく
		＊ベクトルのラジアン角計算では、正規化しない分速い
		***************************************************************************/
		  inline float AngleBetweenNormals(const Vector2& Vec1, const Vector2& Vec2) {
			return ((Vector2)XMVector2AngleBetweenNormals(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline float AngleBetweenNormalsEst(
		const Vector2& Vec1,	//ベクトル1
		const Vector2& Vec2		//ベクトル2
		);
		用途: 正規化したベクトル同士のラジアン角度を計算予測する
		戻り値: ラジアン角度を表すベクトル
		＊正規化はしないので、引数ははあらかじめ正規化しておく
		＊ベクトルのラジアン角計算では、正規化しない分速い
		***************************************************************************/
		  inline float AngleBetweenNormalsEst(const Vector2& Vec1, const Vector2& Vec2) {
			return ((Vector2)XMVector2AngleBetweenNormalsEst(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline float AngleBetweenVectors(
		const Vector2& Vec1,	//ベクトル1
		const Vector2& Vec2		//ベクトル2
		);
		用途: 二つの ベクトル間のラジアン角を計算する。
		戻り値: ラジアン角度
		***************************************************************************/
		  inline float AngleBetweenVectors(const Vector2& Vec1, const Vector2& Vec2) {
			return ((Vector2)XMVector2AngleBetweenVectors(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline Vector2 ClampLength(
		const Vector2& Vec,	//ベクトル
		float LengthMin,	//長さの最小値
		float LengthMax		//長さの最大値
		);
		用途: ベクトルの長さを指定した大きさにクランプする
		戻り値: クランプした結果ベクトル
		***************************************************************************/
		  inline Vector2 ClampLength(const Vector2& Vec, float LengthMin, float LengthMax) {
			return (Vector2)XMVector2ClampLength(XMVECTOR(Vec), LengthMin, LengthMax);
		}
		/**************************************************************************
		  inline Vector2 ClampLengthV(
		const Vector2& Vec,				//ベクトル
		const Vector2& LengthMinV,		//長さの最小値(ベクトル)
		const Vector2& LengthMaxV		//長さの最大値(ベクトル)
		);
		用途: ベクトルを指定した大きさにクランプする
		戻り値: クランプしたベクトル
		***************************************************************************/
		  inline Vector2 ClampLengthV(const Vector2& Vec, const Vector2& LengthMinV, const Vector2& LengthMaxV) {
			return (Vector2)XMVector2ClampLengthV(XMVECTOR(Vec), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		/**************************************************************************
		  inline float Cross(
		const Vector2& Vec1,	//ベクトル１
		const Vector2& Vec2		//ベクトル２
		);
		用途: 外積を計算して返す
		戻り値: なし
		***************************************************************************/
		  inline float Cross(const Vector2& Vec1, const Vector2& Vec2) {
			return ((Vector2)XMVector2Cross(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline float Dot(
		const Vector2& Vec1,	//ベクトル１
		const Vector2& Vec2		//ベクトル２
		);
		用途: ベクトル同士の内積を計算して返す
		戻り値: 内積
		***************************************************************************/
		inline float Dot(const Vector2& Vec1, const Vector2& Vec2) {
			return ((Vector2)XMVector2Dot(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline bool InBounds(
		const Vector2& Vec1,	//比較元ベクトル
		const Vector2& Vec2		//比較するベクトル
		);
		用途: すべての要素が、設定された境界ベクトル内にある場合はtrueを返す
		戻り値: 境界内にある場合はtrue
		***************************************************************************/
		  inline bool InBounds(const Vector2& Vec1, const Vector2& Vec2) {
			return XMVector2InBounds(XMVECTOR(Vec1), XMVECTOR(Vec2));
		}
		/**************************************************************************
		  inline float Length(
		const Vector2& Vec,	//ベクトル
		);
		用途: ベクトルの長さを返す
		戻り値: ベクトルの長さ
		***************************************************************************/
		  inline float Length(const Vector2& Vec) {
			return ((Vector2)XMVector2Length(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline float LengthEst(
		const Vector2& Vec	//ベクトル
		);
		用途: ベクトルの長さを予想して返す
		戻り値: ベクトルの長さの予想
		***************************************************************************/
		  inline float LengthEst(const Vector2& Vec) {
			return ((Vector2)XMVector2LengthEst(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline float LengthSq(
		const Vector2& Vec	//ベクトル
		);
		用途: ベクトルの長さの2乗を返す
		戻り値: ベクトルの長さの2乗
		***************************************************************************/
		  inline float LengthSq(const Vector2& Vec) {
			return ((Vector2)XMVector2LengthSq(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline Vector2 Normalize(
		const Vector2& Vec	//ベクトル
		);
		用途: ベクトルを正規化する
		戻り値: 正規化したベクトル
		***************************************************************************/
		  inline Vector2 Normalize(const Vector2& Vec) {
			return (Vector2)XMVector2Normalize(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline Vector2 NormalizeEst(
		const Vector2& Vec	//ベクトル
		);
		用途: 正規化の予想値を返す
		戻り値: 正規化の予想値
		***************************************************************************/
		  inline Vector2 NormalizeEst(const Vector2& Vec) {
			return (Vector2)XMVector2NormalizeEst(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline Vector2 Orthogonal(
		const Vector2& Vec	//ベクトル
		);
		用途: 垂直なベクトルを返す
		戻り値: 垂直なベクトル
		***************************************************************************/
		  inline Vector2 Orthogonal(const Vector2& Vec) {
			return (Vector2)XMVector2Orthogonal(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline float ReciprocalLength(
		const Vector2& Vec	//ベクトル
		);
		用途: ベクトルの長さの逆数を返す
		戻り値: ベクトルの長さの逆数
		***************************************************************************/
		  inline float ReciprocalLength(const Vector2& Vec) {
			return ((Vector2)XMVector2ReciprocalLength(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  float ReciprocalLengthEst(
		const Vector2& Vec	//ベクトル
		);
		用途: ベクトルの長さの逆数の予測値を返す
		戻り値: ベクトルの長さの逆数の予測値
		***************************************************************************/
		  inline  float ReciprocalLengthEst(const Vector2& Vec) {
			return ((Vector2)XMVector2ReciprocalLengthEst(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  Vector2 Reflect(
		const Vector2& Vec,	//ベクトル
		const Vector2& Normal	//法線ベクトル
		);
		用途: ベクトルを法線ベクトルによって反射させたベクトル(入射ベクトル)を返す
		戻り値: 反射させたベクトル
		***************************************************************************/
		  inline  Vector2 Reflect(const Vector2& Vec, const Vector2& Normal) {
			return (Vector2)XMVector2Reflect(XMVECTOR(Vec), XMVECTOR(Normal));
		}
		/**************************************************************************
		  inline  Vector2 Refract(
		const Vector2& Vec,		//ベクトル
		const Vector2& Normal,	//屈折させる法線ベクトル
		float RefractionIndex	//屈折率
		);
		用途: ベクトルを法線ベクトルによって屈折率で反射させたベクトル(入射ベクトル)を返す
		戻り値: 反射させたベクトル
		***************************************************************************/
		  inline  Vector2 Refract(const Vector2& Vec, const Vector2& Normal, float RefractionIndex) {
			return (Vector2)XMVector2Refract(XMVECTOR(Vec), XMVECTOR(Normal), RefractionIndex);
		}
		/**************************************************************************
		  inline  Vector2 RefractV(
		const Vector2& Vec,		//ベクトル
		const Vector2& Normal,	//屈折させる法線ベクトル
		const Vector2& RefractionIndex	//屈折率
		);
		用途: ベクトルを法線ベクトルによって屈折率ベクトル（すべての要素が同じ値）
		　　　で反射させたベクトル(入射ベクトル)を返す
		   戻り値: 反射させたベクトル
		   ***************************************************************************/
		  inline  Vector2 RefractV(const Vector2& Vec, const Vector2& Normal, const Vector2& RefractionIndex) {
			return (Vector2)XMVector2RefractV(XMVECTOR(Vec), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//トランスフォーム
		/**************************************************************************
		inline  Vector2 Transform(
		const Vector2& Vec,		//ベクトル
		const XMMATRIX& m		//トランスフォームする行列
		);
		用途: ベクトルをmによってトランスフォームさせたベクトルを返す
		戻り値: トランスフォームさせたベクトル
		***************************************************************************/
		inline  Vector2 Transform(const Vector2& Vec, const XMMATRIX& m) {
			return (Vector2)XMVector2Transform(XMVECTOR(Vec), m);
		}
		/**************************************************************************
		inline  Vector2 Transform(
		const Vector2& Vec,		//ベクトル
		const Matrix4X4& m		//トランスフォームする行列
		);
		用途: ベクトルをmによってトランスフォームさせたベクトルを返す
		戻り値: トランスフォームさせたベクトル
		***************************************************************************/
		inline  Vector2 Transform(const Vector2& Vec, const Matrix4X4& m) {
			return (Vector2)XMVector2Transform(XMVECTOR(Vec), XMMATRIX(m));
		}
	};
	// end Vector2EX

	void Vector3::Transform(const Matrix4X4& m) {
		*this = XMVector3Transform(XMVECTOR(*this), XMMATRIX(m));
	}


	/**************************************************************************
	struct Vector3EX;
	用途: 3次元XMVECTORの  inline 関数構造体
	****************************************************************************/
	namespace Vector3EX {
		//作成
		/**************************************************************************
		  inline  Vector3 WStr3ToVec3(
		wstring& x_str,
		wstring& y_str,
		wstring& z_str
		);
		用途: ユーティリティ関数
		戻り値: wstringからベクトルを作成する
		***************************************************************************/
		inline Vector3 WStr3ToVec3(wstring& x_str, wstring& y_str, wstring& z_str) {
			return
				Vector3(
					(float)_wtof(x_str.c_str()),
					(float)_wtof(y_str.c_str()),
					(float)_wtof(z_str.c_str())
					);
		}
		//比較
		/**************************************************************************
		  inline  bool Equal(
		const Vector3& Vec,		//ベクトル
		const Vector3& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しいかどうかを検証する
		戻り値: 等しければtrue
		***************************************************************************/
		inline  bool Equal(const Vector3& Vec, const Vector3& other) {
			return XMVector3Equal(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool EqualInt(
		const Vector3& Vec,		//ベクトル
		const Vector3& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しいかどうかを検証する。
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しければtrue
		***************************************************************************/
		inline  bool EqualInt(const Vector3& Vec, const Vector3& other) {
			return XMVector3EqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool Greater(
		const Vector3& Vec,		//ベクトル
		const Vector3& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルより大きいかどうかを検証する。
		戻り値: 大きければtrue
		***************************************************************************/
		  inline  bool Greater(const Vector3& Vec, const Vector3& other) {
			return XMVector3Greater(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool GreaterOrEqual(
		const Vector3& Vec,		//ベクトル
		const Vector3& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトル以上の大きさかどうかを検証する。
		戻り値: 以上の大きさならばtrue
		***************************************************************************/
		  inline  bool GreaterOrEqual(const Vector3& Vec, const Vector3& other) {
			return XMVector3GreaterOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool IsInfinite(
		const Vector3& Vec		//ベクトル
		);
		用途: ベクトルが値のいずれかが正か負の無限大かどうかを検証する。
		戻り値: 値のいずれかが正か負の無限大ならばtrue
		***************************************************************************/
		  inline  bool IsInfinite(const Vector3& Vec) {
			return XMVector3IsInfinite(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline  bool IsNaN(
		const Vector3& Vec		//ベクトル
		);
		用途: ベクトルの値のいずれかが非数（無効値）かどうかを検証する。
		戻り値: 値のいずれかが非数（無効値）ならばtrue
		***************************************************************************/
		  inline  bool IsNaN(const Vector3& Vec) {
			return XMVector3IsNaN(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline  bool Less(
		const Vector3& Vec,		//ベクトル
		const Vector3& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルより小さいかどうかを検証する。
		戻り値: 小さければtrue
		***************************************************************************/
		  inline  bool Less(const Vector3& Vec, const Vector3& other) {
			return XMVector3Less(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool LessOrEqual(
		const Vector3& Vec,		//ベクトル
		const Vector3& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトル以下の大きさかどうかを検証する。
		戻り値: 以下の大きさならばtrue
		***************************************************************************/
		  inline  bool LessOrEqual(const Vector3& Vec, const Vector3& other) {
			return XMVector3LessOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool NearEqual(
		const Vector3& Vec,		//ベクトル
		const Vector3& other,	//比較するベクトル
		float Epsilon			//比較する最大許容差
		);
		用途: ベクトルがもう一つのベクトルが近い大きさかどうかを検証する。
		戻り値: 近い大きさならばtrue
		***************************************************************************/
		  inline  bool NearEqual(const Vector3& Vec, const Vector3& other, float Epsilon) {
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector3NearEqual(XMVECTOR(Vec), XMVECTOR(other), VecEp);
		}
		/**************************************************************************
		  inline  bool NotEqual(
		const Vector3& Vec,		//ベクトル
		const Vector3& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しくないかどうかを検証する
		戻り値: 等しくなければtrue
		***************************************************************************/
		  inline  bool NotEqual(const Vector3& Vec, const Vector3& other) {
			return XMVector3NotEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool NotEqualInt(
		const Vector3& Vec,		//ベクトル
		const Vector3& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しくないかどうかを検証する
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しくなければtrue
		***************************************************************************/
		  inline  bool NotEqualInt(const Vector3& Vec, const Vector3& other) {
			return XMVector3NotEqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		//ジオメトリ
		/**************************************************************************
		  inline  float AngleBetweenNormals(
		const Vector3& Vec1,	//ベクトル1
		const Vector3& Vec2		//ベクトル2
		);
		用途: 正規化したベクトル同士のラジアン角度を計算する
		戻り値: ラジアン角度
		＊正規化はしないので、引数ははあらかじめ正規化しておく
		＊ベクトルのラジアン角計算では、正規化しない分速い
		***************************************************************************/
		  inline  float AngleBetweenNormals(const Vector3& Vec1, const Vector3& Vec2) {
			return ((Vector3)XMVector3AngleBetweenNormals(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline  float AngleBetweenNormalsEst(
		const Vector3& Vec1,	//ベクトル1
		const Vector3& Vec2		//ベクトル2
		);
		用途: 正規化したベクトル同士のラジアン角度を計算予測する
		戻り値: ラジアン角度
		＊正規化はしないので、引数ははあらかじめ正規化しておく
		＊ベクトルのラジアン角計算では、正規化しない分速い
		***************************************************************************/
		  inline  float AngleBetweenNormalsEst(const Vector3& Vec1, const Vector3& Vec2) {
			return ((Vector3)XMVector3AngleBetweenNormalsEst(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline  float AngleBetweenVectors(
		const Vector3& Vec1,	//ベクトル1
		const Vector3& Vec2		//ベクトル2
		);
		用途: 二つの ベクトル間のラジアン角を計算する。
		戻り値: ラジアン角度を表すベクトル
		***************************************************************************/
		  inline  float AngleBetweenVectors(const Vector3& Vec1, const Vector3& Vec2) {
			return ((Vector3)XMVector3AngleBetweenVectors(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline  Vector3 ClampLength(
		const Vector3& Vec,	//ベクトル
		float LengthMin,	//長さの最小値
		float LengthMax		//長さの最大値
		);
		用途: ベクトルの長さを指定した大きさにクランプする
		戻り値: クランプした結果ベクトル
		***************************************************************************/
		  inline  Vector3 ClampLength(const Vector3& Vec, float LengthMin, float LengthMax) {
			return (Vector3)XMVector3ClampLength(XMVECTOR(Vec), LengthMin, LengthMax);
		}
		/**************************************************************************
		  inline  Vector3 ClampLengthV(
		const Vector3& Vec,				//ベクトル
		const Vector3& LengthMinV,		//長さの最小値(ベクトル)
		const Vector3& LengthMaxV		//長さの最大値(ベクトル)
		);
		用途: ベクトルを指定した大きさにクランプする
		戻り値: クランプしたベクトル
		***************************************************************************/
		  inline  Vector3 ClampLengthV(const Vector3& Vec, const Vector3& LengthMinV, const Vector3& LengthMaxV) {
			return (Vector3)XMVector3ClampLengthV(XMVECTOR(Vec), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		/**************************************************************************
		  inline  Vector3 Cross(
		const Vector3& Vec1,	//ベクトル１
		const Vector3& Vec2		//ベクトル２
		);
		用途: 外積を計算して返す
		戻り値: 外積
		***************************************************************************/
		  inline  Vector3 Cross(const Vector3& Vec1, const Vector3& Vec2) {
			return (Vector3)XMVector3Cross(XMVECTOR(Vec1), XMVECTOR(Vec2));
		}
		/**************************************************************************
		  inline  float Dot(
		const Vector3& Vec1,	//ベクトル１
		const Vector3& Vec2		//ベクトル２
		);
		用途: ベクトル同士の内積を計算して返す
		戻り値: 内積
		***************************************************************************/
		inline  float Dot(const Vector3& Vec1, const Vector3& Vec2) {
			return ((Vector3)XMVector3Dot(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline  bool InBounds(
		const Vector3& Vec1,	//比較元ベクトル
		const Vector3& Vec2		//比較するベクトル
		);
		用途: すべての要素が、設定された境界ベクトル内にある場合はtrueを返す
		戻り値: 境界内にある場合はtrue
		***************************************************************************/
		  inline  bool InBounds(const Vector3& Vec1, const Vector3& Vec2) {
			return XMVector3InBounds(XMVECTOR(Vec1), XMVECTOR(Vec2));
		}
		/**************************************************************************
		  inline  float Length(
		const Vector3& Vec,	//ベクトル
		);
		用途: ベクトルの長さを返す
		戻り値: ベクトルの長さ
		***************************************************************************/
		inline float Length(const Vector3& Vec) {
			return ((Vector3)XMVector3Length(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  float LengthEst(
		const Vector3& Vec	//ベクトル
		);
		用途: ベクトルの長さを予想して返す
		戻り値: ベクトルの長さの予想
		***************************************************************************/
		  inline  float LengthEst(const Vector3& Vec) {
			return ((Vector3)XMVector3LengthEst(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  float LengthSq(
		const Vector3& Vec	//ベクトル
		);
		用途: ベクトルの長さの2乗を返す
		戻り値: ベクトルの長さの2乗
		***************************************************************************/
		  inline  float LengthSq(const Vector3& Vec) {
			return ((Vector3)XMVector3LengthSq(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  Vector3 Normalize(
		const Vector3& Vec	//ベクトル
		);
		用途: ベクトルを正規化する
		戻り値: 正規化したベクトル
		***************************************************************************/
		  inline  Vector3 Normalize(const Vector3& Vec) {
			return (Vector3)XMVector3Normalize(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline  Vector3 NormalizeEst(
		const Vector3& Vec	//ベクトル
		);
		用途: 正規化の予想値を返す
		戻り値: 正規化の予想値
		***************************************************************************/
		  inline  Vector3 NormalizeEst(const Vector3& Vec) {
			return (Vector3)XMVector3NormalizeEst(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline  Vector3 Orthogonal(
		const Vector3& Vec	//ベクトル
		);
		用途: 垂直なベクトルを返す
		戻り値: 垂直なベクトル
		***************************************************************************/
		  inline  Vector3 Orthogonal(const Vector3& Vec) {
			return (Vector3)XMVector3Orthogonal(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline  float ReciprocalLength(
		const Vector3& Vec	//ベクトル
		);
		用途: ベクトルの長さの逆数を返す
		戻り値: ベクトルの長さの逆数
		***************************************************************************/
		  inline  float ReciprocalLength(const Vector3& Vec) {
			return ((Vector3)XMVector3ReciprocalLength(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  float ReciprocalLengthEst(
		const Vector3& Vec	//ベクトル
		);
		用途: ベクトルの長さの逆数の予測値を返す
		戻り値: ベクトルの長さの逆数の予測値
		***************************************************************************/
		  inline  float ReciprocalLengthEst(const Vector3& Vec) {
			return ((Vector3)XMVector3ReciprocalLengthEst(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  Vector3 Reflect(
		const Vector3& Vec,	//ベクトル
		const Vector3& Normal	//法線ベクトル
		);
		用途: ベクトルを法線ベクトルによって反射させたベクトル(入射ベクトル)を返す
		戻り値: 反射させたベクトル
		***************************************************************************/
		  inline  Vector3 Reflect(const Vector3& Vec, const Vector3& Normal) {
			return (Vector3)XMVector3Reflect(XMVECTOR(Vec), XMVECTOR(Normal));
		}
		/**************************************************************************
		  inline  Vector3 Refract(
		const Vector3& Vec,		//ベクトル
		const Vector3& Normal,	//屈折させる法線ベクトル
		float RefractionIndex	//屈折率
		);
		用途: ベクトルを法線ベクトルによって屈折率で反射させたベクトル(入射ベクトル)をthisに設定する
		戻り値: 反射させたベクトル
		***************************************************************************/
		  inline  Vector3 Refract(const Vector3& Vec, const Vector3& Normal, float RefractionIndex) {
			return (Vector3)XMVector3Refract(XMVECTOR(Vec), XMVECTOR(Normal), RefractionIndex);
		}
		/**************************************************************************
		  inline  Vector3 RefractV(
		const Vector3& Vec,		//ベクトル
		const Vector3& Normal,	//屈折させる法線ベクトル
		const Vector3& RefractionIndex	//屈折率
		);
		用途: ベクトルを法線ベクトルによって屈折率ベクトル（すべての要素が同じ値）
		　　　で反射させたベクトル(入射ベクトル)を返す
		   戻り値: 反射させたベクトル
		   ***************************************************************************/
		  inline  Vector3 RefractV(const Vector3& Vec, const Vector3& Normal, const Vector3& RefractionIndex) {
			return (Vector3)XMVector3RefractV(XMVECTOR(Vec), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//トランスフォーム
		/**************************************************************************
		inline  Vector3 Transform(
		const Vector3& Vec,		//ベクトル
		const XMMATRIX& m		//トランスフォームする行列
		);
		用途: ベクトルをmによってトランスフォームさせたベクトルを返す
		戻り値: トランスフォームさせたベクトル
		***************************************************************************/
		inline  Vector3 Transform(const Vector3& Vec, const XMMATRIX& m) {
			return (Vector3)XMVector3Transform(XMVECTOR(Vec), m);
		}
		/**************************************************************************
		inline  Vector3 Transform(
		const Vector3& Vec,		//ベクトル
		const Matrix4X4& m		//トランスフォームする行列
		);
		用途: ベクトルをmによってトランスフォームさせたベクトルを返す
		戻り値: トランスフォームさせたベクトル
		***************************************************************************/
		inline  Vector2 Transform(const Vector2& Vec, const Matrix4X4& m) {
			return (Vector3)XMVector3Transform(XMVECTOR(Vec), XMMATRIX(m));
		}


	};
	//end Vector3EX

	void Vector4::Transform(const Matrix4X4& m) {
		*this = XMVector4Transform(XMVECTOR(*this), XMMATRIX(m));
	}

	/**************************************************************************
	namespace Vector4EX;
	用途: 4次元XMVECTORの  inline 関数構造体
	****************************************************************************/
	namespace Vector4EX {
		//作成
		/**************************************************************************
		  inline  Vector4 WStr4ToVec4(
		wstring& x_str,
		wstring& y_str,
		wstring& z_str,
		wstring& w_str
		);
		用途: ユーティリティ関数
		戻り値: wstringからベクトルを作成する
		***************************************************************************/
		  inline  Vector4 WStr4ToVec4(wstring& x_str, wstring& y_str, wstring& z_str, wstring& w_str) {
			return
				Vector4(
					(float)_wtof(x_str.c_str()),
					(float)_wtof(y_str.c_str()),
					(float)_wtof(z_str.c_str()),
					(float)_wtof(w_str.c_str())
					);
		}
		//比較
		/**************************************************************************
		  inline  bool Equal(
		const Vector4& Vec,		//ベクトル
		const Vector4& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しいかどうかを検証する
		戻り値: 等しければtrue
		***************************************************************************/
		  inline  bool Equal(const Vector4& Vec, const Vector4& other) {
			return XMVector4Equal(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool EqualInt(
		const Vector4& Vec,		//ベクトル
		const Vector4& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しいかどうかを検証する。
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しければtrue
		***************************************************************************/
		  inline  bool EqualInt(const Vector4& Vec, const Vector4& other) {
			return XMVector4EqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool Greater(
		const Vector4& Vec,		//ベクトル
		const Vector4& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルより大きいかどうかを検証する。
		戻り値: 大きければtrue
		***************************************************************************/
		  inline  bool Greater(const Vector4& Vec, const Vector4& other) {
			return XMVector4Greater(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool GreaterOrEqual(
		const Vector4& Vec,		//ベクトル
		const Vector4& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトル以上の大きさかどうかを検証する。
		戻り値: 以上の大きさならばtrue
		***************************************************************************/
		  inline  bool GreaterOrEqual(const Vector4& Vec, const Vector4& other) {
			return XMVector4GreaterOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool IsInfinite(
		const Vector4& Vec		//ベクトル
		);
		用途: ベクトルが値のいずれかが正か負の無限大かどうかを検証する。
		戻り値: 値のいずれかが正か負の無限大ならばtrue
		***************************************************************************/
		  inline  bool IsInfinite(const Vector4& Vec) {
			return XMVector4IsInfinite(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline  bool IsNaN(
		const Vector4& Vec		//ベクトル
		);
		用途: ベクトルの値のいずれかが非数（無効値）かどうかを検証する。
		戻り値: 値のいずれかが非数（無効値）ならばtrue
		***************************************************************************/
		  inline  bool IsNaN(const Vector4& Vec) {
			return XMVector4IsNaN(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline  bool Less(
		const Vector4& Vec,		//ベクトル
		const Vector4& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルより小さいかどうかを検証する。
		戻り値: 小さければtrue
		***************************************************************************/
		  inline  bool Less(const Vector4& Vec, const Vector4& other) {
			return XMVector4Less(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool LessOrEqual(
		const Vector4& Vec,		//ベクトル
		const Vector4& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトル以下の大きさかどうかを検証する。
		戻り値: 以下の大きさならばtrue
		***************************************************************************/
		  inline  bool LessOrEqual(const Vector4& Vec, const Vector4& other) {
			return XMVector4LessOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool NearEqual(
		const Vector4& Vec,		//ベクトル
		const Vector4& other,	//比較するベクトル
		float Epsilon			//比較する最大許容差
		);
		用途: ベクトルがもう一つのベクトルが近い大きさかどうかを検証する。
		戻り値: 近い大きさならばtrue
		***************************************************************************/
		  inline  bool NearEqual(const Vector4& Vec, const Vector4& other, float Epsilon) {
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector4NearEqual(XMVECTOR(Vec), XMVECTOR(other), VecEp);
		}
		/**************************************************************************
		  inline  bool NotEqual(
		const Vector4& Vec,		//ベクトル
		const Vector4& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しくないかどうかを検証する
		戻り値: 等しくなければtrue
		***************************************************************************/
		  inline  bool NotEqual(const Vector4& Vec, const Vector4& other) {
			return XMVector4NotEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		/**************************************************************************
		  inline  bool NotEqualInt(
		const Vector4& Vec,		//ベクトル
		const Vector4& other	//比較するベクトル
		);
		用途: ベクトルがもう一つのベクトルと等しくないかどうかを検証する
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しくなければtrue
		***************************************************************************/
		  inline  bool NotEqualInt(const Vector4& Vec, const Vector4& other) {
			return XMVector4NotEqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		//ジオメトリ
		/**************************************************************************
		  inline  float AngleBetweenNormals(
		const Vector4& Vec1,	//ベクトル1
		const Vector4& Vec2		//ベクトル2
		);
		用途: 正規化したベクトル同士のラジアン角度を計算する
		戻り値: ラジアン角度
		＊正規化はしないので、引数ははあらかじめ正規化しておく
		＊ベクトルのラジアン角計算では、正規化しない分速い
		***************************************************************************/
		  inline  float AngleBetweenNormals(const Vector4& Vec1, const Vector4& Vec2) {
			return ((Vector4)XMVector4AngleBetweenNormals(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline  float AngleBetweenNormalsEst(
		const Vector4& Vec1,	//ベクトル1
		const Vector4& Vec2		//ベクトル2
		);
		用途: 正規化したベクトル同士のラジアン角度を計算予測する
		戻り値: ラジアン角度を表すベクトル
		＊正規化はしないので、引数ははあらかじめ正規化しておく
		＊ベクトルのラジアン角計算では、正規化しない分速い
		***************************************************************************/
		  inline  float AngleBetweenNormalsEst(const Vector4& Vec1, const Vector4& Vec2) {
			return ((Vector4)XMVector4AngleBetweenNormalsEst(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline  float AngleBetweenVectors(
		const Vector4& Vec1,	//ベクトル1
		const Vector4& Vec2		//ベクトル2
		);
		用途: 二つの ベクトル間のラジアン角を計算する。
		戻り値: ラジアン角度を表すベクトル
		***************************************************************************/
		  inline  float AngleBetweenVectors(const Vector4& Vec1, const Vector4& Vec2) {
			return ((Vector4)XMVector4AngleBetweenVectors(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline  Vector4 ClampLength(
		const Vector4& Vec,	//ベクトル
		float LengthMin,	//長さの最小値
		float LengthMax		//長さの最大値
		);
		用途: ベクトルの長さを指定した大きさにクランプする
		戻り値: クランプした結果ベクトル
		***************************************************************************/
		  inline  Vector4 ClampLength(const Vector4& Vec, float LengthMin, float LengthMax) {
			return (Vector4)XMVector4ClampLength(XMVECTOR(Vec), LengthMin, LengthMax);
		}
		/**************************************************************************
		  inline  Vector4 ClampLengthV(
		const Vector4& Vec,				//ベクトル
		const Vector4& LengthMinV,		//長さの最小値(ベクトル)
		const Vector4& LengthMaxV		//長さの最大値(ベクトル)
		);
		用途: ベクトルを指定した大きさにクランプする
		戻り値: クランプしたベクトル
		***************************************************************************/
		  inline  Vector4 ClampLengthV(const Vector4& Vec, const Vector4& LengthMinV, const Vector4& LengthMaxV) {
			return (Vector4)XMVector4ClampLengthV(XMVECTOR(Vec), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		/**************************************************************************
		  inline  Vector4 Cross(
		const Vector4& Vec1,	//ベクトル１
		const Vector4& Vec2,	//ベクトル２
		const Vector4& Vec3		//ベクトル３
		);
		用途: 外積を計算して返す
		戻り値: なし
		***************************************************************************/
		  inline  Vector4 Cross(const Vector4& Vec1, const Vector4& Vec2, const Vector4& Vec3) {
			return (Vector4)XMVector4Cross(XMVECTOR(Vec1), XMVECTOR(Vec2), XMVECTOR(Vec3));
		}
		/**************************************************************************
		  inline  float Dot(
		const Vector4& Vec1,	//ベクトル１
		const Vector4& Vec2		//ベクトル２
		);
		用途: ベクトル同士の内積を計算して返す
		戻り値: 内積
		***************************************************************************/
		  inline  float Dot(const Vector4& Vec1, const Vector4& Vec2) {
			return ((Vector4)XMVector4Dot(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		/**************************************************************************
		  inline  bool InBounds(
		const Vector4& Vec1,	//比較元ベクトル
		const Vector4& Vec2		//比較するベクトル
		);
		用途: すべての要素が、設定された境界ベクトル内にある場合はtrueを返す
		戻り値: 境界内にある場合はtrue
		***************************************************************************/
		  inline  bool InBounds(const Vector4& Vec1, const Vector4& Vec2) {
			return XMVector4InBounds(XMVECTOR(Vec1), XMVECTOR(Vec2));
		}
		/**************************************************************************
		  inline  float Length(
		const Vector4& Vec,	//ベクトル
		);
		用途: ベクトルの長さを返す
		戻り値: ベクトルの長さ
		***************************************************************************/
		  inline  float Length(const Vector4& Vec) {
			return ((Vector4)XMVector4Length(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  float LengthEst(
		const Vector4& Vec	//ベクトル
		);
		用途: ベクトルの長さを予想して返す
		戻り値: ベクトルの長さの予想
		***************************************************************************/
		  inline  float LengthEst(const Vector4& Vec) {
			return ((Vector4)XMVector4LengthEst(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  float LengthSq(
		const Vector4& Vec	//ベクトル
		);
		用途: ベクトルの長さの2乗を返す
		戻り値: ベクトルの長さの2乗
		***************************************************************************/
		  inline  float LengthSq(const Vector4& Vec) {
			return ((Vector4)XMVector4LengthSq(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  Vector4 Normalize(
		const Vector4& Vec	//ベクトル
		);
		用途: ベクトルを正規化する
		戻り値: 正規化したベクトル
		***************************************************************************/
		  inline  Vector4 Normalize(const Vector4& Vec) {
			return (Vector4)XMVector4Normalize(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline  Vector4 NormalizeEst(
		const Vector4& Vec	//ベクトル
		);
		用途: 正規化の予想値を返す
		戻り値: 正規化の予想値
		***************************************************************************/
		  inline  Vector4 NormalizeEst(const Vector4& Vec) {
			return (Vector4)XMVector4NormalizeEst(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline  Vector4 Orthogonal(
		const Vector4& Vec	//ベクトル
		);
		用途: 垂直なベクトルを返す
		戻り値: 垂直なベクトル
		***************************************************************************/
		  inline  Vector4 Orthogonal(const Vector4& Vec) {
			return (Vector4)XMVector4Orthogonal(XMVECTOR(Vec));
		}
		/**************************************************************************
		  inline  float ReciprocalLength(
		const Vector4& Vec	//ベクトル
		);
		用途: ベクトルの長さの逆数を返す
		戻り値: ベクトルの長さの逆数
		***************************************************************************/
		  inline  float ReciprocalLength(const Vector4& Vec) {
			return ((Vector4)XMVector4ReciprocalLength(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  float ReciprocalLengthEst(
		const Vector4& Vec	//ベクトル
		);
		用途: ベクトルの長さの逆数の予測値を返す
		戻り値: ベクトルの長さの逆数の予測値
		***************************************************************************/
		  inline  float ReciprocalLengthEst(const Vector4& Vec) {
			return ((Vector4)XMVector4ReciprocalLengthEst(XMVECTOR(Vec))).x;
		}
		/**************************************************************************
		  inline  Vector4 Reflect(
		const Vector4& Vec,	//ベクトル
		const Vector4& Normal	//法線ベクトル
		);
		用途: ベクトルを法線ベクトルによって反射させたベクトル(入射ベクトル)を返す
		戻り値: 反射させたベクトル
		***************************************************************************/
		  inline  Vector4 Reflect(const Vector4& Vec, const Vector4& Normal) {
			return (Vector4)XMVector4Reflect(XMVECTOR(Vec), XMVECTOR(Normal));
		}
		/**************************************************************************
		  inline  Vector4 Refract(
		const Vector4& Vec,		//ベクトル
		const Vector4& Normal,	//屈折させる法線ベクトル
		float RefractionIndex	//屈折率
		);
		用途: ベクトルを法線ベクトルによって屈折率で反射させたベクトル(入射ベクトル)をthisに設定する
		戻り値: 反射させたベクトル
		***************************************************************************/
		  inline  Vector4 Refract(const Vector4& Vec, const Vector4& Normal, float RefractionIndex) {
			return (Vector4)XMVector4Refract(XMVECTOR(Vec), XMVECTOR(Normal), RefractionIndex);
		}
		/**************************************************************************
		  inline  Vector4 RefractV(
		const Vector4& Vec,		//ベクトル
		const Vector4& Normal,	//屈折させる法線ベクトル
		const Vector4& RefractionIndex	//屈折率
		);
		用途: ベクトルを法線ベクトルによって屈折率ベクトル（すべての要素が同じ値）
		　　　で反射させたベクトル(入射ベクトル)を返す
		   戻り値: 反射させたベクトル
		   ***************************************************************************/
		  inline  Vector4 RefractV(const Vector4& Vec, const Vector4& Normal, const Vector4& RefractionIndex) {
			return (Vector4)XMVector4RefractV(XMVECTOR(Vec), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//トランスフォーム
		/**************************************************************************
		inline  Vector4 Transform(
		const Vector4& Vec,		//ベクトル
		const XMMATRIX& m		//トランスフォームする行列
		);
		用途: ベクトルをmによってトランスフォームさせたベクトルを返す
		戻り値: トランスフォームさせたベクトル
		***************************************************************************/
		inline  Vector4 Transform(const Vector4& Vec, const XMMATRIX& m) {
			return (Vector4)XMVector4Transform(XMVECTOR(Vec), m);
		}
		/**************************************************************************
		inline  Vector4 Transform(
		const Vector4& Vec,		//ベクトル
		const Matrix4X4& m		//トランスフォームする行列
		);
		用途: ベクトルをmによってトランスフォームさせたベクトルを返す
		戻り値: トランスフォームさせたベクトル
		***************************************************************************/
		inline  Vector4 Transform(const Vector4& Vec, const Matrix4X4& m) {
			return (Vector4)XMVector4Transform(XMVECTOR(Vec), XMMATRIX(m));
		}


	};
	//end Vector4EX


}
//end of namespace basedx11.



