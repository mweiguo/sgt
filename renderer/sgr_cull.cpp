void cull ( LC& lc, const BBox& viewFrustum )
{
    lc.toElement ( ROOT );
    // check bbox
    BBox& box = lc.getBBox ();
    if ( isSeperate ( viewFrustum, box ) )
	return;
    // iterate each element
    lc.toElement ( FIRST_CHILD );
    lc.toElement ( NEXT_SIBLING );
}

void render ( LC& lc, vector<int>& objectIndex )
{
    for ( int i=0; i<objectIndex.size(); i++ ) {
	int idx = objectIndex[i];
	// setup material
	// setup texcoord
	// setup normal
	// draw tristrip
    }
}

// generate render tree

// apply appearance
// draw element

// 3d scene format


<model id="cow">
    <vertex>1 2 3 4 5 6 7 8 9</vertex>
    <texcoord>0.1 0.2 0.5 0.6</texcoord>
    <normal>0.1 0.2 0.3 0.1 0.3 0.2</normal>
    <appearance type="phong">
        <material texture="tex1" shininess="0.1" diffuse="0.8 0.8 0.8" />
        <mesh type=triangle>
            <vertexindex>0 1 2 3 2 0</vertexindex>
            <texcoordindex>0 1 2 3 2 0</texcoordindex>
            <normalindex>0 1 2 3 2 0</normalindex>
        </mesh>
    </appearance>
    <appearance type="blin">
        <material texture="tex2" shininess="0.1" diffuse="0.8 0.8 0.8">
        <mesh type=triangle>
            <vertexindex>0 1 2 3 2 0</vertexindex>
            <texcoordindex>0 1 2 3 2 0</texcoordindex>
            <normalindex>0 1 2 3 2 0</normalindex>
        </mesh>
    </appearance>
</model>
