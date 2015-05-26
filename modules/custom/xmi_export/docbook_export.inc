<?php
define('DOCBOOK_HEADER', '<?xml version="1.0" encoding="UTF-8"?>
<?xml-model href="http://docbook.org/xml/5.0/rng/docbook.rng" schematypens="http://relaxng.org/ns/structure/1.0"?>
<?xml-model href="http://docbook.org/xml/5.0/rng/docbook.rng" type="application/xml" schematypens="http://purl.oclc.org/dsdl/schematron"?>');

/**
 * Exports docbook xml for a specified package
 * @param type $package shortname for the package to export docbook xml for
 */
function docbook_export($package = null) {
    require_once('xmi_export.inc');
    require_once('mapping.inc');
    
    $objects = get_all_node_objects($package);
    if($package != null){
        if(array_key_exists($package, $objects)){
            $tmp = $objects[$package];
            $objects = array();
            $objects[$package] = $tmp;
        }
    }
    $build = _get_build_node();
    
    $mappings = array();
    $mappings["ddi3-2"] = get_mappings_array("ddi3-2");
    $mappings["gsim"] = get_mappings_array("gsim");
    
    header("Content-type: text/xml; charset=utf-8");
    print DOCBOOK_HEADER;
    print theme('docbook', array('objects' => $objects, 'mappings' => $mappings, 'build' => $build));
    exit(0);
}

/**
 * Exports all packages as a docbook rendered via the docbook xml->html xslt
 */
function docbook_html() {
    require_once('xmi_export.inc');
    require_once('mapping.inc');
    
    $objects = get_all_node_objects();

    $build = _get_build_node();
    
    $mappings = array();
    $mappings["ddi3-2"] = get_mappings_array("ddi3-2");
    $mappings["gsim"] = get_mappings_array("gsim");
    
    $docbook = theme('docbook', array('objects' => $objects, 'mappings' => $mappings, 'build' => $build));
    $docbook = str_replace('xmlns="http://docbook.org/ns/docbook" xmlns:xlink="http://www.w3.org/1999/xlink"', '', $docbook);

    header("Content-type: text/xml; charset=utf-8");

    print DOCBOOK_HEADER . '<?xml-stylesheet type="text/xsl" href="/' . drupal_get_path('module', 'xmi_export') . '/xslt/docbook/xhtml-1_1/docbook.xsl"?>' . $docbook;

    exit(0);
}

function _get_build_node(){
    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'build_release')
      ->propertyCondition('status', NODE_PUBLISHED);
    
    $result = $query->execute();

    if (isset($result['node'])) {
        $nids = array_keys($result['node']);
        $build = node_load($nids[0]);
        
        foreach ($build->field_author['und'] as $item) {
            $f = field_collection_item_load($item['value']);
            $f->entity_id = $item['value'];                
            $build->author[] = $f;
        }        
    }
    return $build;
}