toplevel .ctrl

button .ctrl.btnleft     -text "left"    -command "camera_change left"
button .ctrl.btnright    -text "right"   -command "camera_change right"
button .ctrl.btnup       -text "up"      -command "camera_change up"
button .ctrl.btndown     -text "down"    -command "camera_change down"
button .ctrl.reset       -text "reset"   -command "camera_change reset"
button .ctrl.btnzoomin   -text "+"       -command "camera_change zoomin"
button .ctrl.btnzoomout  -text "-"       -command "camera_change zoomout"


pack .ctrl.btnleft .ctrl.btnright .ctrl.btnup .ctrl.btndown .ctrl.reset
pack .ctrl.btnzoomin .ctrl.btnzoomout

set currentX 0
set currentY 0
set currentZ 0
set currentScale 1

proc camera_change { action } {
    global camid
    global vpid
    global currentX
    global currentY
    global currentScale
    set delta 0.1
    if { $action == "left" } {
        set currentX [expr $currentX - $delta]
        camera_translate $camid $currentX $currentY 0
    } elseif { $action == "right" } {
        set currentX [expr $currentX + $delta]
        camera_translate $camid $currentX $currentY 0
    } elseif { $action == "up" } {
        set currentY [expr $currentY + $delta]
        camera_translate $camid $currentX $currentY 0
    } elseif { $action == "down" } {
        set currentY [expr $currentY - $delta]
        camera_translate $camid $currentX $currentY 0
    } elseif { $action == "zoomin" } {
        set currentScale [expr $currentScale * 1.2]
        camera_scale $camid $currentScale
    } elseif { $action == "zoomout" } {
        set currentScale [expr $currentScale / 1.2]
        camera_scale $camid $currentScale
    } elseif { $action == "reset" } {
        set currentX 0
        set currentY 0
        set currentScale 1
        camera_scale $camid $currentScale
        camera_translate $camid $currentX $currentY 0
    } else {
        return
    }
    puts "currentX=$currentX currentY=$currentY currentScale=$currentScale"
    update $vpid
}


