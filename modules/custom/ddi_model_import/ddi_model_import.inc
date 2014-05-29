<?php

function ddi_model_import_page(){
    set_time_limit(900);
    require_once('libraries/Excel/reader.php');

    // ExcelFile($filename, $encoding);
    $data = new Spreadsheet_Excel_Reader();

    $file = drupal_realpath('public://') . '/import_3.xls';

    // Set output Encoding.
    $data->setOutputEncoding('CP1251');

    $data->read($file);

    //dsm($data);

    $import = array();

    //put all cells into the structure
    foreach ($data->boundsheets as $k => $sheet){
        $import[$sheet['name']] = array();
        if($sheet['name'] == 'Object'){
            for ($i = 2; $i <= $data->sheets[$k]['numRows']; $i++){
                $col = 1;
                $import['Object'][$data->sheets[$k]['cells'][$i][1]] = array();
                foreach ($data->sheets[$k]['cells'][1] as $row => $cell){
                    $import['Object'][$data->sheets[$k]['cells'][$i][1]][$cell] = $data->sheets[$k]['cells'][$i][$col];
                    $col++;
                }
            }
        }else if($sheet['name'] == 'Property' || $sheet['name'] == 'Relationship'){
            for ($i = 2; $i <= $data->sheets[$k]['numRows']; $i++){
                $col = 1;
                $arr = array();
                foreach ($data->sheets[$k]['cells'][1] as $row => $cell){
                    $arr[$cell] = $data->sheets[$k]['cells'][$i][$col];
                    $col++;
                }
                
                if(array_key_exists($arr['Object Name'], $import['Object']) == false){
                    //drupal_set_message('Error setting '.$sheet['name'].',  '.$arr['Object Name'].' is not an Object');
                }else{
                
                    $import['Object'][$arr['Object Name']][$sheet['name']][] = $arr;
                }
            }
        }
    }
    
    $package_nids['K'] = _get_nid_by_title('Keep');
    $package_nids['T'] = _get_nid_by_title('Toss');
    $package_nids['R'] = _get_nid_by_title('Review');
    

    dsm($import);
    $log = array();
    
    $import_properties = true;
    $import_relationships = true;
    
    /*
    //create objects witch does not extends any other objects first
    foreach ($import['Object'] as $object){
        if (empty($object['Object Extends'])){
            $log['object'][$object['Object Name']] = _create_ddi_object($object, $package_nids[$object['KTR']]);
            if($log['object'][$object['Object Name']]['status'] == 'error'){
                $import_properties = false;
            }
        }
    }

    //create objects witch extends other objects
    foreach ($import['Object'] as $object){
        if (!empty($object['Object Extends'])){
            $log['object'][$object['Object Name']] = _create_ddi_object($object, $package_nids[$object['KTR']]);
            if($log['object'][$object['Object Name']]['status'] == 'error'){
                $import_properties = false;
            }            
        }
    }*/
    
    /*
    if($import_properties){
        //create properties
        //create objects witch extends other objects
        foreach ($import['Object'] as $object){
            if(array_key_exists('Property', $object)){
                $log['object'][$object['Object Name']] = _set_ddi_object_properties($object);
                if($log['object'][$object['Object Name']]['status'] == 'error'){
                   $import_relationships = false;
               }                 
            }
        }
    }*/
    
    /*
    if($import_relationships){
        //create relations
        foreach ($import['Object'] as $object){
            if(array_key_exists('Relationship', $object)){
                $log['object'][$object['Object Name']] = _set_ddi_object_relationships($object);
            }
        }
    }*/
     
        
    
    return theme('import.log', array('log' => $log));
}

function _create_ddi_object($object, $package_nid){
    $result = $object;
    $result['status'] = NULL;

    if(!is_numeric(_get_nid_by_title($object['Object Name']))){
        global $user;
        $values = array(
          'type' => 'ddiobject',
          'uid' => $user->uid,
          'status' => 1,
          'comment' => 1,
          'promote' => 0,
        );
        $entity = entity_create('node', $values);
        $ewrapper = entity_metadata_wrapper('node', $entity);
        $ewrapper->title->set(trim($object['Object Name']));
        
        $ewrapper->body->set(array('value' => trim($object['Description'])));

        if($object['DDI 3.2 mapping'] != NULL){
            $ewrapper->field_ddi_3_2->set($object['DDI 3.2 mapping']);
        }
        
        //set package (defaults to Import (nid: 97))
        if(is_numeric($package_nid)){
            $ewrapper->field_package->set(intval($package_nid));
        }else{
            $ewrapper->field_package->set(intval(97));
        }
        

        //If object extends
        if (!empty($object['Object Extends'])){
            $extends_nid = _get_nid_by_title(trim($object['Object Extends']));
            if ($extends_nid){
                $ewrapper->field_extends->set(intval($extends_nid));
                $result['status'] = 'ok';
            }
            else{
                $result['status'] = 'error';
                $result['status message'] = 'Can´t extend "' . $object['Object Extends'] . '" node not found';
            }
        }else{
            $result['status'] = 'ok';
            $result['status message'] = 'No extension';
        }
        if($result['status'] == 'ok'){
            $ewrapper->save();
            $result['status message'] .= ', Created';
        }
    }else{
        $result['status'] = 'notice';
        $result['status message'] = 'Already exists';
    }
    
    return $result;
}

