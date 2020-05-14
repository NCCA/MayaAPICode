#include "CubeLocatorNodeDrawOverride.h"

CubeLocatorDrawOverride::CubeLocatorDrawOverride(const MObject& obj)
  : MHWRender::MPxDrawOverride(obj, NULL)
{
  m_ModelEditorChangedCbId = MEventMessage::addEventCallback(
    "modelEditorChanged", OnModelEditorChanged, this);

  MStatus status;
  MFnDependencyNode node(obj, &status);
  m_locator = status ? dynamic_cast<CubeLocatorNode*>(node.userNode()) : NULL;

}

CubeLocatorDrawOverride::~CubeLocatorDrawOverride()
{
  m_locator = nullptr;

  if (m_ModelEditorChangedCbId != 0)
  {
    MMessage::removeCallback(m_ModelEditorChangedCbId);
    m_ModelEditorChangedCbId = 0;
  }
}

MHWRender::DrawAPI CubeLocatorDrawOverride::supportedDrawAPIs() const
{
  // this plugin supports both GL and DX
  return (MHWRender::kOpenGL | MHWRender::kDirectX11);
}


bool CubeLocatorDrawOverride::isBounded(const MDagPath& /*objPath*/,
                    const MDagPath& /*cameraPath*/) const
{
  return true;
}

MBoundingBox CubeLocatorDrawOverride::boundingBox(const MDagPath& objPath,const MDagPath& cameraPath) const
{
  //MPoint size=getDimensions();
  MPoint corner1( -1, 0.0, -1 );
  MPoint corner2( 1, 0.0, 1 );


  CubeLocatorDrawOverride *nonConstThis = (CubeLocatorDrawOverride *)this;
  nonConstThis->mCurrentBoundingBox.clear();
  nonConstThis->mCurrentBoundingBox.expand( corner1 );
  nonConstThis->mCurrentBoundingBox.expand( corner2 );

  return mCurrentBoundingBox;
}

bool CubeLocatorDrawOverride::disableInternalBoundingBoxDraw() const
{
  return false;
}

void CubeLocatorDrawOverride::OnModelEditorChanged(void *clientData)
{
  // Mark the node as being dirty so that it can update on display appearance
  // switch among wireframe and shaded.
  CubeLocatorDrawOverride *ovr = static_cast<CubeLocatorDrawOverride*>(clientData);
//  if (ovr && ovr->m_verteces)
//  {
//    MHWRender::MRenderer::setGeometryDrawDirty(ovr->fFootPrint->thisMObject());
//  }
}

MUserData* CubeLocatorDrawOverride::prepareForDraw(
  const MDagPath& objPath,
  const MDagPath& cameraPath,
  const MHWRender::MFrameContext& frameContext,
  MUserData* oldData)
{
  // Retrieve data cache (create if does not exist)
  CubeLocatorNodeData* data = dynamic_cast<CubeLocatorNodeData*>(oldData);
  if (!data)
  {
    data = new CubeLocatorNodeData();
  }
  MPoint dimension=getDimensions(objPath);
  char string[40];
  sprintf(string,"Dim [%f %f %f]",dimension.x,dimension.y,dimension.z);
  MGlobal::displayInfo(string);

/*
  float fMultiplier = getMultiplier(objPath);

  data->fSoleLineList.clear();
  for (int i = 0; i < soleCount; i++)
  {
    data->fSoleLineList.append(sole[i][0] * fMultiplier, sole[i][1] * fMultiplier, sole[i][2] * fMultiplier);
  }

  data->fHeelLineList.clear();
  for (int i = 0; i < heelCount; i++)
  {
    data->fHeelLineList.append(heel[i][0] * fMultiplier, heel[i][1] * fMultiplier, heel[i][2] * fMultiplier);
  }

  data->fSoleTriangleList.clear();
  for (int i = 1; i <= soleCount - 2; i++)
  {
    data->fSoleTriangleList.append(sole[0][0] * fMultiplier, sole[0][1] * fMultiplier, sole[0][2] * fMultiplier);
    data->fSoleTriangleList.append(sole[i][0] * fMultiplier, sole[i][1] * fMultiplier, sole[i][2] * fMultiplier);
    data->fSoleTriangleList.append(sole[i+1][0] * fMultiplier, sole[i+1][1] * fMultiplier, sole[i+1][2] * fMultiplier);
  }

  data->fHeelTriangleList.clear();
  for (int i = 1; i <= heelCount - 2; i++)
  {
    data->fHeelTriangleList.append(heel[0][0] * fMultiplier, heel[0][1] * fMultiplier, heel[0][2] * fMultiplier);
    data->fHeelTriangleList.append(heel[i][0] * fMultiplier, heel[i][1] * fMultiplier, heel[i][2] * fMultiplier);
    data->fHeelTriangleList.append(heel[i+1][0] * fMultiplier, heel[i+1][1] * fMultiplier, heel[i+1][2] * fMultiplier);
  }

*/
  // compute data and cache it
  data->fColor = MHWRender::MGeometryUtilities::wireframeColor(objPath);

  return data;
}

