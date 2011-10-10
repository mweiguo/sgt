#include "sgr_lcreport.h"
#include "sgr_nodetypes.h"

#include <sstream>
#include <iostream>

using namespace std;

LCReport::LCReport ( LC& lc, bool dumpTree ) : _dumpTree(dumpTree)
{
    cscene = 0;
    cmat   = 0;
    clayer = 0;
    clod   = 0;
    clodpage = 0;
    cpline = 0;
    cpoly  = 0;
    cline  = 0;
    ctri   = 0;
    cquad  = 0;
    ctext  = 0;

    _lc = &lc;
    lc.toElement ( ROOT );
    counter ( lc.getType() );
    if ( _dumpTree )
	cout << lc.getGIndex() << ", type : " << lc.getTypeStr() << ", " << getContent(lc.getType(), lc.getValue(), lc, lc.getGIndex() ) << endl;

    traverse ( lc );
}

void LCReport::printCounter ()
{
    cout << "================== counter =======================" << endl;
    cout << "SLC_SCENE    count = " << cscene << endl;
    cout << "SLC_MATERIAL count = " << cmat << endl;
    cout << "SLC_LAYER    count = " << clayer << endl;
    cout << "SLC_LOD      count = " << clod << endl;
    cout << "SLC_LODPAGE  count = " << clodpage << endl;
    cout << "SLC_PLINE    count = " << cpline << endl;
    cout << "SLC_POLY     count = " << cpoly << endl;
    cout << "SLC_LINE     count = " << cline << endl;
    cout << "SLC_TRIANGLE count = " << ctri << endl;
    cout << "SLC_QUAD     count = " << cquad << endl;
    cout << "SLC_TEXT     count = " << ctext << endl;
    cout << "================== memory =======================" << endl;
    cout << "globalLCEntry count = " << _lc->globalLCEntry->LCLen << ", size = " << _lc->globalLCEntry->LCLen * sizeof(GlobalLCRecord) + sizeof(int) << "(Byte)" << endl;
    int levelLCEntriesSize = 0;
    for ( int i=0; i<256; i++ ) {
        if ( _lc->levelLCEntries[i] != 0 ) {
            cout << "levelLCEntries[" << i << "] size : " << _lc->levelLCEntries[i]->LCLen * sizeof(LevelLCRecord) + sizeof(int) << "(Byte)" << endl;
            levelLCEntriesSize += _lc->levelLCEntries[i]->LCLen * sizeof(LevelLCRecord) + sizeof(int);
        }
        else
            break;
    }
    cout << "levelLCEntries total size : " << levelLCEntriesSize << "(Byte)" << endl;

}

void LCReport::counter ( int type )
{
    switch ( type ) {
    case SLC_SCENE:
        cscene++;
        break;
    case SLC_LOD:
        clod++;
        break;
    case SLC_LODPAGE:
        clodpage++;
        break;
    case SLC_LAYER:
        clayer++;
        break;
    case SLC_PLINE:
        cpline++;
        break;
    case SLC_POLY:
        cpoly++;
        break;
    case SLC_LINE:
        cline++;
        break;
    case SLC_TRIANGLE:
        ctri++;
        break;
    case SLC_RECT:
        cquad++;
        break;
    case SLC_TEXT:
        ctext++;
        break;
    case SLC_MATERIAL:
        cmat++;
        break;
    }
}

void LCReport::traverse ( LC& lc )
{
    if ( lc.toElement ( FIRST_CHILD ) >= 0 ) {
        counter ( lc.getType() );
	if ( _dumpTree ) {
	    string prefix = "";
	    for ( int i=0; i<lc.getDepth(); i++ )
		prefix += "    ";
	    cout << prefix << lc.getGIndex() << ", type : " << lc.getTypeStr() << ", " << getContent(lc.getType(), lc.getValue(), lc, lc.getGIndex() ) << endl;
	}
        traverse ( lc );
        while ( lc.toElement ( NEXT_SIBLING )>=0 ) {
            counter ( lc.getType() );
	    if ( _dumpTree ) {
		string prefix = "";
		for ( int i=0; i<lc.getDepth(); i++ )
		    prefix += "    ";
		cout << prefix << lc.getGIndex() << ", type : " << lc.getTypeStr() << ", " << getContent(lc.getType(), lc.getValue(), lc, lc.getGIndex() ) << endl;
	    }
            traverse ( lc );
        }
        lc.toElement ( PARENT );
    }
}