function _set_ddi_object_properties($object){
    $result = $object;
    $result['status'] = NULL;
    
    $nid = _get_nid_by_title($object['Object Name']);
    if(is_numeric($nid)){
        $node = node_load($nid);
 
        foreach($object['Property'] as $property){
            
            $datatype_nid = _get_nid_by_title($property['Property Datatype']);
            if(is_numeric($datatype_nid)){
                //create a new field collection item
                $field_collection_item = entity_create('field_collection_item', array('field_name' => 'field_property'));
                $field_collection_item->setHostEntity('node', $node);

                //Property name
                $field_collection_item->field_property_name[LANGUAGE_NONE][]['value'] = $property['Property Name'];

                //Datatype
                $field_collection_item->field_property_datatype[LANGUAGE_NONE][0]['target_id'] = $datatype_nid;

                //Description
                $field_collection_item->field_property_description_long[LANGUAGE_NONE][]['value'] = $property['Description'];

                $cardinality = $property['min'].'..'.$property['max'];
                if(substr($cardinality, 0, 1) == '.'){
                    $cardinality = '0'.$cardinality;
                }
                
                //Cardinality
                $field_collection_item->field_property_cardinality[LANGUAGE_NONE][]['value'] = $cardinality;

                //save the filed collection
                $field_collection_item->save();
                $node->field_property[LANGUAGE_NONE][]['value'] = $field_collection_item->item_id;
            }else{
                $result['status'] = 'error';
            }
        }
        try{
            node_save($node);
        }catch(Exception $e){
            $result['status'] = 'error';
            $result['status message'] = 'Can not save node';            
        }
    }else{
        $result['status'] = 'error';
        $result['status message'] = $object['Object Name'].' is not a node';
        watchdog('ddi_model_import', $object['Object Name'].' is not a node');
    }
    
    return $result;
}


function _set_ddi_object_relationships($object){
    $result = $object;
    $result['status'] = NULL;
    
    $nid = _get_nid_by_title($object['Object Name']);
    if(is_numeric($nid)){
        $node = node_load($nid);
 
        foreach($object['Relationship'] as $relationship){
            
            $target_nid = _get_nid_by_title($relationship['Target Object']);
            if(is_numeric($target_nid)){
                //create a new field collection item
                $field_collection_item = entity_create('field_collection_item', array('field_name' => 'field_relationship'));
                $field_collection_item->setHostEntity('node', $node);

                //Relationship name
                $field_collection_item->field_relationship_name[LANGUAGE_NONE][]['value'] = $relationship['Relationship Name'];

                //Datatype
                $field_collection_item->field_relationship_target_object[LANGUAGE_NONE][0]['target_id'] = $target_nid;

                //Description
                $field_collection_item->field_relationship_description_l[LANGUAGE_NONE][]['value'] = $relationship['Description'];

                $cardinality = $relationship['Source Cardinality'];
                if(substr($cardinality, 0, 1) == '.'){
                    $cardinality = '0'.$cardinality;
                }
                
                if($cardinality != '0..'){
                    //Cardinality
                    $field_collection_item->field_relationship_source_cardin[LANGUAGE_NONE][]['value'] = $cardinality;                   
                }
                


                //save the filed collection
                $field_collection_item->save();
                $node->field_relationship[LANGUAGE_NONE][]['value'] = $field_collection_item->item_id;
            }else{
                $result['status'] = 'error';
            }
        }
        
        try{
            node_save($node);
            $result['status'] = 'ok';
            $result['status message'] = 'Node saved';   
        }catch(Exception $e){
            $result['status'] = 'error';
            $result['status message'] = 'Can not save node';            
        }
    }else{
        $result['status'] = 'error';
        $result['status message'] = $object['Object Name'].' is not a node';
    }
    
    return $result;
}


function _get_nid_by_title($title){
    $nid = db_query('SELECT nid FROM node WHERE title = :mytitle', array(':mytitle' => $title))->fetchField();

    if (is_numeric($nid)){
        return $nid;
    }
    else{
        return false;
    }
}