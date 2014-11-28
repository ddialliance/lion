<?php
function get_mappings($type){
    $mappings = get_mappings_array($type);
    return theme('mappings', array('mappings' => $mappings, 'type' => $type));
}

function get_mappings_array($type){
    require_once('xmi_export.inc');
    //Getting all DDI Objects
    $objects = get_all_node_objects();
    
    //dsm($objects);
    
   
    
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
        //dsm("Package: ".$key);
        //dsm($wert);
        foreach ($wert as $object){
            //dsm($object['name']);
            $ddi4 = $object['name'];
            $left = $object[$field];
            
            if ($left!=""){
                $multi_array[$left][] = $ddi4;
                //dsm("DDI3.2: ".$ddi3);
                //dsm($multi_array[$ddi3]);
            }
        }
    }
    
    foreach($allowed_values as $list){
        $mappings[$list]= $multi_array[$list];    
        //dsm("DDI3: ".$list." DDI4: ");
        //dsm($mappings[$list][0]);
    }
    
    //dsm($mappings);
    return $mappings;
}