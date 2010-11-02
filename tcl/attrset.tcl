set layerid [layer_create 150 "attrset test"]
add_child 0 $layerid

set colorid [color_create 1000 0]
color_rgbas $colorid "#ff0000ff"

set attrsetid [attrset_create 10000 $layerid]
attrset_fgcolor $attrsetid $colorid
attrset_bgcolor $attrsetid $colorid

# set rcid [rectangle_create]
# rectangle_size $rcid 10 10
# add_child $layerid $rcid
# set_attrset $rcid $attrsetid

#set lineid [line_create]
#line_points $lineid 0 0 1 1
#add_child $layerid $lineid
#set_attrset $lineid $attrsetid
#attrset_fgcolor $attrsetid "#ff0000ff"

# create a polyline for test
set plid [poly_create 10]
poly_points $plid 0 0 1 1 1 -1 -1 1 -1 -1
add_child 0 $plid
set_attrset $plid $attrsetid



update_bbox 0
find_viewi 0 0.8 $camid $vpid