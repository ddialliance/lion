<?php
define('DOCBOOK_HEADER', '<?xml version="1.0" encoding="UTF-8"?>
<?xml-model href="http://docbook.org/xml/5.0/rng/docbook.rng" schematypens="http://relaxng.org/ns/structure/1.0"?>
<?xml-model href="http://docbook.org/xml/5.0/rng/docbook.rng" type="application/xml" schematypens="http://purl.oclc.org/dsdl/schematron"?>');

function docbook_export($package = null) {
    require_once('xmi_export.inc');
    $objects = get_all_node_objects();
    if($package != null){
        if(array_key_exists($package, $objects)){
            $tmp = $objects[$package];
            $objects = array();
            $objects[$package] = $tmp;
        }
    }      
    header("Content-type: text/xml; charset=utf-8");
    print DOCBOOK_HEADER;
    print theme('docbook', array('objects' => $objects));
    exit(0);
}

function docbook_html() {
    require_once('xmi_export.inc');
    $objects = get_all_node_objects();

    $docbook = theme('docbook', array('objects' => $objects));
    $docbook = str_replace('xmlns="http://docbook.org/ns/docbook" xmlns:xlink="http://www.w3.org/1999/xlink"', '', $docbook);

    header("Content-type: text/xml; charset=utf-8");

    print DOCBOOK_HEADER . '<?xml-stylesheet type="text/xsl" href="/' . drupal_get_path('module', 'xmi_export') . '/xslt/docbook/xhtml-1_1/docbook.xsl"?>' . $docbook;

    /*
      print DOCBOOK_HEADER.'<?xml-stylesheet type="text/xsl" href="/'.drupal_get_path('module', 'xmi_export').'/xslt/db2xhtml/xsl/main.xsl"?>'.$docbook;
     */


    //drupal_get_path('module', 'xmi_export').
    exit(0);
}

?>