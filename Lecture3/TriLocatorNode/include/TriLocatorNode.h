#include <maya/MPxNode.h>   
#include <maya/MPxLocatorNode.h> 
#include <maya/MString.h> 

class TriLocatorNode : public MPxLocatorNode
{
  public:
    TriLocatorNode()=default;
    ~TriLocatorNode()=default ; 
    MStatus   		compute(const MPlug& plug, MDataBlock &data) override;
    bool            isBounded() const override;
    MBoundingBox    boundingBox() const override; 
    static  void *          creator();
    static  MStatus         initialize();
    static  MObject         m_size;         // The size of the triangle
  public: 
    static	MTypeId		s_id;
    static  MString		s_drawDbClassification;
    static  MString		s_drawRegistrantId;
};

