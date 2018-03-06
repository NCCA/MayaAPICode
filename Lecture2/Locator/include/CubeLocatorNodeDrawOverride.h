#ifndef CUBENODEDATAOVERRIDE_H_
#define CUBENODEDATAOVERRIDE_H_
// Viewport 2.0
#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>
#include <maya/MEventMessage.h>
#include "CubeLocatorNode.h"

class CubeLocatorNodeData : public MUserData
{
public:
  CubeLocatorNodeData() : MUserData(false) {} // don't delete after draw
  virtual ~CubeLocatorNodeData() {}

  MColor fColor;
  MPointArray m_vertices;
  MPointArray m_normals;
};

class CubeLocatorDrawOverride : public MHWRender::MPxDrawOverride
{
public:
  static MHWRender::MPxDrawOverride* Creator(const MObject& obj)
  {
    return new CubeLocatorDrawOverride(obj);
  }

  virtual ~CubeLocatorDrawOverride();

  virtual MHWRender::DrawAPI supportedDrawAPIs() const;

  virtual bool isBounded(const MDagPath& objPath,const MDagPath& cameraPath) const;

  virtual MBoundingBox boundingBox( const MDagPath& objPath, const MDagPath& cameraPath) const;

  virtual bool disableInternalBoundingBoxDraw() const;

  virtual MUserData* prepareForDraw(
    const MDagPath& objPath,
    const MDagPath& cameraPath,
    const MHWRender::MFrameContext& frameContext,
    MUserData* oldData);

  virtual bool hasUIDrawables() const { return true; }

  virtual void addUIDrawables(
    const MDagPath& objPath,
    MHWRender::MUIDrawManager& drawManager,
    const MHWRender::MFrameContext& frameContext,
    const MUserData* data);

protected:
  MBoundingBox mCurrentBoundingBox;

private:
  CubeLocatorDrawOverride(const MObject& obj);
  MFloatPoint getDimensions(const MDagPath& objPath) const;
  MCallbackId m_ModelEditorChangedCbId;
  static void OnModelEditorChanged(void *clientData);
  CubeLocatorNode *m_locator;


};


#endif
