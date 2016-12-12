<?php $cardinality_parts = explode('..', $cardinality);?>

<?php if($cardinality_parts[0] == '0'):?>
    <lowerValue xmi:type="uml:LiteralInteger" xmi:id="<?php print $object;?>_<?php print $property;?>_lower"/>
<?php elseif(is_numeric($cardinality_parts[0])):?>
    <lowerValue xmi:type="uml:LiteralInteger" xmi:id="<?php print $object;?>_<?php print $property;?>_lower" value="<?php print $cardinality_parts[0];?>"/>
<?php endif;?>

<?php if($cardinality_parts[1] == 'n'):?>
    <upperValue xmi:type="uml:LiteralUnlimitedNatural" xmi:id="<?php print $object;?>_<?php print $property;?>_upper" value="-1"/>
<?php elseif(is_numeric($cardinality_parts[1])):?>
    <upperValue xmi:type="uml:LiteralInteger" xmi:id="<?php print $object;?>_<?php print $property;?>_upper" value="<?php print $cardinality_parts[1];?>"/> 
<?php endif;?> 