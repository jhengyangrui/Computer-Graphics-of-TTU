#include "svl/svl.h"

class mQuat : public Vec4 
{
public:
//	mQuat(Real q0, Real q1, Real q2, Real q3);
	mQuat();
#if 0
	mQuat (const Vec3 &axis, Real angle);
	mQuat (const Mat3 &mat);
	mQuat(const Vec4& v);

  mQuat &operator *= (const mQuat &v);
  mQuat &operator =  (const Vec4 &v);
#endif
};
//mQuat operator * (const mQuat &a, const mQuat &b);
//mQuat slerp (const mQuat &q1, const mQuat &q2, Real t);