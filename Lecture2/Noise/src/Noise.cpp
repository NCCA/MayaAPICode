#include "Noise.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>


//----------------------------------------------------------------------------------------------------------------------
void Noise :: resetTables()
{
  // create an instance of the Mersenne Twister random number generator
  std::mt19937 gen(m_seed);
  // and create a RandFloat function
  std::uniform_real_distribution<float> randPosFloat(0.0f, 1.0f);
  // shuffle the index table randomly
  std::shuffle(std::begin(m_index), std::end(m_index), gen);

  for(auto &t : m_noiseTable)
  {
    t=randPosFloat(gen);
  }
}
//----------------------------------------------------------------------------------------------------------------------
Noise :: Noise()
{
  m_index.resize(256);
  int i=0;
  std::generate(std::begin(m_index), std::end(m_index), [&i]{ return i++; });
  m_noiseTable.resize( 256);
  resetTables();

}
//----------------------------------------------------------------------------------------------------------------------
Noise::~Noise()
{
  // using smart pointers so nothing to see here ;-)
}


//----------------------------------------------------------------------------------------------------------------------
float Noise::latticeNoise(int _i, int _j, int _k)
{
#define PERM(x) m_index[(x)&255]
#define INDEX(ix,iy,iz) PERM( (ix) + PERM((iy)+PERM(iz)))

return m_noiseTable[INDEX(_i,_j,_k)];


}

float Noise::noise(float _scale, const MPoint &_p)
{

  #define Lerp(F, A,B) A + F * ( B - A )
  float d[2][2][2];
  MPoint pp;
  pp.x=_p.x * _scale ;
  pp.y=_p.y * _scale ;
  pp.z=_p.z * _scale ;

  long ix = (long) pp.x;
  long iy = (long) pp.y;
  long iz = (long) pp.z;
  float tx,ty,tz,x0,x1,x2,x3,y0,y1;
  tx=pp.x-ix; ty=pp.y-iy; tz=pp.z-iz;


  for(int k=0; k<=1; ++k)
  {
    for(int j=0; j<=1; ++j)
    {
      for(int i=0; i<=1; ++i)
      {
        d[k][j][i]=latticeNoise(ix+i,iy+j,iz+k);
      }
    }
  }
  x0=Lerp(tx, d[0][0][0],d[0][0][1]);
  x1=Lerp(tx, d[0][1][0],d[0][1][1]);
  x2=Lerp(tx, d[1][0][0],d[1][0][1]);
  x3=Lerp(tx, d[1][1][0],d[1][1][1]);
  y0=Lerp(ty, x0,x1);
  y1=Lerp(ty, x2,x3);
  return Lerp(tz,y0,y1);

}



//----------------------------------------------------------------------------------------------------------------------
float Noise :: turbulance(float _scale, const MPoint &_p  )
{
  float val= (noise(_scale,_p)/2.0) +
             (noise(2.0*_scale,_p)/4.0) +
             (noise(4.0*_scale,_p)/8.0) +
             (noise(8.0*_scale,_p)/16.0);
  return val;
}

//----------------------------------------------------------------------------------------------------------------------
// values for this are based on http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
//----------------------------------------------------------------------------------------------------------------------

float Noise::complex( int _steps,float _persistence,float _scale, const MPoint &_p )
{
  float val=0.0;

  for(int i=1; i<=_steps; ++i)
  {
    val+=(noise(i*_scale,_p)/std::pow(_persistence,i));
  }
  return val;

}
//----------------------------------------------------------------------------------------------------------------------
