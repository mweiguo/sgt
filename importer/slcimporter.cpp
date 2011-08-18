#include "slcimporter.h"
#include <iostream>
#include <COLLADASaxFWLLoader.h>
#include <COLLADAFW.h>
#include <iostream>

using namespace std;

void SLCImporter::import ( const char* daeFileName )
{
    COLLADASaxFWL::Loader loader;
    cout << "COLLADASaxFWL::Loader loader;" << endl;
    COLLADAFW::Root root(&loader, this);
    cout << "COLLADAFW::Root root(&loader, this);" << endl;

    // Load scene graph 
    mInputFile = COLLADABU::URI::nativePathToUri(daeFileName);
    cout << "root.loadDocument( " << mInputFile.toNativePath() << ")" << endl;
    if ( !root.loadDocument(mInputFile.toNativePath()) ) {
	cout << "load failed" << endl;
	return;
    }
    cout << "load ok" << endl;
}

void SLCImporter::cancel(const String& errorMessage)
{
    cout << "cancel " << errorMessage << endl;
}

void SLCImporter::start()
{
    cout << "start" << endl;
}

void SLCImporter::finish()
{
    cout << "finish" << endl;
}

bool SLCImporter::writeGlobalAsset ( const FileInfo* asset )
{
    stringstream ss;
    ss << "writeGlobalAsset" << endl;
    ss << "    UpAxisType = ";
    switch ( asset->getUpAxisType() )
    {
    case FileInfo::NONE:
	ss << "NONE";
	break;
    case FileInfo::X_UP:
	ss << "X_UP";
	break;
    case FileInfo::Y_UP:
	ss << "Y_UP";
	break;
    case FileInfo::Z_UP:
	ss << "Z_UP";
	break;
    }
    ss << endl;
    ss << "    AbsoluteFileUri = " << asset->getAbsoluteFileUri().getURIString() << endl;
    const FileInfo::Unit& unit = asset->getUnit();
    ss << "    LinearUnitName = " << unit.getLinearUnitName () << endl;
    ss << "    LinearUnitMeter = " << unit.getLinearUnitMeter () << endl;
    ss << "    LinearUnitUnit = " << unit.getLinearUnitUnit () << endl;
    ss << "    AngularUnitName = " << unit.getAngularUnitName () << endl;
    ss << "    AngularUnit = " << unit.getAngularUnit () << endl;
    ss << "    TimeUnitName = " << unit.getTimeUnitName () << endl;
    const FileInfo::ValuePairPointerArray& array = asset->getValuePairArray ();
    ss << "    array" << endl;
    for ( int i=0; i<array.getCount(); i++ )
	ss << "        (" << array[i]->first << "," << array[i]->second << ")" << endl;
    cout << ss.str();
    
    return true;
}

bool SLCImporter::writeScene ( const Scene* scene )
{
    stringstream ss;
    ss << "writeScene" << endl;
    const InstanceVisualScene* inst =  scene->getInstanceVisualScene ();
    ss << "\tName = " << inst->getName() << endl;
    const UniqueId& uid = inst->getInstanciatedObjectId ();
    ss << "\tUniqueId   = " << uid.toAscii() << endl;
    cout << ss.str();
    return true;
}

