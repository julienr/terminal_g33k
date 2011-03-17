#include "tgvector3.h"

tgVector3 tgVector3::ZERO(0,0,0);
tgVector3 tgVector3::XNORM(1,0,0);
tgVector3 tgVector3::YNORM(0,1,0);
tgVector3 tgVector3::ZNORM(0,0,1);


bool tgVector3::operator == (const tgVector3& a) const
{
  return ( tgMath::FloatEq(a.x,x) && tgMath::FloatEq(a.y,y) && tgMath::FloatEq(a.z,z)); 
	//return (a.x == x && a.y == y && a.z == z);
}

