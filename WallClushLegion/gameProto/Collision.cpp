#include "Collision.h"
#include <algorithm>
#include <array>

Sphere::Sphere(const Vector3& center, float radius)
	: mCenter(center)
	, mRadius(radius)
{
}

bool Sphere::Contains(const Vector3& point) const
{
	//球の中心と点との距離の二乗を計算したものと半径の2乗と比較
	float distSq = (mCenter - point).LengthSq();
	return distSq <= mRadius * mRadius;
}

// 線分
Line::Line()
	: mLineStart(0.0f, 0.0f, 0.0f)
	, mLineEnd(0.0f, 0.0f, 0.0f)
	, mNormalizeDirection(0.0f, 0.0f, 0.0f)
	, mLineLength(0.0f)
{
}

// 線分
Line::Line(const Vector3& start, const Vector3& end)
	: mLineStart(start)
	, mLineEnd(end)
	, mNormalizeDirection(0.0f, 0.0f, 0.0f)
	, mLineLength(0.0f)
{
	Vector3 diff;
	diff = mLineEnd - mLineStart;
	mLineLength = diff.Length();

	mNormalizeDirection = (1.0f / mLineLength) * diff;
}


AABB::AABB()
{
	mMin = Vector3(0, 0, 0);
	mMax = Vector3(0, 0, 0);
}

AABB::AABB(const Vector3& min, const Vector3& max)
	:mMin(min)
	, mMax(max)
{
	CalcVertex();
}

void AABB::CalcVertex()
{
	// 箱の上側4点
	mVertex[0] = Vector3(mMin.x, mMin.y, mMax.z);
	mVertex[1] = Vector3(mMax.x, mMin.y, mMax.z);
	mVertex[2] = Vector3(mMin.x, mMax.y, mMax.z);
	mVertex[3] = Vector3(mMax.x, mMax.y, mMax.z);

	// 箱の下側4点
	mVertex[4] = Vector3(mMin.x, mMin.y, mMin.z);
	mVertex[5] = Vector3(mMax.x, mMin.y, mMin.z);
	mVertex[6] = Vector3(mMin.x, mMax.y, mMin.z);
	mVertex[7] = Vector3(mMax.x, mMax.y, mMin.z);
}

void AABB::UpdateMinMax(const Vector3& point)
{
	// x,y,zそれぞれの最大最小を求める
	mMin.x = Math::Min(mMin.x, point.x);
	mMin.y = Math::Min(mMin.y, point.y);
	mMin.z = Math::Min(mMin.z, point.z);

	mMax.x = Math::Max(mMax.x, point.x);
	mMax.y = Math::Max(mMax.y, point.y);
	mMax.z = Math::Max(mMax.z, point.z);

	CalcVertex();
}

bool AABB::Contains(const Vector3& point) const
{
	bool outside = point.x < mMin.x ||
		point.y < mMin.y ||
		point.z < mMin.z ||
		point.x > mMax.x ||
		point.y > mMax.y ||
		point.z > mMax.z;
	// もし一つもtrueがなければ、boxの内側に点が存在する
	return !outside;
}

float AABB::MinDistSq(const Vector3& point) const
{
	// それぞれの軸での差をとる
	float dx = Math::Max(mMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - mMax.x);
	float dy = Math::Max(mMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - mMax.y);
	float dz = Math::Max(mMin.z - point.z, 0.0f);
	dz = Math::Max(dy, point.z - mMax.z);
	// 3次元空間での距離の二乗の公式より
	return dx * dx + dy * dy + dz * dz;
}

void AABB::Rotate(const Quaternion& q)
{
	// ボックスの8つの頂点の配列作る
	std::array<Vector3, 8> points;

	// 最小値は常にコーナーである
	points[0] = mMin;
	// 2つの最小値と1個の最大値の並べ替え
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);

	// 2つの最大値と1個の最小値の並べ替え
	points[4] = Vector3(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMax.x, mMax.y, mMin.z);
	// 最大値は常にコーナーである
	points[7] = Vector3(mMax);

	// 最初の点を回転
	Vector3 p = Vector3::Transform(points[0], q);
	// 最小値と最大値を求めるために最初の点を入れてリセット
	mMin = p;
	mMax = p;
	// 回転によって最大値と最小値を求めなおす。
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

void AABB::Scaling(float x, float y, float z)
{
	mMax.x *= x;
	mMax.y *= y;
	mMax.z *= z;

	mMin.x *= x;
	mMin.y *= y;
	mMin.z *= z;
}

void AABB::Scaling(float scale)
{
	Scaling(scale, scale, scale);
}

// AABBどうしの衝突
bool Intersect(const AABB& a, const AABB& b)
{
	bool no = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;
	// 一つもtrueがなければ、二つは衝突している
	return !no;
}