string SLCImporter::writeNode ( const COLLADAFW::Node* node )
{
    stringstream ss;
    ss << "\t\tOriginalId = " << node->getOriginalId () << endl;
    ss << "\t\tName = " << node->getName () << endl;
    switch ( node->getType () )
    {
    case Node::NODE:
	ss << "\t\tType = NODE" << endl;
	break;
    case Node::JOINT:
	ss << "\t\tType = JOINT" << endl;
	break;
    }
    const TransformationPointerArray& tparray = node->getTransformations ();
    if ( tparray.getCount() != 0 )
	ss << "\t\tTransformations : " << endl;
    for ( int i=0; i<tparray.getCount(); i++ )
    {
	const Transformation* t = tparray[i];
	switch ( t->getTransformationType() )
	{
	case Transformation::MATRIX:
	    ss << "\t\t\tTransformationType = MATRIX" << endl;
	    break;
	case Transformation::TRANSLATE:
	    ss << "\t\t\tTransformationType = TRANSLATE" << endl;
	    break;
	case Transformation::ROTATE:
	    ss << "\t\t\tTransformationType = ROTATE" << endl;
	    break;
	case Transformation::SCALE:
	    ss << "\t\t\tTransformationType = SCALE" << endl;
	    break;
	case Transformation::LOOKAT:
	    ss << "\t\t\tTransformationType = LOOKAT" << endl;
	    break;
	case Transformation::SKEW:
	    ss << "\t\t\tTransformationType = SKEW" << endl;
	    break;
	}
    }
    const InstanceGeometryPointerArray& gparray = node->getInstanceGeometries ();
    if ( gparray.getCount() != 0 )
	ss << "\t\tInstanceGeometries : " << endl;
    for ( int i=0; i<gparray.getCount(); i++ )
    {
        const InstanceGeometry::MaterialBindingArray& mbarray = gparray[i]->getMaterialBindings();
	if ( mbarray.getCount() != 0 )
	    ss << "\t\t\tMaterialBinding : " << endl;
	for ( int j=0; j<mbarray.getCount(); j++ )
	{
	    const InstanceGeometry::MaterialBinding& mb = mbarray[j];
	    ss << "\t\t\t\tMaterialId = " << mb.getMaterialId () << endl;
	    ss << "\t\t\t\tReferencedMaterial = " << mb.getReferencedMaterial ().toAscii() << endl;
	    ss << "\t\t\t\tName = " << mb.getName () << endl;
	    const InstanceGeometry::TextureCoordinateBindingArray& tcbarray = mb.getTextureCoordinateBindingArray ();
	    for ( int k=0; k<tcbarray.getCount(); k++ )
	    {
		const InstanceGeometry::TextureCoordinateBinding& tcb = tcbarray[k];
		ss << "\t\t\t\t\ttextureMapId = " << tcb.textureMapId << endl;
		ss << "\t\t\t\t\tsetIndex = " << tcb.setIndex << endl;
	    }
	}
    }

    const InstanceNodePointerArray& nparray = node->getInstanceNodes ();
    if ( gparray.getCount() != 0 )
	ss << "\t\tInstanceNodes : " << endl;
    for ( int i=0; i<nparray.getCount(); i++ )
    {
	const InstanceNode* n = nparray[i];
	ss << "\t\t\tName = " << n->getName() << endl;
	ss << "\t\t\tInstanciatedObjectId = " << n->getInstanciatedObjectId().toAscii() << endl;
    }
	
    const InstanceCameraPointerArray& cpa = node->getInstanceCameras ();
    if ( cpa.getCount() != 0 )
	ss << "\t\tInstanceCameras : " << endl;
    for ( int i=0; i<cpa.getCount(); i++ )
    {
	const InstanceCamera* c = cpa[i];
	ss << "\t\t\tName = " << c->getName() << endl;
	ss << "\t\t\tInstanciatedObjectId = " << c->getInstanciatedObjectId().toAscii() << endl;
    }

    const InstanceLightPointerArray& lpa = node->getInstanceLights ();
    if ( lpa.getCount() != 0 )
	ss << "\t\tInstanceLights : " << endl;
    for ( int i=0; i<lpa.getCount(); i++ )
    {
	const InstanceLight* l = lpa[i];
	ss << "\t\t\tName = " << l->getName() << endl;
	ss << "\t\t\tInstanciatedObjectId = " << l->getInstanciatedObjectId().toAscii() << endl;
    }

// todo
// 	const InstanceControllerPointerArray& ctrlpa = getInstanceControllers ();
// 	for ( int i=0; i<ctrlpa.getCount(); i++ )
// 	{
// 	    const InstanceController& c = ctrlpa[i];
// 	    ss << "\t\t\tName = " << c->getName() << endl;
// 	    ss << "\t\t\tInstanciatedObjectId = " << c->getInstanciatedObjectId().toAscii() << endl;
// 	}

    const NodePointerArray& npa = node->getChildNodes();
    if ( npa.getCount() != 0 )
	ss << "\t\tNodes : " << endl;
    for ( int i=0; i<npa.getCount(); i++ )
	ss << writeNode ( npa[i] );

    COLLADABU::Math::Matrix4 mat = node->getTransformationMatrix();
    ss << "\t\tmat = ";
    for ( int i=0; i<16; i++ )
	ss << mat.getElement( i ) << ", ";
    ss << endl;
	
    return ss.str();
}

