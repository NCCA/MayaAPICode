//----------------------------------------------------------------------------------------------------------------------
/// @brief
/// simple Perlin noise class cobbled together from Computer Graphics OpenGL by F,S Hill
/// and Texturing and Modeling Ebert et-al
/// also thanks to Ian Stephenson for help and debuging tips
/// more work needs to be done to add interpolated noise functions and improve the
/// aliasing of the textures but it is ok for basic use
//----------------------------------------------------------------------------------------------------------------------


#ifndef _NOISE_H_
#define _NOISE_H_

#include <maya/MPoint.h>

//----------------------------------------------------------------------------------------------------------------------
/// @class Noise a simple Perlin noise class
/// @author Jon Macey
/// @version 1.0
/// @date 19/4/12
//----------------------------------------------------------------------------------------------------------------------

class Noise
{
public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a ctor dynamically allocates two tables
  //----------------------------------------------------------------------------------------------------------------------
  Noise();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor will remove tables allocated by dtor
  //----------------------------------------------------------------------------------------------------------------------
  ~Noise();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a noise function to return a float based on input point and scale
  /// @param [in] _scale the scale to process the noise with
  /// @param [in] _p the point to sample noise from
  /// @brief returns a noise value
  //----------------------------------------------------------------------------------------------------------------------
  float noise(
              float _scale,
              const MPoint &_p
             );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief turbulance function creates higher frequency versions of noise as harmonics
  /// @param [in] _scale the scale to process the noise with
  /// @param [in] _p the point to sample noise from
  /// @brief returns a noise value
  //----------------------------------------------------------------------------------------------------------------------
  float turbulance(
                    float _scale,
                    const MPoint &_p
                  );
  float complex(
                int _steps,
                float _persistence,
                float _scale,
                const MPoint &_p
               );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief reset the noise tables will also re-set the seed so must be called after setSeed is
  /// called
  //----------------------------------------------------------------------------------------------------------------------
  void resetTables();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the seed of the random number generator
  //----------------------------------------------------------------------------------------------------------------------
  inline void setSeed(int _s){m_seed=_s;}


private :

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief noise table used for the noise generation
  //----------------------------------------------------------------------------------------------------------------------
  float *m_noiseTable;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief index into the noise table
  //----------------------------------------------------------------------------------------------------------------------
  unsigned char * m_index;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief random number generator seed (default to 1)
  //----------------------------------------------------------------------------------------------------------------------
  unsigned int m_seed;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief function to generate latticeNoise (from Ian Stephenson)
  /// @param [in] _i index into table
  /// @param [in] _j index into table
  /// @param [in] _k index into table

  //----------------------------------------------------------------------------------------------------------------------
  float latticeNoise(int _i, int _j, int _k);

};



#endif
