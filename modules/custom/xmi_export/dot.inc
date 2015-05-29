<?php
require_once('xmi_export.inc');
define('GRAPH_PREFIX', 'DDI4_');

function graph_index(){
    $packages = get_all_packages();
    
    $output = "";
    foreach($packages as $name => $package){
        $output .= l($name, 'graph/page/'.$name).'<br/>';
    }
    $breadcrumb = array();
    $breadcrumb[] = l('Home', '<front>');
    $breadcrumb[] = 'Graph';    
    drupal_set_breadcrumb($breadcrumb);
    return $output;
}


function graph_page($format, $package, $object = null){
    $graph = '';
    $breadcrumb = array();
    $breadcrumb[] = l('Home', '<front>');
    $breadcrumb[] = l('Graph', 'graph');
    if(!empty($object)){
        $graph = generate_dot_for_object($object, $format);
        drupal_set_title($object);
        $breadcrumb[] = l($package, 'graph/page/'.$package);
        $breadcrumb[] = $object;
    }else if(!empty($package)){
        $graph = generate_dot_for_package($package, $format);
        drupal_set_title($package);
        $breadcrumb[] = $package;
    }
    
    drupal_set_breadcrumb($breadcrumb);
    
    switch($format){
        case 'dot':
            header("Content-Type: text/plain");
            print $graph;
            exit(0);
            break;
        case 'svg':
            dot_package_export_image($graph, $format);
            break;
        case 'page':
            return display_graph($graph);
            break;
    }
}


function generate_dot_for_package($package_slug, $link_type = 'page'){
    $all_objects = get_all_node_objects();
    $package = null;
    $to_build = array('main'=>array(), 'extra' => array());
    
    foreach($all_objects as $p => $classes){
        if(strtolower($p) == strtolower($package_slug)){
            $package = $p;
            $to_build['main'][$p] = $all_objects[$p];
            
        }
    }
    
    $to_build = _include_references($to_build, $all_objects);
    
    unset($to_build['extra'][$package]);
        
    return theme('dot.graph', array('data' => $to_build, 'link_type'=>$link_type));
}

function generate_dot_for_object($object_slug, $link_type = 'webb'){
    $all_objects = get_all_node_objects();
    $object_name = null;
    $package = null;
    $to_build = array('main' => array(), 'extra' => array());
    foreach($all_objects as $p => $objects){
        foreach($objects as $name => $object){
            if(strtolower($name) == strtolower($object_slug)){
                $object_name = $name;
                $package = $object['package'];
                $to_build['main'][$p][$name] = $object;
            }
        }
    }
    
    $to_build = _include_references($to_build, $all_objects);
    
    return theme('dot.graph', array('data' => $to_build, 'link_type'=>$link_type));
}

function display_graph($graph){
    $module_path = drupal_get_path('module', 'xmi_export');
    drupal_add_js($module_path . '/libs/viz/viz.js', 'file');
    drupal_add_js($module_path . '/js/graph.utils.js', 'file');
    
    $js = '
    jQuery(document).ready(function() {      
         jQuery("#graph").html(show_dot_graph("data", "svg"));
    })';
    drupal_add_js($js, 'inline');    
    
    return theme('dot', array('dot'=>$graph));
}


function _include_references($to_build, $all_objects){
    $refered = array();
    
    foreach($to_build['main'] as $package => $objects){
        foreach ($objects as $name => $object){
            $refered = array_merge($refered, _get_object_references($object, $all_objects));
        }
    }
    foreach($refered as $name => $object){
        $to_build['extra'][$object['package']][$name] = $object;
    }
    
    return $to_build;
}

function _get_object_references($object, $all_objects){
    $refered = array();
    
    if(array_key_exists('relationships', $object)){
        foreach($object['relationships'] as $relation_name => $relation){
            $target = $relation['target_object'];
            $refered[$target] = _get_referenced_object($target, $all_objects);
        }
    }
    if(array_key_exists('extends', $object)){
        $refered[$object['extends']] = _get_referenced_object($object['extends'], $all_objects);
    }
    
    return $refered;
}

