#include "svl/mQuat.h"
#include <cctype>
#include <iomanip>

//
// mQuat in SVL: [(q0,q1,q2),q3]
//
// implement:
//
// mQuaterion multiplication
// 3x3 -> mQuat; mQuat->3x3 already available 
// slerp
//
// find application that demonstrate the needs ...
//
mQuat::mQuat(){}
#if 0
mQuat::mQuat(Real q0, Real q1, Real q2, Real q3)
{
	elt[0] = q0;
	elt[1] = q1;
	elt[2] = q2;
	elt[3] = q3;
}

mQuat::mQuat(const Vec3 &axis, Real angle)
{
	Vec3 n = norm(axis);
	Real sinhalf = sin(angle/2);
	elt[0] = sinhalf*n[0];
	elt[1] = sinhalf*n[1];
	elt[2] = sinhalf*n[2];
	elt[3] = cos(angle/2);
}
mQuat::mQuat(const Vec4 &v)
{
	elt[0] = v[0];
	elt[1] = v[1];
	elt[2] = v[2];
	elt[3] = v[3];
}

mQuat::mQuat(const Mat3 & m)
{   
	Real compare[4], max;
    compare[0] = 1 + m[0][0] + m[1][1] + m[2][2];
    compare[1] = 1 + m[0][0] - m[1][1] - m[2][2];
    compare[2] = 1 - m[0][0] + m[1][1] - m[2][2];
    compare[3] = 1 - m[0][0] - m[1][1] + m[2][2];

	int i, which;
	for (i = 0, which = -1, max = 0; i < 4; i++)
	if (fabs(compare[i]) > max)
	    which = i, max = fabs(compare[i]);

	Real q0,q1,q2,q3;
	switch (which) {
      case 0:
		q0 = 0.5*sqrt (compare[which]);
		q1 = 0.25*(m[2][1] - m[1][2])/q0;
		q2 = 0.25*(m[0][2] - m[2][0])/q0;
		q3 = 0.25*(m[1][0] - m[0][1])/q0;
		break;

      case 1:
		q1 = 0.5*sqrt (compare[which]);
		q0 = 0.25*(m[2][1] - m[1][2])/q1;
		q2 = 0.25*(m[0][1] + m[1][0])/q1;
		q3 = 0.25*(m[0][2] + m[2][0])/q1;
		break;

      case 2:
		q2 = 0.5*sqrt (compare[which]);
		q0 = 0.25*(m[0][2] - m[2][0])/q2;
		q1 = 0.25*(m[0][1] + m[1][0])/q2;
		q3 = 0.25*(m[1][2] + m[2][1])/q2;
		break;

      case 3:
		q3 = 0.5*sqrt (compare[which]);
		q0 = 0.25*(m[1][0] - m[0][1])/q3;
		q1 = 0.25*(m[0][2] + m[2][0])/q3;
		q2 = 0.25*(m[1][2] + m[2][1])/q3;
		break;
    }
	elt[0] = q1, elt[1] = q2, elt[2] = q3, elt[3] = q0;
}


mQuat operator * (const mQuat &a, const mQuat &b)
{
//	cout << "in mQuat *" << endl;

	Vec3 av(a[0],a[1],a[2]); Real as = a[3];
	Vec3 bv(b[0],b[1],b[2]); Real bs = b[3];

	Vec3 rv = as*bv + bs*av + cross(av,bv);
	Real rs = as*bs - dot(av,bv);

	mQuat result(rv[0],rv[1],rv[2],rs);
    return(result);
}

mQuat &mQuat::operator *= (const mQuat &b)
{
//	cout << "in mQuat *=" << endl;

    mQuat a(elt[0],elt[1],elt[2],elt[3]);
		a = a*b;
    for (int i = 0; i < 4; i++)
        elt[i] = a[i];

    return(SELF);
}

mQuat &mQuat::operator = (const Vec4 &v)
{
	for (int i = 0; i < 4; i++)
		elt[i] = v[i];
	return (SELF);
}

mQuat slerp (const mQuat& q1, const mQuat& q2, Real t)
{
	Real phi = acos(dot (q1, q2));
	Real denom = sin(phi);
	
	mQuat result( sin(phi*(1-t))/denom * q1 + sin(phi*t)/denom * q2);

	return result;
}
#endif
