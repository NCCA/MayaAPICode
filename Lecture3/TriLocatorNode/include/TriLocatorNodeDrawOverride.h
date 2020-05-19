#ifndef TRILOCATORNODEDRAWOVERRIDE_H_
#define TRILOCATORNODEDRAWOVERRIDE_H_

#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>
#include <maya/MCallbackIdArray.h>


class TriLocatorNodeDrawOverride : public MHWRender::MPxDrawOverride
{
public:
	static MHWRender::MPxDrawOverride* Creator(const MObject& obj)
	{
		return new TriLocatorNodeDrawOverride(obj);
	}

	~TriLocatorNodeDrawOverride() override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;

	bool isBounded(
		const MDagPath& objPath,
		const MDagPath& cameraPath) const override;

	MBoundingBox boundingBox(
		const MDagPath& objPath,
		const MDagPath& cameraPath) const override;

	bool disableInternalBoundingBoxDraw() const override;

	MUserData* prepareForDraw(
		const MDagPath& objPath,
		const MDagPath& cameraPath,
		const MHWRender::MFrameContext& frameContext,
		MUserData* oldData) override;

	bool hasUIDrawables() const override { return true; }

	void addUIDrawables(
		const MDagPath& objPath,
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext,
		const MUserData* data) override;

protected:
	MBoundingBox mCurrentBoundingBox;

	MCallbackId fModelEditorChangedCbId;
	MObject fTriLocator;

private:
	TriLocatorNodeDrawOverride(const MObject& obj);
	float getMultiplier(const MDagPath& objPath) const;

	static void OnModelEditorChanged(void *clientData);
};


#endif