<?php

function graph_json() {
    
    $color = array('FF7400', 'FFAA00', 'FF0000', '009999', '33CCCC', 'FF7373', 'FFD073', 'FFB273', 'A60000');

    require_once('xmi_export.inc');
    $objects = get_all_node_objects();

    $json = array();

    foreach ($objects as $package => $classes) {

        foreach ($classes as $class) {
            $o = new stdClass();
            $o->id = $class['name'];
            $json['nodes'][] = array('data' => $o);

            if ($class['extends']) {
                $o = new stdClass();
                $o->class = $class['name'];
                $o->extends = $class['extends'];
                $json['extensions'][] = array('data' => $o);
            }


            foreach ($class['relationships'] as $relation) {
                if ($relation['target_object']) {
                    $o = new stdClass();
                    $o->source = $class['name'];
                    $o->target = $relation['target_object'];
                    $json['edges'][] = array('data' => $o);
                }
            }
        }
    }

    header('Content-Type: application/json');
    print json_encode($json);
    exit(0);
}

function dot_url_list(){
    global $base_url;
    require_once('xmi_export.inc');
    header("Content-Type: text/plain");
    $packages = get_all_packages();
    
    foreach($packages as $package){
        print $base_url.'/package/'.  strtolower($package['name']).'/dot.txt/'.$package['name'].".dot\n";
    }
    exit(0);
}

function graph_dot() {
    header("Content-Type: text/plain");
    print _generate_dot();
    exit();
}

function dot_package_export($package){
    $output = _generate_dot($package);
    header("Content-Type: text/plain");
    print $output;
    exit(0);
}

function dot_package_export_image($package, $format){
    
    $graph = _generate_dot($package);
    
    $output = graphviz_filter_filterproc('
        /*
         * @formats = '.$format.'
         * @imagemap = true
         *
         */'.$graph
    );
    
    $regex = '/https?\:\/\/[^\" ]+/i';
    preg_match_all($regex, $output, $matches);
    switch($format){
        case "png":
            header("Content-Type: image/png");
            break;
        
        case "svg":
            header("Content-Type: image/svg+xml");
            break;
    }
    
    
    foreach($matches[0] as $url){
        if(substr($url, -8) === ".out.".$format){
            print file_get_contents($url);
            break;
        }
    }
    
    exit(0);
}

function _generate_dot($collection = null, $type = "package") {
    global $base_url;

    set_time_limit(250);
    ini_set('memory_limit','512M');
    require_once('xmi_export.inc');
    $objects = get_all_node_objects($collection, $type);


    $output .= 'digraph ddi4 {
        rankdir = "LR";
        node [
            fillcolor= "gray"
            fontsize = "12"
            shape = "record"
            align = "left"
        ];
';

    $prefix = 'DDI_';
    
    
    //Classes 

    foreach ($objects as $package => $classes) {
        $rand = array('0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f');
        $package_color = '#' . $rand[rand(0, 15)] . $rand[rand(0, 15)] . $rand[rand(0, 15)] . $rand[rand(0, 15)] . $rand[rand(0, 15)] . $rand[rand(0, 15)];
        
        foreach ($classes as $class) {
            $properties = ' | ';
            $i = 2;
            foreach ($class['properties'] as $property) {
                $properties .= '+ ' . $property['name'];
                if ($property['datatype']) {
                    $properties .= ':' . $property['datatype'];
                }
                $properties .= ' \l ';
                $i++;
            }
            $properties .= ' \l ';

            foreach ($class['relationships'] as $relation) {
                $properties .= ' \l| <' . $class['name'] . $relation['name'] . '> ' . $relation['name'];
                $i++;
            }
            $properties .= ' \l ';

            $output .= "\t" . $class['name'] . ' [tooltip="'.$class['package'].':'.$class['name'].'" href="'.$base_url.'/ddiobjects/' . $class['name'] . '" label="<id> ' . $class['name'] . $properties . '" shape = "record" fillcolor="white" color="' . $package_color . '"];' . "\n";
        }
    }

    foreach ($objects as $package => $classes) {
        $output .= "\trank=";
        foreach ($classes as $class) {
            $output .= $class['name'] . ';';
        }
        $output .= "\n";
    }

    $output .= "\n";

    foreach ($objects as $package => $classes) {

        foreach ($classes as $class) {

            //extensions
            if ($class['extends']) {
                $output .= "\t" . $class['name'] . ' -> ' . $class['extends'] . ' [arrowhead=empty color="#000000" ];' . "\n";
            }

            //relations
            foreach ($class['relationships'] as $relation) {
                if ($relation['target_object']) {
                    $color = '';
                    $arrow = 'vee';
                    $arrow_type = 'l';
                    switch ($relation['type']) {
                        case 'Aggregation':
                            $arrow = 'odiamond';
                            $side = 'l';
                            $output .= "\t" . $relation['target_object'] . ' -> ' .  $class['name'] . ':' . $class['name'] . $relation['name'] . ' [arrowhead=' . $arrow . ' labeldistance=1.9 taillabel="' . $relation['target_cardinality'] . '" headlabel="' . $relation['source_cardinality'] . '"  edgetooltip="' . $relation['name'] . '" fontcolor="black" color="#' . $color . '"];' . "\n";
                            break;
                        case 'Composition':
                            $arrow = 'diamond';
                            $side = 'r';
                            $output .= "\t" . $class['name'] . ':' . $class['name'] . $relation['name'] . ' -> ' . $relation['target_object'] . ' [side='.$side.' arrowhead=' . $arrow . ' labeldistance=1.9 taillabel="' . $relation['target_cardinality'] . '" headlabel="' . $relation['source_cardinality'] . '"  edgetooltip="' . $relation['name'] . '" fontcolor="black" color="#' . $color . '"];' . "\n";
                            break;
                        default:
                            $arrow = 'open';
                            $color = '00000';
                            $output .= "\t" . $class['name'] . ':' . $class['name'] . $relation['name'] . ' -> ' . $relation['target_object'] . ' [side='.$side.' arrowhead=' . $arrow . ' labeldistance=1.9 taillabel="' . $relation['target_cardinality'] . '" headlabel="' . $relation['source_cardinality'] . '"  edgetooltip="' . $relation['name'] . '" fontcolor="black" color="#' . $color . '"];' . "\n";
                    }
                    
                }
            }
        }
    }
    $output .= '}';
    
    return $output;
}

