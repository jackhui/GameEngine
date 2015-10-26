#ifndef POINT_H
#define POINT_H

#include "../Math/simdmath.h"
#include "collide.h"
#include "body.h"

class Point : public Body
{
public:
	Point(){}

	Point(const SIMDVector3& point)
	{
		m_pPoint = point;
		Body::setType(typePOINT);
	}

	SIMDVector3 getPoint() const { return m_pPoint; }
	virtual void translate(const SIMDVector3& translate);
	const SIMDVector3 getCenter();

private:
	SIMDVector3 m_pPoint;
};

#endif POINT_H