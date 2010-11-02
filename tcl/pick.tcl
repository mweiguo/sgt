# pick test
proc rectangle_pick_test { x0 y0 w h isShouldBeClicked rcid } {
    global camid
#   2     3
#
#   
#   1     4
    set x1 $x0
    set y1 $y0
    set x2 $x0
    set y2 [expr $y0 + $h]
    set x3 [expr $x0 + $w]
    set y3 $y2
    set x4 $x3
    set y4 $y1
    for {set y $y1} {$y<=$y2} {set y [expr $y+0.01] } {
	for {set x $x1} {$x<=$x4} {set x [expr $x+0.01] } {
	    
	    set pickid [pick $x $y 0 $camid]

	    if { $isShouldBeClicked } {
		if { $pickid != $rcid } {
		    puts "rectangle pick failed:  pickid = $pickid"
		}
	    } else {
		if { $pickid == $rcid } {
		    puts "rectangle pick failed:  pickid = $pickid"
		}
	    }

	}
    }
}

proc rectangle_test {} {
    global rcid
    # crate layer
    set rclayer [layer_create "rectangles"]
    add_child 0 $rclayer

    # create rectangle
    set rcid [rectangle_create]
    rectangle_size $rcid 10 10
    add_child $rclayer $rcid

    update_bbox 0

    # center
    rectangle_pick_test -0.01 -0.01 10.02 10.02 1 $rcid
    #left
    rectangle_pick_test -0.1 -1 0.089 12 0 $rcid
    #top
    rectangle_pick_test -1 11 12 0.09 0 $rcid
    #right
    rectangle_pick_test 10.011 0 1 12 0 $rcid
    #bottom
    rectangle_pick_test -1 -1 12 0.09 0 $rcid

    # clean up
    node_delete $rclayer
}

proc line_test {} {
    # crate layer
    set layer [layer_create "lines"]
    add_child 0 $layer

    # create line
    set id [line_create]
    line_points $id 0 0 0 1

    add_child $layer $id

    update_bbox 0

    global camid
    set pickid [pick 0.01 1 0 $camid]
    if { $pickid != $id } {
	puts "line pick failed:  pickid = $pickid"
    }

    # clean up
    node_delete $layer
}

#rectangle_test
line_test