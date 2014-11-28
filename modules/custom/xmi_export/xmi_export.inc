<?php

function xmi_export_page() {
    $objects = get_all_node_objects(null);
    dsm($objects);
    return '<br>This is a draft of the export of the objects to XMI 2.1: <a href="/xmi.xml">xmi.xml</a> (this is generated on the fly)';
}

function get_all_node_objects($entityTitle = null, $type = "package") {

    $last_update = db_query('SELECT changed FROM {node} ORDER BY changed DESC LIMIT 1')
                        ->fetchField();
    
    $objects = &drupal_static(__FUNCTION__);
    if (!isset($my_data)) {
      if ($cache = cache_get('xmi_export'.$last_update.'-'.$entityTitle.'-'.$type)) {
        $objects = $cache->data;
      }else {
        if($type == "view"){
            
            $result = db_query("SELECT field_object_reference_target_id AS nid
                                    FROM field_data_field_object_reference f
                                    INNER JOIN {node} n ON n.nid = f.entity_id
                                    WHERE n.status = :status
                                    AND n.title = :title
                                ", array(
                                    ':status' => 1,
                                    ':title' => $entityTitle
                                ));            
        }else if($type == "package"){
            $result = db_query("SELECT n.nid
                                FROM {node} n 
                                WHERE n.type = :type AND n.status = :status
                                AND n.nid IN (SELECT f.entity_id FROM field_data_field_package f 
                                                INNER JOIN node n ON n.nid = f.field_package_target_id 
                                                INNER JOIN field_data_field_include_in_build df ON n.nid = df.entity_id
                                                
                                                AND f.deleted = 0
                                                AND df.field_include_in_build_value = 1 
                                             )
                                ", array(
                                    ':type' => 'ddiobject',
                                    ':status' => 1,
                                ));
        }
        
        $objects = get_nodes($result);

        if($type == "package"){
            if($entityTitle != null){
                foreach ($objects as $package_name => $tmp){
                    if(strtolower($package_name) == strtolower($entityTitle)){
                        $objects = array();
                        $objects[$package_name] = $tmp;
                    }
                }
            }          
        }
        
        cache_set('xmi_export'.$last_update.'-'.$collection.'-'.$type, $objects, 'cache', time() + 3600);
      }
    }

    return $objects;
}

function get_all_packages(){
    $result = db_query("SELECT n.nid, n.title AS name, u.name AS creator  
                        FROM {node} n  INNER JOIN {users} u ON u.uid = n.uid 
                        WHERE n.title NOT IN('Deleted', 'Keep', 'Toss', 'Review', 'Achim-Test')
                        AND n.type = :type AND n.status = :status", array(
                            ':type' => 'package',
                            ':status' => 1,
                        ));    
    $packages = array();
    while($p = $result->fetchAssoc()){
        $packages[] = $p;
    }
    return $packages;
}

function get_nodes($result) {
    $objects = array();

    foreach ($result as $row) {
        $node = node_load($row->nid);
        
        if ($node->field_property) {
            $node->PROPERTIES = array();
            foreach ($node->field_property['und'] as $item) {
                $f = field_collection_item_load($item['value']);
                $f->entity_id = $item['value'];
                $node->PROPERTIES[] = $f; 
            }
        }

        if ($node->field_relationship) {
            $node->RELATIONSHIP = array();
            foreach ($node->field_relationship['und'] as $item) {
                $f = field_collection_item_load($item['value']);
                $f->entity_id = $item['value'];                
                $node->RELATIONSHIP[] = $f;
            }
        }
        
        if ($node->field_rdf_mapping) {
            $node->RDF_MAPPING = array();
            foreach ($node->field_rdf_mapping['und'] as $item) {
                $f = field_collection_item_load($item['value']);
                $f->entity_id = $item['value'];                
                $node->RDF_MAPPING[] = $f;
            }
        }        

       $arr = node_to_xmi_array($node);

       $objects[$node->title] = $arr;
        
    }
    
    return _put_into_packages($objects);
}

function get_nodes_by_status($tid) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'ddiobject')
            ->fieldCondition('field_status', 'tid', $tid);

    return $query->execute();
}

function xmi_export_xmi($package = null) {
    $objects = get_all_node_objects($package);
    if($package != null){
        if(array_key_exists($package, $objects)){
            $tmp = $objects[$package];
            $objects = array();
            $objects[$package] = $tmp;
        }
    }    
    
    header("Content-type: text/xml; charset=utf-8");
    
    print theme('xmi', array('objects' => $objects));
    exit(0);
}

/**
 * Restreucture the list to an nested array of packages
 * @param type $objects
 * @return type
 */
function _put_into_packages($objects){
    $result = array();
    foreach($objects as $object){
        $result[$object['package']][] = $object;
    }
    return $result;
}

/**
 * Pulls fields from a drupal node and creates an array of fields used
 * in the export
 * @param type $node
 * @return array() associative array
 */