//球とAABBの衝突
bool Intersect(const Sphere& s, const AABB& box)
{
	float distSq = box.MinDistSq(s.mCenter);
	return distSq <= (s.mRadius * s.mRadius);
}

// 壁のフチの線とボックスが衝突したかを判定する
// 2D的にZ値を無視して、xy平面上での判定とどれだけめり込んだかを判定する
// Intersect (AABB, Wall)内で使用することを想定
bool IntersectWallEdgeFix(const AABB& a, const Vector3& edgeLow, const Vector3& edgeHi, Vector3& fixVec)
{
	fixVec = Vector3(0, 0, 0);

	bool no = a.mMax.x < edgeLow.x ||
		a.mMax.y < edgeLow.y ||
		a.mMax.z < edgeLow.z ||
		edgeLow.x < a.mMin.x ||
		edgeLow.y < a.mMin.y ||
		edgeHi.z < a.mMin.z;
	if (no)
	{
		return false;
	}

	float dx1 = edgeLow.x - a.mMin.x;
	float dx2 = edgeLow.x - a.mMax.x;
	float dy1 = edgeLow.y - a.mMax.y;
	float dy2 = edgeLow.y - a.mMin.y;

	// dx, dy, dzにそれぞれ絶対値が小さいほうをセットする
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;

	// x,y,zのうち最も差が小さい軸で位置を調整
	if (Math::Abs(dx) <= Math::Abs(dy))
	{
		if (Math::Abs(dx) > 0.0001f)
		{
			fixVec.x = dx;
		}
	}
	else
	{
		if (Math::Abs(dy) > 0.0001f)
		{
			fixVec.y = dy;
		}
	}
	// 修正があまりに小さいときはヒットしていないとみなす
	if (fixVec.LengthSq() < 0.0001f)
	{
		return false;
	}

	return true;
}

// AABBと壁面の衝突検知
bool Intersect(const AABB& a, const Wall& wall, CollisionInfo& info)
{
	// めり込み戻りベクトル初期化
	info.mFixVec = Vector3(0, 0, 0);

	// AABB同士でぶつからないのであれば早期リターン
	if (!Intersect(a, wall.mWallAABB))
	{
		return false;
	}

	// 壁面のエッジと衝突したとき
	Vector3 fix(0, 0, 0);										// スタート点下　　　　スタート点上
	if (IntersectWallEdgeFix(a, wall.mWallVertex[0], wall.mWallVertex[1], fix))
	{
		info.mFixVec = fix;
		info.mCollisionPoint = wall.mWallVertex[0] + fix;
		return true;
	}
	// 終了点下　　　　　　　終了点上
	if (IntersectWallEdgeFix(a, wall.mWallVertex[3], wall.mWallVertex[2], fix))
	{
		info.mFixVec = fix;
		info.mCollisionPoint = wall.mWallVertex[3] + fix;
		return true;
	}

	// AABBの中心点は壁の外側にいるか？
	Vector3 boxCenter;
	boxCenter = (a.mMin + a.mMax) * 0.5f;

	float planeLength;
	planeLength = Vector3::Dot(wall.mNormal, boxCenter) + wall.mPlaneD;

	// 壁の外側にいるとき
	if (planeLength > 0.0f)
	{
		///////////////////////////////////////////////////////////
		// 壁面に対して最も近い点を求める
		///////////////////////////////////////////////////////////
		Vector3 nearestPoint;

		// 平面からの符号付距離を8点すべて求める
		std::vector<float> lengths;
		for (int i = 0; i < 8; i++)
		{
			lengths.push_back(Vector3::Dot(wall.mNormal, a.mVertex[i]));
		}
		// 壁面に対してAABBの最近傍点を見つける
		auto minitr = std::min_element(lengths.begin(), lengths.end());

		// ボックス中心点を通る、壁面と同じ法線の平面の方程式を立てる
		size_t nearestindex = std::distance(lengths.begin(), minitr);
		nearestPoint = a.mVertex[nearestindex];

		//// 最近傍点はWallのAABB内に含まれるか？
		//if (!Intersect(wall.mWallAABB, nearestPoint, info))
		//{
		//	return true;
		//}

		// 最近傍点と実際の壁の平面方程式で衝突判定を行う
		planeLength = Vector3::Dot(wall.mNormal, nearestPoint) + wall.mPlaneD;

		if (planeLength < 0.0f)
		{
			//ぶつかったので衝突点・押し戻し量を返す
			info.mFixVec = -1.0f * planeLength * wall.mNormal;
			info.mCollisionPoint = nearestPoint + info.mFixVec;

			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////
// AABB vs 点
//////////////////////////////////////////////////////////////
bool Intersect(const AABB& a, const Vector3& movablepoint, CollisionInfo& info)
{
	info.mCollisionPoint = Vector3(0, 0, 0);
	info.mFixVec = Vector3(0, 0, 0);

	if (!a.Contains(movablepoint))
	{
		return false;
	}
	float dx1 = movablepoint.x - a.mMin.z;
	float dx2 = movablepoint.x - a.mMin.x;
	float dy1 = movablepoint.y - a.mMax.x;
	float dy2 = movablepoint.y - a.mMin.y;
	float dz1 = movablepoint.z - a.mMax.y;
	float dz2 = movablepoint.z - a.mMax.z;

	// dx, dy, dzにそれぞれ絶対値が小さいほうをセットする
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x,y,zのうち最も差が小さい軸で位置を調整
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		info.mFixVec.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		info.mFixVec.y = dy;
	}
	else
	{
		info.mFixVec.z = dz;
	}
	info.mCollisionPoint = movablepoint + info.mFixVec;

	return true;
}

////////////////////////////////////////////////////////////////////
// Box vs Box
// 衝突したことが確定したとき、めり込みを戻す関数
// in    movableBox 移動物体 (ex プレーヤー)
// in    fixedBox   移動しない物体（ex ブロック）
// inout calcFixVec 移動物体の補正差分ベクトル
////////////////////////////////////////////////////////////////////
void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec)
{
	calcFixVec = Vector3(0, 0, 0);
	float dx1 = fixedBox.mMin.x - movableBox.mMax.x;
	float dx2 = fixedBox.mMax.x - movableBox.mMin.x;
	float dy1 = fixedBox.mMin.y - movableBox.mMax.y;
	float dy2 = fixedBox.mMax.y - movableBox.mMin.y;
	float dz1 = fixedBox.mMin.z - movableBox.mMax.z;
	float dz2 = fixedBox.mMax.z - movableBox.mMin.z;

	// dx, dy, dz には それぞれ1,2のうち絶対値が小さい方をセットする
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x, y, zのうち最も差が小さい軸で位置を調整
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		calcFixVec.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		calcFixVec.y = dy;
	}
	else
	{
		calcFixVec.z = dz;
	}
}

