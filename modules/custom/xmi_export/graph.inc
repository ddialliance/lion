<?php

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
        default:
            header("Content-Type: image/svg+xml");
            $format = "svg";
    }
       
    foreach($matches[0] as $url){
        if(substr($url, -8) === ".out.".$format){
            print file_get_contents($url);
            break;
        }
    }
    
    exit(0);
}

function _generate_dot($entityTitle = null, $type = "package") {
    global $base_url;

    set_time_limit(250);
    ini_set('memory_limit','512M');
    require_once('xmi_export.inc');
    
    if($type == "package" || $type == "view"){
        $objects = get_all_node_objects($entityTitle, $type);
    }else if($type == "object"){
        
    }

    return _generate_dot_graph($objects);
}

function entity_graph($entityTitle) {
    require_once('xmi_export.inc');
    
    $query = new EntityFieldQuery();
    
    $query->entityCondition('entity_type', 'node')
          ->propertyCondition('status', NODE_PUBLISHED);
    $query->propertyCondition('title', $entityTitle, '=');
    
    $result = $query->execute();
    
    if(count($result['node']) == 1){
        dsm($result['node']);
        $nid = key($result['node']);
        
    }
    
    return "";
}


function _generate_dot_graph($objects){
    $output .= 'digraph ddi4 {
        rankdir = "LR";
        layout=dot;
        overlap=false; //false, compress, ...
        sep=true;
        compound=true;

        graph [
            fontname = "helvetica"
        ];

        node [
            fillcolor= "gray"
            fontname = "helvetica"
            fontsize = "12"
            shape = "record"
            align = "left"
            mindist=0.5
        ];
        
        edge [
            fontname = "helvetica"
            fontsize = "10"
            minlen=1
            mode="ipsep"
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
            if(count($class['properties']) > 0){
                foreach ($class['properties'] as $property) {
                    $properties .= '+ ' . $property['name'];
                    if ($property['datatype']) {
                        $properties .= ':' . $property['datatype'];
                    }
                    $properties .= ' \l ';
                    $i++;
                }
            }else{
                $properties .= "(no properties)";
            }
            $properties .= ' \l ';

            if(count($class['relationships']) > 0){
                foreach ($class['relationships'] as $relation) {
                    $properties .= ' \l| <' . $class['name'] . $relation['name'] . '> ' . $relation['name'];
                    $i++;
                }
            }else{
                $properties .= "\l| (no relationships)";
            }
            $properties .= ' \l ';

            $output .= "\t" . $prefix.$class['name'] . ' [tooltip="'.$class['package'].':'.$class['name'].'" href="'.$base_url.'/ddiobjects/' . $class['name'] . '" label="<id> ' . $class['name'] . $properties . '" shape = "record" fillcolor="white" color="' . $package_color . '"];' . "\n";
        }
    }

    foreach ($objects as $package => $classes) {
        $output .= "\trank=";
        foreach ($classes as $class) {
            $output .= $prefix.$class['name'] . ';';
        }
        $output .= "\n";
    }

    $output .= "\n";

    foreach ($objects as $package => $classes) {
        foreach ($classes as $class) {

            //extensions
            if ($class['extends']) {
                $output .= "\t" . $prefix.$class['name'] . ' -> ' . $prefix.$class['extends'] . ' [arrowhead=empty color="#000000" ];' . "\n";
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
                            $output .= "\t" . $prefix.$relation['target_object'] . ' -> ' .  $prefix.$class['name'] . ':' . $class['name'] . $relation['name'] . ' [arrowhead=' . $arrow . ' labeldistance=1.9 taillabel="' . $relation['target_cardinality'] . '" headlabel="' . $relation['source_cardinality'] . '"  edgetooltip="' . $relation['name'] . '" fontcolor="black" color="#' . $color . '"];' . "\n";
                            break;
                        case 'Composition':
                            $arrow = 'diamond';
                            $side = 'r';
                            
                            $output .= "\t" . $prefix.$relation['target_object']  . ' -> ' . $prefix.$class['name'] . ':' . $class['name'] . $relation['name'] . ' [side='.$side.' arrowhead=' . $arrow . ' labeldistance=1.9 taillabel="' . $relation['target_cardinality'] . '" headlabel="' . $relation['source_cardinality'] . '"  edgetooltip="' . $relation['name'] . '" fontcolor="black" color="#' . $color . '"];' . "\n";
                            break;
                        default:
                            $arrow = 'none';
                            $color = '00000';
                            $output .= "\t" . $prefix.$class['name'] . ':' . $class['name'] . $relation['name'] . ' -> ' . $prefix.$relation['target_object'] . ' [side='.$side.' arrowhead=' . $arrow . ' labeldistance=1.9 taillabel="' . $relation['source_cardinality'] . '" headlabel="' . $relation['target_cardinality'] . '"  edgetooltip="' . $relation['name'] . '" fontcolor="black" color="#' . $color . '"];' . "\n";
                    }
                }
            }
        }
    }
    $output .= '}';
    
    return $output;    
}