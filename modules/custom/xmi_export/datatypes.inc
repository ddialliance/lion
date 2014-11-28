<?php

function get_datatypes(){
    require_once('xmi_export.inc');
    
    //xs primitives will not be exported
    $skipTypes = array(
                       'xs:string', 
                       'xs:boolean', 
                       'xs:integer', 
                       'xs:double', 
                       'xs:float', 
                       'xs:decimal'
                    );
    
    $query = new EntityFieldQuery();
    
    $query->entityCondition('entity_type', 'node')
          ->propertyCondition('type', 'datatype')
          ->propertyCondition('status', NODE_PUBLISHED);
    
    $query->propertyCondition('title', $skipTypes, 'NOT IN');
    $query->propertyOrderBy('title', 'ASC');
    
    $result = $query->execute();
    
    $types = node_load_multiple(array_keys($result['node']));
    
    $datatypes = array();
    
    foreach($types as $t){
        $datatype = array();
        $datatype['name'] = $t->title;
        $datatype['package'] = _get_reference_title($t->field_package);
        
        if($t->field_datatype_enumeration){
            $datatype['enumeration'] = array();
            foreach($t->field_datatype_enumeration['und'] as $fu){
                $field = field_collection_item_load($fu['value']);
                $key         = _get_value($field->field_enumeration_value);
                $description = _get_value($field->field_enumeration_description);
                
                $datatype['enumeration'][$key]['value'] = $key;
                $datatype['enumeration'][$key]['description'] = $description;
            }
        }
        
        $datatypes[$t->title] = $datatype;
    }
    
    $datatypes = _put_into_packages($datatypes);

    return $datatypes;
}