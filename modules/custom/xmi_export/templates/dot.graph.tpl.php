digraph G {
    fontname = "Bitstream Vera Sans"
    fontsize = 8

    overlap=false;

    compound=true;    
    
    node [
        fontname = "Bitstream Vera Sans"
        fontsize = 8
        shape = "record"
    ]

    edge [
        fontname = "Bitstream Vera Sans"
        fontsize = 8
        mode="ipsep"            
    ]
      
    
    <?php foreach($data['main'] as $packageName => $objects):?>
    
    <?php foreach($objects as $objectName => $object):?>
    <?php if(is_array($object)):?>
        
    <?php print GRAPH_PREFIX.$objectName;?> [
        label = "{<?php print _get_object_diagram_content($object);?>}"
    
        <?php if($link_type == 'page'):?>
        href="/graph/page/<?php print strtolower($packageName);?>/<?php print strtolower($objectName);?>"
        <?php elseif($link_type == 'svg'):?>
        href="/graph/svg/<?php print $packageName;?>/<?php print $objectName;?>"
        <?php endif;?>
        tooltip = "<?php print $packageName;?>:<?php print $objectName;?>"
    ]
    
    <?php endif;?>
    <?php endforeach; ?>
    <?php endforeach; ?>

  
    
    <?php foreach($data['extra'] as $packageName => $objects):?>
    
    subgraph <?php print $packageName;?> {
        label = "<?php print $packageName;?>"    
        
        node [ color = "#<?php print get_package_color($packageName);?>" ]
 
        <?php foreach($objects as $objectName => $object):?>
        <?php if(is_array($object)):?>

        <?php print GRAPH_PREFIX.$objectName;?> [
        
            label = "{<?php print _get_object_diagram_content($object);?>}"
        
            <?php if($link_type == 'page'):?>
            href="/graph/page/<?php print strtolower($packageName);?>/<?php print strtolower($objectName);?>"
            <?php elseif($link_type == 'svg'):?>
            href="/graph/svg/<?php print $packageName;?>/<?php print $objectName;?>"
            <?php endif;?>
            tooltip = "<?php print $packageName;?>:<?php print $objectName;?>"
        ];
        
        <?php endif;?>
        <?php endforeach; ?>    
    
    }
    <?php endforeach; ?>

    
    <?php foreach($data['main'] as $packageName => $objects):?>
    <?php foreach($objects as $objectName => $object):?>
    <?php if(is_array($object)):?>
    <?php if($object['extends']):?>
    
    <?php print  GRAPH_PREFIX.$object['name'];?> -> <?php print GRAPH_PREFIX.$object['extends'];?> [arrowhead=empty color="#000000" ];
    
    <?php endif;?>
    
    <?php foreach ($object['relationships'] as $relation):?>
    
    <?php print _render_relationship($objectName, $relation);?>
    
    <?php endforeach;?>
   
    <?php endif;?>
    <?php endforeach; ?>
    <?php endforeach; ?>      
}