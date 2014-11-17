<?php

function issues_page(){
    $objects = validate_objects();
    
    $total = 0;
    $errors = 0;
    $warnings = 0;
    
    foreach($objects as $package => $objs){
        foreach($objs as $object){
            $total++;
            if(array_key_exists("errors", $object)){
                $errors++;
            }
            if(array_key_exists("warnings", $object)){
                $warnings++;
            }            
        }
    }
    
    return theme("issues", array("objects" => $objects, 
                                 "total"=>$total, 
                                 "warnings" => $warnings,
                                 "errors" => $errors));
}

function validate_objects(){
    require_once('xmi_export.inc');
    $objects = get_all_node_objects();
        
    foreach($objects as $package => &$objs){
        foreach($objs as &$object){
            $object = validate_object($object);
        }
    }
    return $objects;
}

function validate_object($object){
    //check if first letter in object name is upper case
    if(!starts_with_upper($object["name"])){
        $object["errors"][] = "Object '".$object["name"]."' name first character should be upper case";
    }
    
    //check for whitespace
    if (preg_match('/\s/',$object["name"]) ){
        $object["errors"][] = "Object '".$object["name"]."' name contains whitespace";
    }
    
    if (!ctype_alpha($object["name"])){
        $object["errors"][] = "Object '".$object["name"]."' name contains non alpha characters";
    }
            
    //check if object have description
    
    
    foreach($object["properties"] as $key => $property){
        //check for whitespace
        if (preg_match('/\s/',$property["name"]) ){
            $object["errors"][] = "Property '".$property["name"]."' contains whitespace";
        }        
        
        if (starts_with_upper($property["name"])){
            $object["warnings"][] = "Property '".$property["name"]."' first character should be lower case";
        }
        
        if (!ctype_alpha($property["name"]) && !startsWith($property["name"], "xml:")){
            $object["errors"][] = "Property '".$property["name"]."' contains non alpha characters";
        }
        
        if (strlen(trim($property["datatype"])) == 0){
            $object["errors"][] = "Property '".$property["name"]."' have no datatype";
        }
        
        if (strlen(trim($property["cardinality"])) == 0){
            $object["errors"][] = "Property '".$property["name"]."' have no cardinality";
        }
    }
    
    //check relations
    foreach($object["relationships"] as $key => $relationship){
        if (preg_match('/\s/',$relationship["name"]) ){
            $object["errors"][] = "Relationship '".$relationship["name"]."' contains whitespace";
        }
        
        if (starts_with_upper($relationship["name"])){
            $object["warnings"][] = "Relationship '".$relationship["name"]."' first character should be lower case";
        }
        
        if (!ctype_alpha($relationship["name"])){
            $object["errors"][] = "Relationship '".$relationship["name"]."' contains non alpha characters";
        }

        if (strlen(trim($relationship["target_object"])) == 0){
            $object["errors"][] = "Relationship '".$relationship["name"]."' have no target object";
        }

        if (strlen(trim($relationship["type"])) == 0){
            $object["errors"][] = "Relationship '".$relationship["name"]."' have no type";
        }        
        
        if (strlen(trim($relationship["source_cardinality"])) == 0){
            $object["errors"][] = "Relationship '".$relationship["name"]."' have no source cardinality";
        }

        if (strlen(trim($relationship["target_cardinality"])) == 0){
            $object["errors"][] = "Relationship '".$relationship["name"]."' have no target cardinality";
        }
    } 
    
    return $object;
}

function startsWith($haystack, $needle){
    return $needle === "" || strpos($haystack, $needle) === 0;
}

function starts_with_upper($str) {
    $chr = mb_substr ($str, 0, 1, "UTF-8");
    return mb_strtolower($chr, "UTF-8") != $chr;
}