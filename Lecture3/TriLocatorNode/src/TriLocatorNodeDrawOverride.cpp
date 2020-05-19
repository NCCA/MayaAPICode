#include "TriLocatorNodeDrawOverride.h"
#include "TriLocatorNode.h"
#include "TriLocatorNodeData.h"
#include <maya/MEventMessage.h>
#include <maya/MDistance.h>
#include <array>

// Tri Data
//       V2 
//
//  V1       V3

constexpr static float scale = 0.5f;
constexpr static std::array<float[3],3> triangle = 
							 {  -scale, -scale, 0.0f,
							    0.0f,   scale, 0.0f ,
							    scale, -scale, 0.0f };


// By setting isAlwaysDirty to false in MPxDrawOverride constructor, the
// draw override will be updated (via prepareForDraw()) only when the node
// is marked dirty via DG evaluation or dirty propagation. Additional
// callback is also added to explicitly mark the node as being dirty (via
// MRenderer::setGeometryDrawDirty()) for certain circumstances. Note that
// the draw callback in MPxDrawOverride constructor is set to NULL in order
// to achieve better performance.
TriLocatorNodeDrawOverride::TriLocatorNodeDrawOverride(const MObject& obj): MHWRender::MPxDrawOverride(obj, NULL, false), fTriLocator(obj)
{
	fModelEditorChangedCbId = MEventMessage::addEventCallback(
		"modelEditorChanged", OnModelEditorChanged, this);
}

TriLocatorNodeDrawOverride::~TriLocatorNodeDrawOverride()
{
	if (fModelEditorChangedCbId != 0)
	{
		MMessage::removeCallback(fModelEditorChangedCbId);
		fModelEditorChangedCbId = 0;
	}
}

void TriLocatorNodeDrawOverride::OnModelEditorChanged(void *clientData)
{
	// Mark the node as being dirty so that it can update on display mode switch,
	// e.g. between wireframe and shaded.
	TriLocatorNodeDrawOverride *ovr = static_cast<TriLocatorNodeDrawOverride*>(clientData);
	if (ovr) MHWRender::MRenderer::setGeometryDrawDirty(ovr->fTriLocator);
}

MHWRender::DrawAPI TriLocatorNodeDrawOverride::supportedDrawAPIs() const
{
	return MHWRender::kAllDevices;
}

float TriLocatorNodeDrawOverride::getMultiplier(const MDagPath& objPath) const
{
	// Retrieve value of the size attribute from the node
	MStatus status;
	MObject TriLocatorNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(TriLocatorNode, TriLocatorNode::size);
		if (!plug.isNull())
		{
			MDistance sizeVal;
			if (plug.getValue(sizeVal))
			{
				return (float)sizeVal.asCentimeters();
			}
		}
	}

	return 1.0f;
}

bool TriLocatorNodeDrawOverride::isBounded(const MDagPath& /*objPath*/,
									  const MDagPath& /*cameraPath*/) const
{
	return true;
}

MBoundingBox TriLocatorNodeDrawOverride::boundingBox(
	const MDagPath& objPath,
	const MDagPath& cameraPath) const
{
	MPoint corner1( -scale, scale, 0.0f );
	MPoint corner2( scale, -scale, 0.0f );

	float multiplier = getMultiplier(objPath);
	corner1 = corner1 * multiplier;
	corner2 = corner2 * multiplier;

	TriLocatorNodeDrawOverride *nonConstThis = (TriLocatorNodeDrawOverride *)this;
	nonConstThis->mCurrentBoundingBox.clear();
	nonConstThis->mCurrentBoundingBox.expand( corner1 );
	nonConstThis->mCurrentBoundingBox.expand( corner2 );

	return mCurrentBoundingBox;
}

bool TriLocatorNodeDrawOverride::disableInternalBoundingBoxDraw() const
{
	return false;
}

MUserData* TriLocatorNodeDrawOverride::prepareForDraw(const MDagPath& objPath,const MDagPath& cameraPath,const MHWRender::MFrameContext& frameContext,MUserData* oldData)
{
	// Retrieve data cache (create if does not exist)
	TriLocatorNodeData* data = dynamic_cast<TriLocatorNodeData*>(oldData);
	if (!data)
	{
		data = new TriLocatorNodeData();
	}

	float fMultiplier = getMultiplier(objPath);

	data->fLineList.clear();
	// line 1
	data->fLineList.append(triangle[0][0] * fMultiplier, triangle[0][1] * fMultiplier, triangle[0][2] * fMultiplier);
	data->fLineList.append(triangle[1][0] * fMultiplier, triangle[1][1] * fMultiplier, triangle[1][2] * fMultiplier);

	// line 2
	data->fLineList.append(triangle[1][0] * fMultiplier, triangle[1][1] * fMultiplier, triangle[1][2] * fMultiplier);
	data->fLineList.append(triangle[2][0] * fMultiplier, triangle[2][1] * fMultiplier, triangle[2][2] * fMultiplier);

	// line 3
	data->fLineList.append(triangle[2][0] * fMultiplier, triangle[2][1] * fMultiplier, triangle[2][2] * fMultiplier);
	data->fLineList.append(triangle[0][0] * fMultiplier, triangle[0][1] * fMultiplier, triangle[0][2] * fMultiplier);

	data->fTriangleList.clear();

	data->fTriangleList.append(triangle[0][0] * fMultiplier, triangle[0][1] * fMultiplier, triangle[0][2] * fMultiplier);
	data->fTriangleList.append(triangle[1][0] * fMultiplier, triangle[1][1] * fMultiplier, triangle[1][2] * fMultiplier);
	data->fTriangleList.append(triangle[2][0] * fMultiplier, triangle[2][1] * fMultiplier, triangle[2][2] * fMultiplier);
	// per vertex normals
		
	MVector normal(0.0f, 0.0f, 1.0f);
	data->fNormalsList->clear();
	data->fNormalsList->append(normal);
	data->fNormalsList->append(normal);
	data->fNormalsList->append(normal);
	data->fColours->clear();
	data->fColours->append(MColor(1.0f, 0.0f, 0.0f));
	data->fColours->append(MColor(0.0f, 1.0f, 0.0f));
	data->fColours->append(MColor(0.0f, 0.0f, 1.0f));


	// compute data and cache it
	data->fColor = MHWRender::MGeometryUtilities::wireframeColor(objPath);

	return data;
}

void TriLocatorNodeDrawOverride::addUIDrawables(const MDagPath& objPath,MHWRender::MUIDrawManager& drawManager,const MHWRender::MFrameContext& frameContext,const MUserData* data)
{
	//TriLocatorNodeData* pLocatorData = (TriLocatorNodeData*)data;

	auto pLocatorData = static_cast< const TriLocatorNodeData *>(data);

	drawManager.beginDrawable();

	// Draw the tri print solid/wireframe
	drawManager.setColor( pLocatorData->fColor );
	drawManager.setDepthPriority(5);

	if (frameContext.getDisplayStyle() & MHWRender::MFrameContext::kGouraudShaded) 
	{
		drawManager.mesh(MHWRender::MUIDrawManager::kTriangles, pLocatorData->fTriangleList, pLocatorData->fNormalsList, pLocatorData->fColours);
	}

	drawManager.mesh(MHWRender::MUIDrawManager::kLineStrip, pLocatorData->fLineList, pLocatorData->fNormalsList, pLocatorData->fColours);


	drawManager.endDrawable();
}
