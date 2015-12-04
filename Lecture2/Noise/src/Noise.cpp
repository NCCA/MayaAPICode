#include "Noise.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <boost/random.hpp>


//----------------------------------------------------------------------------------------------------------------------
void Noise :: resetTables()
{
  int i;

  // create an instance of the Mersenne Twister random number generator
  boost::mt19937 Generator;

  // now create a random number generator distrib float from 0.0 - 1.0 for Colour
  boost::uniform_int<int> distibution(0, 256);
  // and create a RandFloat function
  boost::variate_generator<boost::mt19937 &, boost::uniform_int<int> >randomPositiveNumber(Generator, distibution);
  boost::uniform_real<float> table(0.0, 1.0);
  // and create a RandFloat function
  boost::variate_generator<boost::mt19937 &, boost::uniform_real<float> >randomPositiveNumberTable(Generator, table);
  setSeed(m_seed);

  for(i=0; i<256; ++i)
  {
    int which=int(randomPositiveNumber());
    unsigned char tmp=m_index[which];
    m_index[which]=m_index[i];
    m_index[i]=tmp;
  }

  for(i=0; i<256; ++i)
  {
    m_noiseTable[i]=randomPositiveNumberTable();
  }

}

//----------------------------------------------------------------------------------------------------------------------
Noise :: Noise()
{
  int i;
  m_seed=1;
  m_index = new unsigned char[256];
  for(i=0; i<256; ++i)
  {
    m_index[i]=i;
  }
  m_noiseTable = new float[256];
  resetTables();

}
//----------------------------------------------------------------------------------------------------------------------
Noise::~Noise()
{
  delete m_index;
  delete m_noiseTable;
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
