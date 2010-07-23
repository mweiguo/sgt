set flyTime 10000
set flyDeltaTime 80
set flybrk 0

proc flyto { x y z scale } {
    global currentX
    global currentY
    global currentZ
    global currentScale
#    puts "x=$x y=$y z=$z scale=$scale"
#    puts "currentX=$currentX currentY=$currentY currentZ=$currentZ currentScale=$currentScale"

    global deltaX
    global deltaY
    global deltaZ
    global deltaScale
    global flyTime
    global flyDeltaTime
    
    set k [expr $flyTime / $flyDeltaTime]
    set deltaX [expr 1.0 * ($x-$currentX) / $k ]
    set deltaY [expr 1.0 * ($y-$currentY) / $k ]
    set deltaZ [expr 1.0 * ($z-$currentZ) / $k ]
    set deltaScale [expr 1.0 * ($scale-$currentScale) / $k ]
 #   puts "k=$k deltaX=$deltaX deltaY=$deltaY deltaZ=$deltaZ deltaScale=$deltaScale"
    if { [expr $deltaX==0 && $deltaY==0 && $deltaZ==0 && $deltaScale==0] == 0 } {
	flyToDelta $x $y $z $scale
    }
}
 

proc flyToDelta { x y z scale } {
    global flybrk
    global camid
    global vpid

    global deltaX
    global deltaY
    global deltaZ
    global deltaScale
    global flyTime
    global flyDeltaTime

    global currentX
    global currentY
    global currentZ
    global currentScale

    if {$flybrk==1} {return}
    
    camera_translate $camid $currentX $currentY $currentZ
    camera_scale $camid $currentScale
    update $vpid

    set translateThreshold 0.001
    set scaleThreshold 0.0000001
    set currentX [expr $currentX + $deltaX]
    set currentY [expr $currentY + $deltaY]
    set currentZ [expr $currentZ + $deltaZ]
    set currentScale [expr $currentScale + $deltaScale]
#    puts "currentX=$currentX currentY=$currentY currentZ=$currentZ currentScale=$currentScale"
    set rst [expr abs($currentX-$x)<$translateThreshold && abs($currentY-$y)<$translateThreshold && abs($currentZ-$z)<$translateThreshold && abs($currentScale-$scale)<$scaleThreshold ]
#    puts $rst
    if { $rst==1  } {
	set currentX $x
	set currentY $y
	set currentZ $z
	set currentScale $scale
	return
    }

    after $flyDeltaTime [list flyToDelta $x $y $z $scale]
}

#fly 0 0 0 1