bool SLCImporter::writeVisualScene ( const VisualScene* visualScene )
{
    stringstream ss;
    ss << "writeVisualScene" << endl;
    ss << "\tName = " << visualScene->getName() << endl;
    const NodePointerArray& array = visualScene->getRootNodes ();
    if ( array.getCount() != 0 )
	ss << "\tNodes : " << endl;
    for ( int i=0; i<array.getCount(); i++ )
	ss << writeNode ( array[i] );

    cout << ss.str();
    return true;
}

bool SLCImporter::writeLibraryNodes ( const LibraryNodes* libraryNodes )
{
    stringstream ss;
    ss << "writeLibraryNodes" << endl;
    return true;
}

bool SLCImporter::writeGeometry ( const Geometry* geometry )
{
    stringstream ss;
    ss << "writeGeometry" << endl;
    ss << "\tOriginalId = " << geometry->getOriginalId () << endl;
    ss << "\tName = " << geometry->getName () << endl;
    switch ( geometry->getType () )
    {
    case Geometry::GEO_TYPE_MESH:
	ss << "\tType = GEO_TYPE_MESH" << endl;
	break;
    case Geometry::GEO_TYPE_SPLINE:
	ss << "\tType = GEO_TYPE_SPLINE" << endl;
	break;
    case Geometry::GEO_TYPE_CONVEX_MESH:
	ss << "\tType = GEO_TYPE_CONVEX_MESH" << endl;
	break;
    case Geometry::GEO_TYPE_UNKNOWN:
	ss << "\tType = GEO_TYPE_UNKNOWN" << endl;
	break;
    }
    cout << ss.str();
    
    return true;
}

bool SLCImporter::writeMaterial( const Material* material )
{
    stringstream ss;
    ss << "writeMaterial" << endl;
    ss << "\tOriginalId = " << material->getOriginalId () << endl;
    ss << "\tName       = " << material->getName () << endl;
    const UniqueId& uid = material->getInstantiatedEffect();
    ss << "\tUniqueId   = " << uid.toAscii() << endl;
    cout << ss.str();
    return true;
}

