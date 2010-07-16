toplevel .ctrl

button .ctrl.btnleft   	 -text "left"    -command "camera_change left"
button .ctrl.btnright  	 -text "right"   -command "camera_change right"
button .ctrl.btnup     	 -text "up"      -command "camera_change up"
button .ctrl.btndown   	 -text "down"    -command "camera_change down"
button .ctrl.reset     	 -text "reset"   -command "camera_change reset"
button .ctrl.btnzoomin   -text "+"       -command "camera_change zoomin"
button .ctrl.btnzoomout  -text "-"       -command "camera_change zoomout"


pack .ctrl.btnleft .ctrl.btnright .ctrl.btnup .ctrl.btndown .ctrl.reset
pack .ctrl.btnzoomin .ctrl.btnzoomout

set dx 0
set dy 0
set scale 1

proc camera_change { action } {
    global camid
    global vpid
    global dx
    global dy
    global scale

    if { $action == "left" } {
	set dx [expr $dx - 0.5]
	camera_translate $camid $dx $dy 0
    } elseif { $action == "right" } {
	set dx [expr $dx + 0.5]
	camera_translate $camid $dx $dy 0
    } elseif { $action == "up" } {
	set dy [expr $dy + 0.5]
	camera_translate $camid $dx $dy 0
    } elseif { $action == "down" } {
	set dy [expr $dy - 0.5]
	camera_translate $camid $dx $dy 0
    } elseif { $action == "zoomin" } {
	set scale [expr $scale * 1.2]
	camera_scale $camid $scale
    } elseif { $action == "zoomout" } {
	set scale [expr $scale / 1.2]
	camera_scale $camid $scale
    } elseif { $action == "reset" } {
	set dx 0
	set dy 0
	camera_translate $camid $dx $dy 0
    } else {
	return
    }

    update $vpid
}


