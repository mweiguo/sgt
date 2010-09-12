
proc candle { index start end highest lowest } {
    set candleWidth 5
    set marginWidth 2
    set candleNmarginWidth [expr $candleWidth + $marginWidth]
    set halfCandleWidth [expr $candleWidth / 2.0]
    set deltaSE [expr abs($start   - $end)]
    set deltaHL [expr abs($highest - $lowest) ]
    set deltaEH [expr abs($highest - $end) ]
    set deltaSH [expr abs($highest - $start) ]
    set deltaEL [expr abs($lowest  - $end) ]
    set deltaSL [expr abs($lowest  - $start) ]

    set candleId [groupnode_create]

    # transform node, vertical offset = start, horizontal offset = index * candleNmarginWidth + halfCandleWidth
    set transid [transform_create]
    transform_translate $transid [expr $index * $candleNmarginWidth + $halfCandleWidth] $start 0

    set rcid [rectangle_create]
    rectangle_size $rcid $candleWidth $deltaSE
    
    # generate white line
    set whiteline [line_create]
    if { [expr $start < $end] } {
	line_points $whiteline $halfCandleWidth $deltaSE $halfCandleWidth $deltaSH
    } else {
	line_points $whiteline $halfCandleWidth 0 $halfCandleWidth -$deltaSH
    }

    # generate shade line
    set shadeline [line_create]
    if { [expr $start < $end] } {
	line_points $shadeline $halfCandleWidth 0 $halfCandleWidth -$deltaSL
    } else {
	line_points $shadeline $halfCandleWidth $deltaSE $halfCandleWidth $deltaSL
    }

    add_child $candleId $transid
    add_child $transid $rcid
    add_child $rcid $whiteline
    add_child $rcid $shadeline

    return $candleId
}

