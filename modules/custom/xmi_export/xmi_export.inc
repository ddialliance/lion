<?php

function xmi_export_page() {
    $objects = get_all_node_objects();
    return '<br>This is a draft of the export of the objects to XMI 2.1: <a href="/xmi.xml">xmi.xml</a> (this is generated on the fly)';
}

function get_all_node_objects($package = null) {

    $result = db_query("SELECT n.nid, n.title AS name, u.name AS creator  
                        FROM {node} n  INNER JOIN {users} u ON u.uid = n.uid 
                        WHERE n.type = :type AND n.status = :status", array(
                            ':type' => 'ddiobject',
                            ':status' => 1,
                        ));
    $objects = get_nodes($result);
    
    if($package != null){
        if(array_key_exists($package, $objects)){
            $tmp = $objects[$package];
            $objects = array();
            $objects[$package] = $tmp;
        }
    }
    
    return $objects;
}

function get_all_packages(){
    $result = db_query("SELECT n.nid, n.title AS name, u.name AS creator  
                        FROM {node} n  INNER JOIN {users} u ON u.uid = n.uid 
                        WHERE n.type = :type AND n.status = :status", array(
                            ':type' => 'package',
                            ':status' => 1,
                        ));    
}

function get_nodes($result) {
    $objects = array();

    foreach ($result as $row) {
        $node = node_load($row->nid);

        if ($node->field_property) {
            $node->PROPERTIES = array();
            foreach ($node->field_property['und'] as $item) {
                $node->PROPERTIES[] = field_collection_item_load($item['value']);
            }
        }

        if ($node->field_relationship) {
            $node->RELATIONSHIP = array();
            foreach ($node->field_relationship['und'] as $item) {
                $node->RELATIONSHIP[] = field_collection_item_load($item['value']);
            }
        }
        $objects[$row->name] = node_to_xmi_array($node);
    }
    
    return _put_into_packages($objects);
    //return $objects;
}

function get_nodes_by_status($tid) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'ddiobject')
            ->fieldCondition('field_status', 'tid', $tid);

    return $query->execute();
}

function xmi_export_xmi($package = null) {
    $objects = get_all_node_objects();
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
 * @return type
 */
function node_to_xmi_array($node) {
    $xmi = array('defenition', 
                 'name', 
                 'package',
                 'properties' => array(),
                 'relationships' => array(),
                 'is_abstract' => 'false'
                );
    $xmi['name'] = $node->title;
    $xmi['nid'] = $node->nid;
    $xmi['is_abstract'] = _get_value($node->field_is_abstract);
    $xmi['definition']  = _get_value($node->body);
    
    $xmi['extends'] = _get_reference_title($node->field_extends);  
    $xmi['package'] = _get_reference_title($node->field_package);
    
    if (property_exists($node, 'PROPERTIES')) {
        if ($node->PROPERTIES) {
            $xmi['properties'] = array();
            foreach ($node->PROPERTIES as $prop) {
                $property = array('name', 'description', 'cardinality', 'datatype');

                $property['name']           = _get_value($prop->field_property_name);
                $property['description']    = _get_value($prop->field_property_description);
                $property['cardinality']    = _get_value($prop->field_property_cardinality);

                $property['datatype']       = _get_reference_title($prop->field_property_datatype);

                $xmi['properties'][] = $property;
            }
        }
    }
    if (property_exists($node, 'RELATIONSHIP')) {

        $xmi['relationships'] = array();
        foreach ($node->RELATIONSHIP as $rel) {
            $relationship = array('name','target_object', 'source_cardinality', 'target_cardinality', 'type', 'description');
            
            $relationship['name']               = _get_value($rel->field_relationship_name);
            $relationship['source_cardinality'] = _get_value($rel->field_relationship_source_cardin);
            $relationship['target_cardinality'] = _get_value($rel->field_relationship_target_cardin);
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
            
            $relationship['description']        = _get_value($rel->field_relationship_description);
            
            $relationship['target_object'] = _get_reference_title($rel->field_relationship_target_object);
            $xmi['relationships'][] = $relationship;
        }
    }

    return $xmi;
}

/**
 * Helper function to get value from a field array
 * @param type $field
 * @return string
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
 * @return type
 */
function _get_reference_title($field){
    $result = '';
    if (array_key_exists('und', $field)) {
        $nid = $field['und'][0]['target_id'];
        $result = db_query("SELECT n.title  FROM {node} n  WHERE n.nid = :nid", array(':nid' => $nid))->fetchField();
    }
    return $result;
}

function _render_datatype($type){
    $output = '<type xmi:type="'.$type.'"/>';
    
    if(strstr('xs:')){
        switch($type){
            case 'xs:string':
            case 'xs:boolean':
            case 'xs:integer':
                $parts = explode(':', $type);
                $uctype = ucfirst ($parts[1]);
                $output = '<type xmi:type="uml:PrimitiveType" href="http://schema.omg.org/spec/UML/2.1/uml.xml#'.$uctype.'"/>';
                break;
            case 'xs:double':
            case 'xs:float':
                $output = '<type xmi:type="uml:PrimitiveType" href="http://schema.omg.org/spec/UML/2.1/uml.xml#Real"/>';
                break;
            default:
                 $output = '<type xmi:type="uml:PrimitiveType" href="http://schema.omg.org/spec/UML/2.1/uml.xml#UnlimitedNatural"/>';
        }
    }
    
    return $output;
}