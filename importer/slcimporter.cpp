#include "slcimporter.h"
#include <iostream>
#include <COLLADASaxFWLLoader.h>
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

    for ( UniqueIdFWMaterialMap::iterator pp=_materialMap.begin(); pp!=_materialMap.end(); ++pp ) {
	cout << "material: " << pp->first.toAscii() << endl;
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
    ss << "\tInstanciatedObjectId   = " << uid.toAscii() << endl;
    cout << ss.str();
    return true;
}

string SLCImporter::writeNode ( const COLLADAFW::Node* node, const string& prefix )
{
    stringstream ss;
    ss << prefix << "OriginalId = " << node->getOriginalId () << endl;
    ss << prefix << "Name = " << node->getName () << endl;
    switch ( node->getType () )
    {
    case Node::NODE:
	ss << prefix << "Type = NODE" << endl;
	break;
    case Node::JOINT:
	ss << prefix << "Type = JOINT" << endl;
	break;
    }
    const TransformationPointerArray& tparray = node->getTransformations ();
    if ( tparray.getCount() != 0 )
	ss << prefix << "Transformations : " << endl;
    for ( int i=0; i<tparray.getCount(); i++ )
    {
	const Transformation* t = tparray[i];
	switch ( t->getTransformationType() )
	{
	case Transformation::MATRIX:
	    ss << prefix << "\tTransformationType = MATRIX" << endl;
	    break;
	case Transformation::TRANSLATE:
	    ss << prefix << "\tTransformationType = TRANSLATE" << endl;
	    break;
	case Transformation::ROTATE:
	    ss << prefix << "\tTransformationType = ROTATE" << endl;
	    break;
	case Transformation::SCALE:
	    ss << prefix << "\tTransformationType = SCALE" << endl;
	    break;
	case Transformation::LOOKAT:
	    ss << prefix << "\tTransformationType = LOOKAT" << endl;
	    break;
	case Transformation::SKEW:
	    ss << prefix << "\tTransformationType = SKEW" << endl;
	    break;
	}
    }
    COLLADABU::Math::Matrix4 mat = node->getTransformationMatrix();
    ss << prefix << "\tmat = ";
    for ( int i=0; i<16; i++ )
	ss << mat.getElement( i ) << ", ";
    ss << endl;

    const InstanceGeometryPointerArray& gparray = node->getInstanceGeometries ();
    for ( int i=0; i<gparray.getCount(); i++ )
    {
	ss << prefix << "InstanceGeometry : " << i << endl;
	const InstanceGeometry* instgeo = gparray[i];
	ss << prefix << "\tName = " << instgeo->getName() << endl;
	ss << prefix << "\tInstanciatedObjectId = " << instgeo->getInstanciatedObjectId().toAscii() << endl;
        const InstanceGeometry::MaterialBindingArray& mbarray = gparray[i]->getMaterialBindings();
	for ( int j=0; j<mbarray.getCount(); j++ )
	{
	    ss << prefix << "\tMaterialBinding : " << j << endl;
	    const InstanceGeometry::MaterialBinding& mb = mbarray[j];
	    ss << prefix << "\t\tMaterialId = " << mb.getMaterialId () << endl;
	    ss << prefix << "\t\tReferencedMaterial = " << mb.getReferencedMaterial ().toAscii() << endl;
	    ss << prefix << "\t\tName = " << mb.getName () << endl;
	    const InstanceGeometry::TextureCoordinateBindingArray& tcbarray = mb.getTextureCoordinateBindingArray ();
	    for ( int k=0; k<tcbarray.getCount(); k++ )
	    {
		const InstanceGeometry::TextureCoordinateBinding& tcb = tcbarray[k];
		ss << prefix << "\t\t\ttextureMapId = " << tcb.textureMapId << endl;
		ss << prefix << "\t\t\tsetIndex = " << tcb.setIndex << endl;
	    }
	}
    }

    const InstanceNodePointerArray& nparray = node->getInstanceNodes ();
    if ( nparray.getCount() != 0 )
	ss << prefix << "InstanceNodes : " << endl;
    for ( int i=0; i<nparray.getCount(); i++ )
    {
	const InstanceNode* n = nparray[i];
	ss << prefix << "\tName = " << n->getName() << endl;
	ss << prefix << "\tInstanciatedObjectId = " << n->getInstanciatedObjectId().toAscii() << endl;
    }
	
    const InstanceCameraPointerArray& cpa = node->getInstanceCameras ();
    if ( cpa.getCount() != 0 )
	ss << prefix << "InstanceCameras : " << endl;
    for ( int i=0; i<cpa.getCount(); i++ )
    {
	const InstanceCamera* c = cpa[i];
	ss << prefix << "\tName = " << c->getName() << endl;
	ss << prefix << "\tInstanciatedObjectId = " << c->getInstanciatedObjectId().toAscii() << endl;
    }

    const InstanceLightPointerArray& lpa = node->getInstanceLights ();
    if ( lpa.getCount() != 0 )
	ss << prefix << "InstanceLights : " << endl;
    for ( int i=0; i<lpa.getCount(); i++ )
    {
	const InstanceLight* l = lpa[i];
	ss << prefix << "\tName = " << l->getName() << endl;
	ss << prefix << "\tInstanciatedObjectId = " << l->getInstanciatedObjectId().toAscii() << endl;
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
	ss << prefix << "Nodes : " << endl;
    for ( int i=0; i<npa.getCount(); i++ )
	ss << writeNode ( npa[i], prefix + "\t" );
	
    return ss.str();
}

bool SLCImporter::writeVisualScene ( const VisualScene* visualScene )
{
//    _visualSceneMap.insert ( pair<COLLADAFW::UniqueId, COLLADAFW::VisualScene>( visualScene->getUniqueId (), *visualScene) );
    stringstream ss;
    ss << "writeVisualScene" << endl;
    ss << "\tUniqueId = " << visualScene->getUniqueId ().toAscii() << endl;
    ss << "\tName = " << visualScene->getName() << endl;
    const NodePointerArray& array = visualScene->getRootNodes ();

    for ( int i=0; i<array.getCount(); i++ ) {
	ss << "\tNode : " << i << endl;
	ss << writeNode ( array[i], "\t\t" );
    }

    cout << ss.str();
    return true;
}

bool SLCImporter::writeLibraryNodes ( const LibraryNodes* libraryNodes )
{
    stringstream ss;
    ss << "writeLibraryNodes" << endl;
    return true;
}

string writeMeshVertexData ( const MeshVertexData& mvd, const string& prefix )
{
    stringstream ss;
    ss << prefix << ".ValuesCount = " << mvd.getValuesCount () << endl;
    switch ( mvd.getType () )
    {
    case FloatOrDoubleArray::DATA_TYPE_FLOAT:
    {
	ss << prefix << "DataType = DATA_TYPE_FLOAT" << endl;
	const FloatArray* farray = mvd.getFloatValues();
	if ( mvd.getValuesCount() != 0 ) {
	    ss << prefix << "DataValues = ";
	    for ( int i=0; i<mvd.getValuesCount(); i++ )
		ss << (*farray)[i] << ' ';
	    ss << endl;
	}
	break;
    }
    case FloatOrDoubleArray::DATA_TYPE_DOUBLE:
    {
	ss << prefix << "DataType = DATA_TYPE_DOUBLE" << endl;
	const DoubleArray* farray = mvd.getDoubleValues();
	if ( mvd.getValuesCount() != 0 ) {
	    ss << prefix << "DataValues = ";
	    for ( int i=0; i<mvd.getValuesCount(); i++ )
		ss << (*farray)[i] << ' ';
	    ss << endl;
	}
	break;
    }
    case FloatOrDoubleArray::DATA_TYPE_UNKNOWN:
	ss << prefix << "DataType = DATA_TYPE_UNKNOWN" << endl;
	break;		
    }
    return ss.str();
}

bool SLCImporter::writeGeometry ( const Geometry* geometry )
{
//    _geometryMap.insert(pair<COLLADAFW::UniqueId, COLLADAFW::Geometry>(geometry->getUniqueId (), *geometry));
    stringstream ss;
    ss << "writeGeometry" << endl;
    ss << "\tUniqueId = " << geometry->getUniqueId ().toAscii() << endl;
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

    if ( Geometry::GEO_TYPE_MESH == geometry->getType ())
    {
	const Mesh* mesh = dynamic_cast<const Mesh*>(geometry);
	if ( mesh )
	{
	    ss << writeMeshVertexData ( mesh->getPositions (), "\tPositions" );
	    ss << writeMeshVertexData ( mesh->getNormals (), "\tNormals" );
	    ss << writeMeshVertexData ( mesh->getColors (), "\tColors" );
	    ss << writeMeshVertexData ( mesh->getUVCoords (), "\tUVCoords" );

	    const MeshPrimitiveArray& mparray = mesh->getMeshPrimitives ();
	    for ( int i=0; i<mparray.getCount(); i++)
	    {
		const MeshPrimitive* p = mparray[i];
		ss << "\tMeshPrimitive" << i << " : " << endl;
		switch ( p->getPrimitiveType() )
		{
		case MeshPrimitive::LINES:
		    ss << "\t\tPrimitiveType = LINES" << endl;
		    break;
		case MeshPrimitive::LINE_STRIPS:
		    ss << "\t\tPrimitiveType = LINE_STRIPS" << endl;
		    break;
		case MeshPrimitive::POLYGONS:
		    ss << "\t\tPrimitiveType = POLYGONS" << endl;
		    break;
		case MeshPrimitive::POLYLIST:
		    ss << "\t\tPrimitiveType = POLYLIST" << endl;
		    break;
		case MeshPrimitive::TRIANGLES:
		    ss << "\t\tPrimitiveType = TRIANGLES" << endl;
		    break;
		case MeshPrimitive::TRIANGLE_FANS:
		    ss << "\t\tPrimitiveType = TRIANGLE_FANS" << endl;
		    break;
		case MeshPrimitive::TRIANGLE_STRIPS:
		    ss << "\t\tPrimitiveType = TRIANGLE_STRIPS" << endl;
		    break;
		case MeshPrimitive::POINTS:
		    ss << "\t\tPrimitiveType = POINTS" << endl;
		    break;
		case MeshPrimitive::UNDEFINED_PRIMITIVE_TYPE:
		    ss << "\t\tPrimitiveType = UNDEFINED_PRIMITIVE_TYPE" << endl;
		    break;
		}
		ss << "\t\tFaceCount = " << p->getFaceCount () << endl;
		ss << "\t\tMaterial = " << p->getMaterial () << endl;
		const UIntValuesArray& pindices = p->getPositionIndices ();
		if ( pindices.getCount() != 0 ) {
		    ss << "\t\tPositionIndices = ";
		    for ( int j=0; j<pindices.getCount(); j++ )
			ss << pindices[j] << ' ';
		    ss << endl;
		}

		const UIntValuesArray& nindices = p->getNormalIndices ();
		if ( nindices.getCount() != 0 ) {
		    ss << "\t\tNormalIndices = ";
		    for ( int j=0; j<nindices.getCount(); j++ )
			ss << nindices[j] << ' ';
		    ss << endl;
		}

		const IndexListArray& ciarray = p->getColorIndicesArray ();
		if ( ciarray.getCount() != 0 ) {
		    for ( int j=0; j<ciarray.getCount(); j++ ) {
			ss << "\t\tColorIndexList" << j << " = ";
			const IndexList* il = ciarray[j];
			const UIntValuesArray& nindices = il->getIndices ();
			if ( nindices.getCount() != 0 ) {
			    ss << "\t\t\tIndices = ";
			    for ( int k=0; k<nindices.getCount(); k++ )
				ss << nindices[k] << ' ';
			    ss << endl;
			}
			ss << "\t\t\tStride = " << il->getStride () << endl;
			ss << "\t\t\tName = " << il->getName () << endl;
			ss << "\t\t\tInitialIndex = " << il->getInitialIndex () << endl;
		    }
		}

		const IndexListArray& uvarray = p->getUVCoordIndicesArray ();
		if ( uvarray.getCount() != 0 ) {
		    for ( int j=0; j<uvarray.getCount(); j++ ) {
			ss << "\t\tUVCoordIndexList" << j << " : " << endl;
			const IndexList* il = uvarray[j];
			const UIntValuesArray& nindices = il->getIndices ();
			if ( nindices.getCount() != 0 ) {
			    ss << "\t\t\tIndices = ";
			    for ( int k=0; k<nindices.getCount(); k++ )
				ss << nindices[k] << ' ';
			    ss << endl;
			}
			ss << "\t\t\tStride = " << il->getStride () << endl;
			ss << "\t\t\tName = " << il->getName () << endl;
			ss << "\t\t\tInitialIndex = " << il->getInitialIndex () << endl;
		    }
		}
		
		ss << "\t\tMaterialId = " << p->getMaterialId() << endl;
		ss << "\t\tGroupedVertexElementsCount = " << p->getGroupedVertexElementsCount () << endl;
	    }

//	    ss << "\tPrimitiveCount = " << mesh->getPrimitiveCount () << endl;
	    ss << "\tFacesCount = " << mesh->getFacesCount () << endl;
//	    getMeshPrimitiveCount ().
	}
    }

    cout << ss.str();
    
    return true;
}

bool SLCImporter::writeMaterial( const Material* material )
{
    _materialMap.insert(pair<UniqueId, Material>(material->getUniqueId(), *material));
    stringstream ss;
    ss << "writeMaterial" << endl;
    ss << "\tUniqueId = " << material->getUniqueId().toAscii() << endl;
    ss << "\tOriginalId = " << material->getOriginalId () << endl;
    ss << "\tName       = " << material->getName () << endl;
    const UniqueId& uid = material->getInstantiatedEffect();
    ss << "\tInstantiatedEffect.UID = " << uid.toAscii() << endl;
    cout << ss.str();
    return true;
}

bool SLCImporter::writeEffect( const Effect* effect )
{
//    _effectMap.insert(pair<COLLADAFW::UniqueId, COLLADAFW::Effect>(effect->getUniqueId (),*effect));
    stringstream ss;
    ss << "writeEffect" << endl;
    ss << "\tUniqueId = " << effect->getUniqueId ().toAscii() << endl;
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
//     _cameraMap.insert(pair<COLLADAFW::UniqueId, COLLADAFW::Camera>(camera->getUniqueId (), *camera));
    stringstream ss;
    ss << "writeCamera" << endl;
    ss << "\tUniqueId = " << camera->getUniqueId ().toAscii() << endl;
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
//     _imageMap.insert(pair<COLLADAFW::UniqueId, COLLADAFW::Image>(image->getUniqueId(), *image));
    stringstream ss;
    ss << "writeImage" << endl;
    ss << "\tUniqueId = " << image->getUniqueId().toAscii() << endl;
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
//     _lightMap.insert(pair<COLLADAFW::UniqueId, COLLADAFW::Light>(light->getUniqueId(), *light));
    stringstream ss;
    ss << "writeLight" << endl;
    ss << "\tUniqueId = " << light->getUniqueId().toAscii() << endl;
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