function dot_graph_page() {
    drupal_add_js(drupal_get_path('module', 'xmi_export') . '/libs/viz/viz.js', 'file');
    drupal_add_js(drupal_get_path('module', 'xmi_export') . '/js/graph.utils.js', 'file');
    $js = '
    jQuery(document).ready(function() {      
         jQuery("#graph").html(show_dot_graph("data", "svg"));
    })';
    drupal_add_js($js, 'inline');

    //return theme('dot', array('dot' => _generate_dot()));
    return 'temporarily out of order';
}


function graph_page() {
    drupal_add_js(drupal_get_path('module', 'xmi_export') . '/libs/dracula/raphael-min.js', 'file');
    drupal_add_js(drupal_get_path('module', 'xmi_export') . '/libs/dracula/dracula_graffle.js', 'file');
    drupal_add_js(drupal_get_path('module', 'xmi_export') . '/libs/dracula/dracula_graph.js', 'file');
    drupal_add_js(drupal_get_path('module', 'xmi_export') . '/libs/dracula/dracula_algorithms.js', 'file');
    $js = '
           jQuery(document).ready(function() {
            var g = new Graph();
            g.edgeFactory.template.style.directed = true;        

            jQuery.getJSON("/objects.json", function(data) {
               jQuery.each(data.nodes, function(i,node){
                 console.log(node.data.id);
                 g.addNode(node.data.id);
               });
               jQuery.each(data.edges, function(i,edge){
                 g.addEdge(edge.data.source, edge.data.target);
               });
               jQuery.each(data.extensions, function(i,extend){
                 g.addEdge(extend.data.extends, extend.class);
               });
               //var layouter = new Graph.Layout.Ordered(g, topological_sort(g));
               var layouter = new Graph.Layout.Spring(g);
               layouter.layout();
               var renderer = new Graph.Renderer.Raphael("canvas", g, 1000, 900);
            });
          });';
    drupal_add_js($js, 'inline');
    return '<div id="canvas"></div><br/>Rendered using <a href="http://www.graphdracula.net/">Dracula Graph Library</a>';
}

function graph_arbor_page() {
    drupal_add_js(drupal_get_path('module', 'xmi_export') . '/libs/arbor/arbor.js', 'file');

    return '<canvas id="viewport" width="800" height="600"></canvas>';
}

?>