void CubeLocatorDrawOverride::addUIDrawables(
  const MDagPath& objPath,
  MHWRender::MUIDrawManager& drawManager,
  const MHWRender::MFrameContext& frameContext,
  const MUserData* data)
{
  CubeLocatorNodeData* pLocatorData = (CubeLocatorNodeData*)data;

  drawManager.beginDrawable();

  // Draw the foot print solid/wireframe
  drawManager.setColor( pLocatorData->fColor );
  drawManager.setDepthPriority(5);

  // Draw a text "Foot"
  MPoint pos( 0.0, 0.0, 0.0 ); // Position of the text
  MColor textColor( 0.1f, 0.8f, 0.8f, 1.0f ); // Text color

  drawManager.setColor( textColor );
  drawManager.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );
  drawManager.text( pos,  MString("Cube"), MHWRender::MUIDrawManager::kCenter );

  drawManager.endDrawable();


/*
  if (frameContext.getDisplayStyle() & MHWRender::MFrameContext::kGouraudShaded) {
    drawManager.mesh(MHWRender::MUIDrawManager::kTriangles, pLocatorData->fSoleTriangleList);
    drawManager.mesh(MHWRender::MUIDrawManager::kTriangles, pLocatorData->fHeelTriangleList);
  }

  drawManager.mesh(MHWRender::MUIDrawManager::kClosedLine, pLocatorData->fSoleLineList);
  drawManager.mesh(MHWRender::MUIDrawManager::kClosedLine, pLocatorData->fHeelLineList);

  // Draw a text "Foot"
  MPoint pos( 0.0, 0.0, 0.0 ); // Position of the text
  MColor textColor( 0.1f, 0.8f, 0.8f, 1.0f ); // Text color

  drawManager.setColor( textColor );
  drawManager.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );
  drawManager.text( pos,  MString("FootprintLocator"), MHWRender::MUIDrawManager::kCenter );

  drawManager.endDrawable();*/
}


MFloatPoint CubeLocatorDrawOverride::getDimensions(const MDagPath& objPath) const
{
 MFloatPoint p(1.0f,1.0f,1.0f);
 // Retrieve value of the size attribute from the node
 MStatus status;
 MObject cubleLocatorNode = objPath.node(&status);

 if (status)
 {
   float sizeVal;
   MPlug width(cubleLocatorNode, CubeLocatorNode::m_width);
   if (width.getValue(sizeVal))
   {
    p.x=sizeVal;
   }
   MPlug height(cubleLocatorNode, CubeLocatorNode::m_height);
   if (height.getValue(sizeVal))
   {
    p.y=sizeVal;
   }
   MPlug depth(cubleLocatorNode, CubeLocatorNode::m_depth);
   if (depth.getValue(sizeVal))
   {
    p.z=sizeVal;
   }

 }
 return p;

}