string LCReport::getContent ( int type, int idx, LC& lc, int gidx )
{
    stringstream ss;
    GlobalLCRecord& gr = lc.globalLCEntry->LCRecords[gidx];
    ss << "Min(" << gr.minmax[0] << ',' << gr.minmax[1] << ")" << ", Max(" << gr.minmax[2] << ',' << gr.minmax[3] << ") ";
    switch ( type )
    {
    case SLC_SCENE:
        ss << "scenename : " << lc.sceneEntry->LCRecords[idx].name;
        break;
    case SLC_LAYER:
        ss << "layername : " << lc.layerEntry->LCRecords[idx].name;
        break;
    case SLC_LOD:
    {
	LODRecord& lod = lc.lodEntry->LCRecords[idx];
	if ( lod.scalecnt != 0 )
	{
	    ss << "scales : ";
	    for ( int i=0; i<lod.scalecnt; i++ )
		ss << lod.scales[i] << ' ';
	}

        break;
    }
    case SLC_LODPAGE:
        ss << "kdtreepath : " << lc.lodpageEntry->LCRecords[idx].kdtreepath
	   << ", delayloading : " << lc.lodpageEntry->LCRecords[idx].delayloading
	   << ", imposter : " << lc.lodpageEntry->LCRecords[idx].imposter;
        break;
    case SLC_PLINE:
    {
	PLineRecord& pline = lc.plineEntry->LCRecords[idx];
	ss << "matidx = " << pline.materialIdx << "; ";
	for ( int i=pline.start; i<pline.end; i++ ) {
	    vec3f& v = lc.plineBufferEntry->LCRecords[i];
	    ss << v.x() << ' ' << v.y() << ' ' << v.z() << ", ";
	}
        break;
    }
    case SLC_POLY:
    {
	PolyRecord& poly = lc.polyEntry->LCRecords[idx];
	ss << "matidx = " << poly.materialIdx << "; ";
	if ( poly.filltexture ) {
	    ss << "angle = " << poly.textureAngle << ", scale = " << poly.textureScale << ", texcoords = (";
	    for ( int i=poly.texcoordstart; i!=poly.texcoordend; i++ )
	    {
		vec2f& v = lc.texCoordBufferEntry->LCRecords[i];
		ss << v.x() << ' ' << v.y() << ' ';
	    }
	    ss << ") ";
	}
	for ( int i=poly.start; i<poly.end; i++ ) {
	    vec3f& v = lc.plineBufferEntry->LCRecords[i];
	    ss << v.x() << ' ' << v.y() << ' ' << v.z() << ", ";
	}
        break;
    }
    case SLC_LINE:
    {
        LineRecord& line = lc.lineEntry->LCRecords[idx];
        vec2f& p0 = line.data[0];
        vec2f& p1 = line.data[1];
        ss << "line : (" << p0.x() << ", " << p0.y() << ")  (" << p1.x() << ", " << p1.y() << ")";
        break;
    }
    case SLC_TRIANGLE:
    {
        TriangleRecord& tri = lc.triangleEntry->LCRecords[idx];
        vec2f& p0 = tri.data[0];
        vec2f& p1 = tri.data[1];
        vec2f& p2 = tri.data[2];
        ss << "triangle : (" << 
            p0.x() << ", " << p0.y() << ") (" <<
            p1.x() << ", " << p1.y() << ") (" << 
            p2.x() << ", " << p2.y() << ")";
        break;
    }
    case SLC_RECT:
    {
        RectRecord& quad = lc.rectEntry->LCRecords[idx];
	ss << "matidx = " << quad.materialIdx << "; ";
	if ( quad.filltexture )
	    ss << "angle = " << quad.textureAngle << ", scale = " << quad.textureScale << ", ";
        vec3f& p0 = quad.data[0];
        vec3f& p1 = quad.data[1];
        vec3f& p2 = quad.data[2];
        vec3f& p3 = quad.data[3];
        ss << "rect : (" << 
            p0.x() << ", " << p0.y() << ", " << p0.z() << ") (" <<
            p1.x() << ", " << p1.y() << ", " << p1.z() << ") (" << 
            p2.x() << ", " << p2.y() << ", " << p2.z() << ") (" << 
            p3.x() << ", " << p3.y() << ", " << p3.z() << ")";
        break;
    }
    case SLC_TEXT:
    {
        TextRecord& text = lc.textEntry->LCRecords[idx];
	ss << "matidx = " << text.materialIdx << "; ";
        ss << "pos (" << text.pos.x() << ", " << text.pos.y() << ", " << text.pos.z() << "), scale=" << text.scale <<
	    ", rotz=" << text.rotz << ", content=" << lc.textBufferEntry->LCRecords + text.start;
        break;
    }
    case SLC_MATERIAL:
    {
        MaterialRecord& mr = lc.materialEntry->LCRecords[idx];
        ss << "name : " << mr.name <<
            ", background_color ( " << mr.background_color.x() << "," << mr.background_color.y() << "," << mr.background_color.z() <<"," << mr.background_color.w() <<
            " ), foreground_color ( " << mr.foreground_color.x() << "," << mr.foreground_color.y() << "," << mr.foreground_color.z() << "," << mr.foreground_color.w() <<
            " ), linewidth = " << mr.linewidth << ", linetype = " << mr.linetype << ", linetypefactor = " << mr.linetypefactor;
	ss << ", font=" << mr.fontfile;
	ss << ", texfilename=" << mr.texturefile;
        break;
    }
    default:
        ss << "unsupport type " << type;
        break;
    }
    return ss.str();
}
