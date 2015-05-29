<?php
include('xmi_export.inc');

function sphinx_page(){
    generateFolders();
    return "build test";
}

function generateFolders(){
    $sphinx_dir = drupal_realpath('public://').'/sphinx/';
    drupal_mkdir($sphinx_dir);
    
    file_put_contents($sphinx_dir.'index.rst', createSphinxIndex());
    $packages = get_all_node_objects();
    
    
    foreach($packages as $packageName => $package){
        drupal_mkdir($sphinx_dir.$packageName);
        foreach($package as $objectName => $object){
            $package_index = createPackageIndex($package);
            file_put_contents($sphinx_dir.$packageName.'/index.rst', $package_index);
            if(is_array($object)){
                $object_rst = theme('sphinx.object', array('object' => $object));
                $file_path = $sphinx_dir.$packageName.'/'.$objectName.'.rst';
                file_put_contents($file_path, $object_rst);
            }
        }
    }
}


function createSphinxIndex(){
    $output =
        "====\n".
        "DDI4\n".
        "====\n".
        "\n".
        ".. image:: /images/ddi-logo.*\n".
        "\n".
        "Contents\n".
        "\n".
        ".. toctree::\n".
        "   :maxdepth: 2\n";
    $packages = get_all_packages();
    foreach($packages as $packageName => $package){
        $output .= '   '.$packageName.'/index.rst'."\n";
    }
    return $output;
}

function createPackageIndex($package){
    $output = _heading($package['name'], '*', true)."\n".
                _get_node_body($package['name'])."\n".
                "Contents \n".
                "\n".
                ".. toctree::\n".
                "   :maxdepth: 2\n".
                "\n";    
    foreach($package as $objectName => $object){

        if(is_array($object)){
            $output .= '   '.$objectName."\n";
        }
    }
    return $output;
}

function _heading($heading, $c, $double=false){
    $output = '';
    if($double){
        $output = str_pad($c, strlen($heading), $c)."\n";
    }
    $output .= $heading."\n".str_pad($c, strlen($heading), $c);
    return $output;
}