bool SLCImporter::writeEffect( const Effect* effect )
{
    stringstream ss;
    ss << "writeEffect" << endl;
    ss << "\tOriginalId = " << effect->getOriginalId () << endl;
    ss << "\tName = " << effect->getName () << endl;
    const Color& c = effect->getStandardColor();
    ss << "\tColor = (" << c.getRed() << ',' << c.getGreen() << ',' << c.getBlue() << ')' << endl;
    const CommonEffectPointerArray& effects = effect->getCommonEffects();
    for ( int i=0; i<effects.getCount(); i++ )
    {
	ss << "\tCommonEffect : " << i << endl;
	const EffectCommon* common = effects[i];
	ss << "\t\tOriginalId = " << common->getOriginalId () << endl;
	switch ( common->getShaderType () )
	{
	case EffectCommon::SHADER_UNKNOWN:
	    ss << "\t\tShaderType = SHADER_UNKNOWN" << endl;
	    break;
	case EffectCommon::SHADER_BLINN:
	    ss << "\t\tShaderType = SHADER_BLINN" << endl;
	    break;
	case EffectCommon::SHADER_CONSTANT:
	    ss << "\t\tShaderType = SHADER_CONSTANT" << endl;
	    break;
	case EffectCommon::SHADER_PHONG:
	    ss << "\t\tShaderType = SHADER_PHONG" << endl;
	    break;
	case EffectCommon::SHADER_LAMBERT:
	    ss << "\t\tShaderType = SHADER_LAMBERT" << endl;
	    break;
	}
	const ColorOrTexture& emission = common->getEmission ();
	const Color& ec = emission.getColor();
	ss << "\t\tEmission = (" << ec.getRed() << ',' << ec.getGreen() << ',' << ec.getBlue() << ',' << ec.getAlpha() << ')' << endl;
	const ColorOrTexture& ambient = common->getAmbient ();
	const Color& ac = ambient.getColor();
	ss << "\t\tAmbient = (" << ac.getRed() << ',' << ac.getGreen() << ',' << ac.getBlue() << ',' << ac.getAlpha() << ')' << endl;
	const ColorOrTexture& diffuse = common->getDiffuse ();
	const Color& dc = diffuse.getColor();
	ss << "\t\tDiffuse = (" << dc.getRed() << ',' << dc.getGreen() << ',' << dc.getBlue() << ',' << dc.getAlpha() << ')' << endl;
	const ColorOrTexture& specular = common->getSpecular ();
	const Color& sc = specular.getColor();
	ss << "\t\tSpecular = (" << sc.getRed() << ',' << sc.getGreen() << ',' << sc.getBlue() << ',' << sc.getAlpha() << ')' << endl;
	const FloatOrParam& shininess = common->getShininess ();
	ss << "\t\tShininess = " << shininess.getFloatValue () << endl;
	const ColorOrTexture& reflective = common->getReflective ();
	const Color& rc = reflective.getColor();
	ss << "\t\tReflective = (" << rc.getRed() << ',' << rc.getGreen() << ',' << rc.getBlue() << ',' << rc.getAlpha() << ')' << endl;
	const FloatOrParam& reflectivity = common->getReflectivity();
	if ( reflectivity.getType () == FloatOrParam::FLOAT )
	    ss << "\t\tReflectivity = " << common->getReflectivity().getFloatValue() << endl;
	if ( reflectivity.getType () == FloatOrParam::PARAM ) {
	    const Param& p = common->getReflectivity().getParam();
	    ss << "\t\tReflectivity : name=" << p.getName() << ", Sid=" << p.getSid() << ", Semantic=" << p.getSemantic() << endl;
	}
	const ColorOrTexture& opacity = common->getOpacity  ();
	const Color& oc = opacity.getColor();
	ss << "\t\tOpacity = (" << oc.getRed() << ',' << oc.getGreen() << ',' << oc.getBlue() << ',' << oc.getAlpha() << ')' << endl;
	const FloatOrParam& ior = common->getIndexOfRefraction();
	ss << "\t\tIndexOfRefraction = " << ior.getFloatValue() << endl;
// 	const SamplerPointerArray& sparray = common->getSamplerPointerArray();
// 	ss << "\t\tSamplerPointerArray = " << common->getSamplerPointerArray() << endl;
    }

    cout << ss.str();
    return true;
}

bool SLCImporter::writeCamera( const Camera* camera )
{
    stringstream ss;
    ss << "writeCamera" << endl;
    ss << "\tOriginalId = " << camera->getOriginalId () << endl;
    ss << "\tName = " << camera->getName() << endl;
    switch ( camera->getCameraType() )
    {
    case Camera::UNDEFINED_CAMERATYPE:
	ss << "\tCameraType = UNDEFINED_CAMERATYPE" << endl;
	break;
    case Camera::ORTHOGRAPHIC:
	ss << "\tCameraType = ORTHOGRAPHIC" << endl;
	break;
    case Camera::PERSPECTIVE:
	ss << "\tCameraType = PERSPECTIVE" << endl;
	break;
    }
    switch ( camera->getDescriptionType() )
    {
    case Camera::UNDEFINED:
	ss << "\tDescriptionType = UNDEFINED" << endl;
	break;
    case Camera::SINGLE_X:
	ss << "\tDescriptionType = SINGLE_X" << endl;
	break;
    case Camera::SINGLE_Y:
	ss << "\tDescriptionType = SINGLE_Y" << endl;
	break;
    case Camera::X_AND_Y:
	ss << "\tDescriptionType = X_AND_Y" << endl;
	break;
    case Camera::ASPECTRATIO_AND_X:
	ss << "\tDescriptionType = ASPECTRATIO_AND_X" << endl;
	break;
    case Camera::ASPECTRATIO_AND_Y:
	ss << "\tDescriptionType = ASPECTRATIO_AND_Y" << endl;
	break;
    }
    ss << "\tXFov = " << camera->getXFov().getValue() << endl;
    ss << "\tXMag = " << camera->getXMag().getValue() << endl;
    ss << "\tYFov = " << camera->getYFov().getValue() << endl;
    ss << "\tYMag = " << camera->getYMag().getValue() << endl;
    ss << "\tAspectRatio = " << camera->getAspectRatio().getValue() << endl;
    ss << "\tNearClippingPlane = " << camera->getNearClippingPlane().getValue() << endl;
    ss << "\tFarClippingPlane = " << camera->getFarClippingPlane().getValue() << endl;
    
    cout << ss.str();
    return true;
}

