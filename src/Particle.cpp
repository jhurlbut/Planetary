#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;
using std::vector;

Particle::Particle()
{
}

Particle::Particle( int index, Vec3f pos, Vec3f vel )
{
	mIndex			= index;
	mColor			= ColorA( 1.0f, 1.0f, 1.0f, 1.0f );
	
	mLifespan       = Rand::randInt( 35, 100 );
	mIsDead			= false;
	
	setup();
}

void Particle::setup()
{
	mPos		= Rand::randVec3f() * 0.4f;
	mPrevPos	= mPos;
	mVel		= mPos * Rand::randFloat( 0.00125f, 0.003f );
	mAcc		= Rand::randVec3f() * 0.01f;
	mAge		= 0;
	mAgePer		= 0.0f;
	mRadius		= Rand::randFloat( 0.025f, 0.125f );
}

void Particle::pullToCenter( Node *trackNode )
{
	if( trackNode ){
		Vec3f dirToCenter = mPos - trackNode->mRelPos;
		float distToCenterSqrd = dirToCenter.lengthSquared();
		
		if( distToCenterSqrd > 0.00001f ){
			dirToCenter.normalize();
			float pullStrength = 1.75f;
			mVel -= dirToCenter * ( ( distToCenterSqrd - 0.00001f ) * pullStrength );
		}
	}
}

void Particle::update()
{
	mAgePer = 1.0f - (float)mAge/(float)mLifespan;
	mPrevPos = mPos;
    mPos += mVel;
    mAge ++;
    if( mAge > mLifespan ){
        setup();		
    }
}

void Particle::draw()
{
    gl::drawLine( mPos, mPrevPos );
}

void Particle::drawScreenspace( const Matrix44f &mat, const Vec3f &sUp, const Vec3f &sRight )
{
	gl::pushModelView();
	gl::translate( mat * mPos );
	float r = mAgePer * mRadius;
	gl::drawBillboard( Vec3f::zero(), Vec2f( r, r ), 0.0f, sUp, sRight );
	gl::popModelView();
}