function _get_referenced_object($object_name, $all_objects){
    foreach($all_objects as $package => $objects){
        foreach ($objects as $name => $object){
            if($object_name == $name){
                return $object;
            }
        }
    }
}


function dot_package_export_image($graph, $format) {
    switch ($format) {
        case "png":
            header("Content-Type: image/png");
            break;

        case "svg":
            header("Content-Type: image/svg+xml");
            break;
        default:
            header("Content-Type: image/svg+xml");
            $format = "svg";
    }
    
    $output = graphviz_filter_filterproc('
        /*
         * @formats = ' . $format . '
         * @imagemap = true
         *
         */' . $graph
    );

    $regex = '/https?\:\/\/[^\" ]+/i';
    preg_match_all($regex, $output, $matches);


    foreach ($matches[0] as $url) {
        if (substr($url, -8) === ".out." . $format) {
            print file_get_contents($url);
            break;
        }
    }

    exit(0);
}


function _get_object_diagram_content($object){
                $base_url = "/";
                $prefix = '';
                $properties = ' | ';
                
                if (count($object['properties']) > 0) {
                    foreach ($object['properties'] as $property) {
                        $properties .= '+ ' . $property['name'];
                        if ($property['datatype']) {
                            $properties .= ' : ' . $property['datatype'];
                        }
                        $properties .= ' \l ';
                        
                    }
                } else {
                    $properties .= "";
                }
                $properties .= ' \l ';

                if (count($object['relationships']) > 0) {
                    foreach ($object['relationships'] as $relation) {
                        $properties .= ' \l| <' . $object['name'] . $relation['name'] . '> ' . $relation['name'];
                        
                    }
                } else {
                    $properties .= "\l| ";
                }
                $properties .= ' \l ';

                return '<id>'.$object['name'] . $properties.'\l';
}


function _render_relationship($objectName, $relation){
    $prefix = GRAPH_PREFIX;
    $output = '';
    if ($relation['target_object']) {
        $color = '00000';
        $arrow = 'vee';
        $arrow_type = 'l';
        switch ($relation['type']) {
            case 'Aggregation':
                $arrow = 'odiamond';
                $side = 'side=l';
                $output .= "\t" . $prefix . $relation['target_object'] . ' -> ' . $prefix . $objectName . ':' . $objectName . $relation['name'] . ' [arrowhead=' . $arrow . ' labeldistance=1.9 taillabel="' . $relation['target_cardinality'] . '" headlabel="' . $relation['source_cardinality'] . '"  edgetooltip="' . $relation['name'] . '" fontcolor="black" color="#' . $color . '"];' . "\n";
                break;
            case 'Composition':
                $arrow = 'diamond';
                $side = 'side=r';

                $output .= "\t" . $prefix . $relation['target_object'] . ' -> ' . $prefix . $objectName . ':' . $objectName . $relation['name'] . ' [' . $side . ' arrowhead=' . $arrow . ' labeldistance=1.9 taillabel="' . $relation['target_cardinality'] . '" headlabel="' . $relation['source_cardinality'] . '"  edgetooltip="' . $relation['name'] . '" fontcolor="black" color="#' . $color . '"];' . "\n";
                break;
            default:
                $arrow = 'none';
                $color = '00000';
                $output .= "\t" . $prefix . $objectName . ':' . $objectName . $relation['name'] . ' -> ' . $prefix . $relation['target_object'] . ' [' . $side . ' arrowhead=' . $arrow . ' labeldistance=1.9 taillabel="' . $relation['source_cardinality'] . '" headlabel="' . $relation['target_cardinality'] . '"  edgetooltip="' . $relation['name'] . '" fontcolor="black" color="#' . $color . '"];' . "\n";
        }
    }
    return $output;
}

function get_package_color($package){
    $color = db_query("SELECT field_color_value 
                        FROM field_data_field_color fc
                        INNER JOIN node n ON n.nid = fc.entity_id 
                        WHERE LOWER(n.title) = :package", 
                        array('package'=>  strtolower($package)))
            ->fetchField();
    
    return $color;   
}