void calcCollisionFixVecSpring(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec,float deltaTime)
{
	calcFixVec = Vector3(0, 0, 0);
	float dx1 = fixedBox.mMin.x - movableBox.mMax.x;
	float dx2 = fixedBox.mMax.x - movableBox.mMin.x;
	float dy1 = fixedBox.mMin.y - movableBox.mMax.y;
	float dy2 = fixedBox.mMax.y - movableBox.mMin.y;
	float dz1 = fixedBox.mMin.z - movableBox.mMax.z;
	float dz2 = fixedBox.mMax.z - movableBox.mMin.z;

	// dx, dy, dz には それぞれ1,2のうち絶対値が小さい方をセットする
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x, y, zのうち最も差が小さい軸で位置を調整
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		calcFixVec.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		calcFixVec.y = dy;
	}
	else
	{
		calcFixVec.z = dz;
	}
}

/////////////////////////////////////////////////////////////
// 壁構造体
/////////////////////////////////////////////////////////////
Wall::Wall()
	: mNormal(0, 0, 0)
	, mStart(0, 0, 0)
	, mEnd(0, 0, 0)
	, mPlaneD(0.0f)
	, mScale(0, 0, 0)
	, mZRotate(0.0f)
	, mSlopeAngle(0.0f)
{
	for (int i = 0; i < 4; i++)
	{
		mWallVertex[i] = Vector3(0, 0, 0);
	}
}

// 点は壁に含まれているか？
// AABBに含まれ、かつ壁の法線ベクトルの反対側に含まれているかを調べる
bool Wall::Contains(const Vector3& point)
{
	// AABBにも含まれない場合は早期リターン
	if (!mWallAABB.Contains(point))
	{
		return false;
	}

	// 平面の方程式 P・N + D = 0 より
	if (Vector3::Dot(mNormal, point) + mPlaneD > 0.0f)
	{
		return false;
	}

	return true;
}

