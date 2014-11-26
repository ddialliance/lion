<?php
function get_mappings($type){
    require_once('xmi_export.inc');
    $objects = get_all_node_objects();
    dsm($objects);
    
    $mappings = array();
    
    
    
    
    return theme('mappings', array('mappings' => $mappings));
}