bool SLCImporter::writeImage( const Image* image )
{
    stringstream ss;
    ss << "writeImage" << endl;
    ss << "\tOriginalId = " << image->getOriginalId () << endl;
    ss << "\tSourceType = ";
    switch ( image->getSourceType ())
    {
    case Image::SOURCE_TYPE_URI:
	ss << "SOURCE_TYPE_URI" << endl;
	break;
    case Image::SOURCE_TYPE_DATA:
	ss << "SOURCE_TYPE_DATA" << endl;
	break;
    case Image::SOURCE_TYPE_UNKNOWN:
	ss << "SOURCE_TYPE_UNKNOWN" << endl;
	break;
    }
    ss << "\tName = " << image->getName () << endl;
    ss << "\tFormat = " << image->getFormat () << endl;
    ss << "\tHeight = " << image->getHeight () << endl;
    ss << "\tWidth = " << image->getWidth () << endl;
    ss << "\tDepth = " << image->getDepth () << endl;
    ss << "\tImageURI = " << image->getImageURI ().getURIString() << endl;
    cout << ss.str();
    return true;
}

bool SLCImporter::writeLight( const Light* light )
{
    stringstream ss;
    ss << "writeLight" << endl;
    ss << "\tOriginalId = " << light->getOriginalId () << endl;
    ss << "\tName = " << light->getName() << endl;
    switch ( light->getLightType() )
    {
    case Light::UNDEFINED:
	ss << "\tLightType = UNDEFINED" << endl;
	break;
    case Light::AMBIENT_LIGHT:
	ss << "\tLightType = AMBIENT_LIGHT" << endl;
	break;
    case Light::DIRECTIONAL_LIGHT:
	ss << "\tLightType = DIRECTIONAL_LIGHT" << endl;
	break;
    case Light::POINT_LIGHT:
	ss << "\tLightType = POINT_LIGHT" << endl;
	break;
    case Light::SPOT_LIGHT:
	ss << "\tLightType = SPOT_LIGHT" << endl;
	break;
    }

    const Color& c = light->getColor();
    ss << "\tColor = (" << c.getRed() << ',' << c.getGreen() << ',' << c.getBlue() << ')' << endl;
    ss << "\tConstantAttenuation = " << light->getConstantAttenuation().getValue() << endl;
    ss << "\tLinearAttenuation = " << light->getLinearAttenuation().getValue() << endl;
    ss << "\tQuadraticAttenuation = " << light->getQuadraticAttenuation().getValue() << endl;
    ss << "\tFallOffAngle = " << light->getFallOffAngle().getValue() << endl;
    ss << "\tFallOffExponent = " << light->getFallOffExponent().getValue() << endl;

    cout << ss.str();
    return true;
}

bool SLCImporter::writeAnimation( const Animation* animation )
{
    cout << "writeAnimation" << endl;
    return true;
}

bool SLCImporter::writeAnimationList( const AnimationList* animationList )
{
    cout << "writeAnimationList" << endl;
    return true;
}

bool SLCImporter::writeSkinControllerData( const SkinControllerData* skinControllerData )
{
    cout << "writeSkinControllerData" << endl;
    return true;
}

bool SLCImporter::writeController( const Controller* controller )
{
    cout << "writeController" << endl;
    return true;
}

bool SLCImporter::writeFormulas( const Formulas* formulas )
{
    cout << "writeFormulas" << endl;
    return true;
}

bool SLCImporter::writeKinematicsScene( const KinematicsScene* kinematicsScene )
{
    stringstream ss;
    ss << "writeKinematicsScene" << endl;
    
    return true;
}
