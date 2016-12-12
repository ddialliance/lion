<?php
function get_mappings($type){
    $mappings = get_mappings_array($type);
    return theme('mappings', array('mappings' => $mappings, 'type' => $type));
}

function get_mappings_array($type){
    require_once('xmi_export.inc');
    //Getting all DDI Objects
    $objects = get_all_node_objects();

    if ($type=="ddi3-2"){
        
        $my_field = field_info_field('field_ddi_3_2');
        $field="ddi_3_2";
        
    } else {
        if ($type=="gsim"){
            $my_field = field_info_field('field_gsim');
            $field="gsim";
        }
    }
    
    $allowed_values = list_allowed_values($my_field);
    
    $mappings = array();
    
    //Generating a multi-dimensional empty array with keys from the allowed values ddi3 or gsim
    foreach($allowed_values as $k){ 
        $multi_array[$k][] = NULL;   
    }
    
    foreach ($objects as $key => $wert){

        foreach ($wert as $object){

            $ddi4 = $object['name'];
            $left = $object[$field];
            
            if ($left!=""){
                $multi_array[$left][] = $ddi4;
            }
        }
    }
    
    foreach($allowed_values as $list){
        $mappings[$list]= $multi_array[$list];
    }
    
    return $mappings;
}