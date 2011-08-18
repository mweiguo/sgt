#ifndef _SLC_IMPORTER_H_
#define _SLC_IMPORTER_H_

#include <COLLADAFWIWriter.h>
#include <COLLADAFWPrerequisites.h>
#include <COLLADABUURI.h>
#include <string>
using namespace std;

using namespace COLLADAFW;

namespace COLLADAFW
{
    class Node;
}
class SLCImporter : public COLLADAFW::IWriter
{
    COLLADABU::URI mInputFile;
public:
    void import ( const char* daeFileName );
    /** This method will be called if an error in the loading process occurred and the loader cannot
	continue to to load. The writer should undo all operations that have been performed.
	@param errorMessage A message containing informations about the error that occurred.
    */
    virtual void cancel(const String& errorMessage);

    /** This is the method called. The writer hast to prepare to receive data.*/
    virtual void start();

    /** This method is called after the last write* method. No other methods will be called after this.*/
    virtual void finish();

    /** When this method is called, the writer must write the global document asset.
        @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeGlobalAsset ( const FileInfo* asset );

    /** When this method is called, the writer must write the scene.
        @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeScene ( const Scene* scene );

    /** When this method is called, the writer must write the entire visual scene.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeVisualScene ( const VisualScene* visualScene );

    /** When this method is called, the writer must handle all nodes contained in the 
	library nodes.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeLibraryNodes ( const LibraryNodes* libraryNodes );

    /** When this method is called, the writer must write the geometry.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeGeometry ( const Geometry* geometry );

    /** When this method is called, the writer must write the material.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeMaterial( const Material* material );

    /** When this method is called, the writer must write the effect.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeEffect( const Effect* effect );

    /** When this method is called, the writer must write the camera.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeCamera( const Camera* camera );

    /** When this method is called, the writer must write the image.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeImage( const Image* image );

    /** When this method is called, the writer must write the light.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeLight( const Light* light );

    /** When this method is called, the writer must write the Animation.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeAnimation( const Animation* animation );

    /** When this method is called, the writer must write the AnimationList.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeAnimationList( const AnimationList* animationList );

    /** When this method is called, the writer must write the skin controller data.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeSkinControllerData( const SkinControllerData* skinControllerData );

    /** When this method is called, the writer must write the controller.
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeController( const Controller* controller );

    /** When this method is called, the writer must write the formulas. All the formulas of the entire
	COLLADA file are contained in @a formulas.
        @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeFormulas( const Formulas* formulas );

    /** When this method is called, the writer must write the kinematics scene. 
	@return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeKinematicsScene( const KinematicsScene* kinematicsScene );

    string writeNode ( const COLLADAFW::Node* node );

};

#endif // _SLC_IMPORTER_H_
