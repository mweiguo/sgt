package require tdom

# detail document see trunk\sw\primace\agtk\renderer\interface.h
set attrseed 200

proc normalize_attrid { srcfilename dstfilename } {
    set f [open $srcfilename]
    set xml [read $f]
    close $f

    # read srcfile
    set document [dom parse $xml]
    set root     [$document documentElement] 
    parseNode root

    set f [open $dstfilename "w+"]
    puts $f [$root asXML]
    close $f

    $document    delete
    # add attrid
    # save to dstfile    
}


proc parseNode { node } {
    global attrseed
    upvar $node localnode
    set nodes [$localnode childNodes]
    foreach nd $nodes {
	if { [$nd nodeType] == "ELEMENT_NODE" } {
	    puts [$nd nodeName]
	    $nd setAttribute "id" $attrseed
	    incr attrseed

	}
	parseNode nd
    }
}