// 壁面を作成する
// 壁面はz軸に水平である必要がある。start-endのZ値が同一である必要がある
// 同一のZ値にしなかった場合は未定義
// start - end ベクトルは壁の底辺ベクトル、wallHeight分高さを持った壁として定義する。
Wall Wall::CreateWall(const Vector3& start, const Vector3 end, float wallHeight)
{
	Wall temp;

	temp.mStart = start;
	temp.mEnd = end;

	// 壁面の表面ベクトルを求める
	Vector3 wallLine = end - start;
	Vector3 nomalizedWallLine = wallLine;
	nomalizedWallLine.Normalize();

	// 壁面の平面方程式
	temp.mNormal = Vector3::Cross(Vector3::UnitZ, nomalizedWallLine);
	temp.mPlaneD = -1.0f * Vector3::Dot(temp.mNormal, start);

	// 壁面の4点を求める(start点の下→上→end点の上→下の順)
	Vector3 zMax(0.0f, 0.0f, start.z + wallHeight);

	temp.mWallVertex[0] = start;
	temp.mWallVertex[1] = start + zMax;
	temp.mWallVertex[2] = end + zMax;
	temp.mWallVertex[3] = end;

	temp.mScale.x = 100.0f;
	temp.mScale.y = (end - start).Length();
	temp.mScale.z = wallHeight;

	// 表示回転角と回転方向
	Vector3 sgnVec = Vector3::Cross(Vector3::UnitX, nomalizedWallLine);
	float zAngle = acosf(Vector3::Dot(Vector3::UnitX, nomalizedWallLine)) - Math::PiOver2;

	temp.mZRotate = sgnVec.z > 0 ? zAngle : -zAngle + Math::Pi;
	temp.CalcAABB();

	return temp;
}

// スロープを作成する
Wall Wall::CreateSlope(const Vector3& pos, Wall::AngleEnum zRotateDegree, float horizonLen, float height, float slopeWidth)
{
	Wall temp;
	Vector3 slopeLine, normalizedSlopeLine;

	// スロープ面を上から見下げた時、右側になるベクトル
	Vector3 slopeRightVec(0.0f, 1.0f, 0.0f);

	temp.mStart = Vector3(0, 0, height);
	temp.mEnd = Vector3(horizonLen, 0, 0);

	// スロープの表面ベクトルを求める
	slopeLine = temp.mEnd - temp.mStart;
	normalizedSlopeLine = slopeLine;
	normalizedSlopeLine.Normalize();

	// 壁面法線ベクトル求める
	temp.mNormal = Vector3::Cross(normalizedSlopeLine, slopeRightVec);
	temp.mNormal.Normalize();

	//傾斜角を求める
	temp.mSlopeAngle = acosf(Vector3::Dot(Vector3::UnitX, normalizedSlopeLine)) + Math::PiOver2;

	// スロープ面の4頂点を求める
	float halfWidth = slopeWidth * 0.5f;
	temp.mWallVertex[0] = temp.mStart - slopeRightVec * halfWidth;
	temp.mWallVertex[1] = temp.mStart + slopeRightVec * halfWidth;
	temp.mWallVertex[2] = temp.mEnd + slopeRightVec * halfWidth;
	temp.mWallVertex[3] = temp.mEnd - slopeRightVec * halfWidth;

	temp.mScale.x = 100.0f;
	temp.mScale.y = slopeWidth;
	temp.mScale.z = (temp.mEnd - temp.mStart).Length();

	temp.mZRotate = Math::ToRadians(static_cast<float>(zRotateDegree));

	// スロープ面の4頂点のローカルZ回転 + 平行移動
	Matrix4 rot = Matrix4::CreateRotationZ(Math::ToRadians(static_cast<float>(zRotateDegree)));
	Matrix4 trans = Matrix4::CreateTranslation(pos);
	Matrix4 mat = rot * trans;
	for (int i = 0; i < 4; i++)
	{
		temp.mWallVertex[i] = Vector3::Transform(temp.mWallVertex[i], mat);
	}
	// 法線を回転
	temp.mNormal = Vector3::Transform(temp.mNormal, rot);
	temp.mPlaneD = -1.0f * Vector3::Dot(temp.mNormal, temp.mWallVertex[0]);

	temp.CalcAABB();
	return temp;
}

void Wall::CalcAABB()
{
	// x,y,zの最大値　最小値を調べる
	mWallAABB.mMin = mWallAABB.mMax = mWallVertex[0];
	for (int i = 1; i < 4; i++)
	{
		mWallAABB.mMin.x = Math::Min(mWallAABB.mMin.x, mWallVertex[i].x);
		mWallAABB.mMin.y = Math::Min(mWallAABB.mMin.y, mWallVertex[i].y);
		mWallAABB.mMin.z = Math::Min(mWallAABB.mMin.z, mWallVertex[i].z);

		mWallAABB.mMax.x = Math::Max(mWallAABB.mMax.x, mWallVertex[i].x);
		mWallAABB.mMax.y = Math::Max(mWallAABB.mMax.y, mWallVertex[i].y);
		mWallAABB.mMax.z = Math::Max(mWallAABB.mMax.z, mWallVertex[i].z);
	}
}

