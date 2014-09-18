#include "RibExport.h"
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>

#include <iostream>

//-------------------------------------------------------------------

/// specifies a script to use for the user interface options box
const char *g_OptionScript = "RibExportScript";

/// a set of default options for the exporter
const char *g_DefaultOptions = "";


//-------------------------------------------------------------------	initializePlugin
///	\brief	initializePlugin( MObject obj )
///	\param	obj		-	the plugin handle
///	\return	MS::kSuccess if ok
///	\note	Registers all of the new commands, file translators and new
///			node types.
///
MStatus initializePlugin( MObject obj )
{
  MStatus status;

  MFnPlugin plugin( obj, "Jon Macey", "1.0", "Any");

  // Register the translator with the system
  status =  plugin.registerFileTranslator( "RibExport", "none",
    RibExport::creator,
    (char*)g_OptionScript,
    (char*)g_DefaultOptions
     );

  if (status != MS::kSuccess) {
    status.perror("MayaExportCommand::registerFileTranslator");
  }
  return status;

}


//-------------------------------------------------------------------	uninitializePlugin
///	\brief	uninitializePlugin( MObject obj )
///	\param	obj	-	the plugin handle to un-register
///	\return	MS::kSuccess if ok
///	\note	un-registers the plugin and destroys itself
///
MStatus uninitializePlugin( MObject obj )
{
  MFnPlugin plugin( obj );
  MStatus status =  plugin.deregisterFileTranslator( "RibExport" );
  if (status != MS::kSuccess) {
    status.perror("MayaExportCommand::deregisterFileTranslator");
  }

  return status;
}

