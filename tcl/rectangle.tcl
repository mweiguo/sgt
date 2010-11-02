set rcid [rectangle_create]
rectangle_size $rcid 10 10
add_child 0 $rcid

update_bbox $rcid

find_viewi $rcid 0.8 $camid $vpid