function node_to_xmi_array($node) {
    $xmi = array('definition' => "", 
                 'name' => "", 
                 'package' => "",
                 'properties' => array(),
                 'relationships' => array(),
                 'is_abstract' => 'false'
                );
    $xmi['name'] = $node->title;
    $xmi['nid'] = $node->nid;
    $xmi['is_abstract'] = _get_value($node->field_is_abstract);
    $xmi['ddi_3_2'] = _get_value($node->field_ddi_3_2);
    $xmi['gsim'] = _get_value($node->field_gsim);
    $xmi['definition']  = _get_value($node->body);
    
    $extends_info = _get_reference_info($node->field_extends);
    
    $xmi['extends'] = $extends_info['title'];
    $xmi['extends_uuid'] = $extends_info['uuid'];
    
    $xmi['package'] = _get_reference_title($node->field_package);
    $xmi['uuid'] = $node->uuid;
    
    if (property_exists($node, 'PROPERTIES')) {
        if ($node->PROPERTIES) {
            $xmi['properties'] = array();
            foreach ($node->PROPERTIES as $prop) {
                $property = array('name'=>"", 'description'=>"", 'cardinality'=>"", 'datatype'=>"");

                $property['name']           = _get_value($prop->field_property_name);
                $property['description']    = _get_value($prop->field_property_description_long);
                $property['cardinality']    = _get_value($prop->field_property_cardinality);
                $property['entity_id']      = $prop->entity_id;
                
                $datatype_info = _get_reference_info($prop->field_property_datatype);
                $property['datatype']       = $datatype_info['title'];
                $property['datatype_uuid']       = $datatype_info['uuid'];

                $xmi['properties'][$property['name']] = $property;

            }
        }
    }
    if (property_exists($node, 'RELATIONSHIP')) {

        $xmi['relationships'] = array();
        foreach ($node->RELATIONSHIP as $rel) {
            $relationship = array('name'=>"",'target_object'=>"", 'source_cardinality'=>"", 'target_cardinality'=>"", 'type'=>"", 'description'=>"");
            
            $relationship['name']               = _get_value($rel->field_relationship_name);
            $relationship['source_cardinality'] = _get_value($rel->field_relationship_source_cardin);
            $relationship['target_cardinality'] = _get_value($rel->field_relationship_target_cardin);
            $relationship['entity_id']          = $rel->entity_id;
            $relationship['type']               = _get_value($rel->field_relationship_type);
            
            //composite
            //shared    aggregation
            //none    
            switch($relationship['type']){
                case 'Aggregation':
                    $relationship['xmi_type'] = 'shared';
                    break;
                case 'Composition':
                    $relationship['xmi_type'] = 'composite';
                    break;
                default:
                    $relationship['xmi_type'] = 'none';
            }
            
            $relationship['description']        = _get_value($rel->field_relationship_description_l);
            
            $target_object_info = _get_reference_info($rel->field_relationship_target_object);
            
            $relationship['target_object'] = $target_object_info['title'];
            $relationship['target_object_uuid'] = $target_object_info['uuid'];
            
            $xmi['relationships'][$relationship['name']] = $relationship;
        }
    }
    
    if (property_exists($node, 'RDF_MAPPING')) {
        if ($node->RDF_MAPPING) {
            $xmi['rdf_mapping'] = array();
            foreach ($node->RDF_MAPPING as $m) {
                $mapping = array('label'=>"",'type'=>"", 'uri'=>"");
                $mapping['label'] = _get_value($m->field_mapping_label);
                $mapping['type']  = _get_value($m->field_mapping_type);
                $mapping['uri']   = _get_value($m->field_mapping_uri);
                 $xmi['rdf_mapping'][] = $mapping;
            }
        }    
    }

    return $xmi;
}

/**
 * Helper function to get value from a field array
 * @param type $field
 * @return string the value of the field
 */
function _get_value($field){
    if (array_key_exists('und', $field)) {
        return $field['und'][0]['value'];
    }else{
        return '';
    }
}
/**
 * Helper function to get the title of the node from a reference field
 * @param type $field
 * @return string title of the referenced object
 */
function _get_reference_title($field){
    $result = '';
    if (array_key_exists('und', $field)) {
        $nid = $field['und'][0]['target_id'];
        $result = db_query("SELECT n.title  FROM {node} n  WHERE n.nid = :nid", array(':nid' => $nid))->fetchField();
    }
    return $result;
}

/**
 * Helper function to get title and uuid for a referenced object
 * @param type $field
 * @return array() title and uuid for the referenced node
 */
function _get_reference_info($field){
    $fields = array('title' => '', 'uuid'=> '');
    if (array_key_exists('und', $field)) {
        $nid = $field['und'][0]['target_id'];
        $result = db_query("SELECT n.title, n.uuid  FROM {node} n  WHERE n.nid = :nid", array(':nid' => $nid));
        $fields = $result->fetchAssoc();
    }
    return $fields;
}

/**
 * Helper function for render a datatype
 * @param type $type
 * @return string
 */
function _render_datatype($type){
    $type = trim($type);
    $output = '<type xmi:type="'.$type.'"/>';
    
    if(substr($type, 0, 3 ) === "xs:"){
        switch($type){
            case "xs:string":
            case "xs:boolean":
            case "xs:integer":
                $parts = explode(':', $type);
                $uctype = ucfirst ($parts[1]);
                $output = '<type xmi:type="uml:PrimitiveType" href="http://schema.omg.org/spec/UML/2.1/uml.xml#'.$uctype.'"/>';
                break;
            case "xs:double":
            case "xs:float":
            case "xs:decimal":
                $output = '<type xmi:type="uml:PrimitiveType" href="http://schema.omg.org/spec/UML/2.1/uml.xml#Real"/>';
                break;
            default:
                 $output = '<type xmi:type="uml:PrimitiveType" href="http://schema.omg.org/spec/UML/2.1/uml.xml#UnlimitedNatural"/>';
        }
    }
    
    return $output;
}

function _escape_for_export($string){
    $string = str_replace(':', '_', $string);
    return $string;
}