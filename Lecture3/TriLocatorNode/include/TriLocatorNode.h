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
    static  MObject         size;         // The size of the foot
  public: 
    static	MTypeId		id;
    static  MString		drawDbClassification;
    static  MString		drawRegistrantId;
};

