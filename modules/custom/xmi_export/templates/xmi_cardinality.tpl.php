<?php if($cardinality == '0..n'): ?>
   <lowerValue xmi:type="uml:LiteralInteger"          xmi:id="<?php print $object;?>_<?php print $property;?>_lower"/>
   <upperValue xmi:type="uml:LiteralUnlimitedNatural" xmi:id="<?php print $object;?>_<?php print $property;?>_upper" value="-1"/>                        
<?php endif; ?>
<?php if($cardinality == '1..n'): ?>
   <lowerValue xmi:type="uml:LiteralInteger"          xmi:id="<?php print $object;?>_<?php print $property;?>_lower" value="1"/>
   <upperValue xmi:type="uml:LiteralUnlimitedNatural" xmi:id="<?php print $object;?>_<?php print $property;?>_upper" value="-1"/>                        
<?php endif; ?>
<?php if($cardinality == '2..n'): ?>
   <lowerValue xmi:type="uml:LiteralInteger"          xmi:id="<?php print $object;?>_<?php print $property;?>_lower" value="2"/>
   <upperValue xmi:type="uml:LiteralUnlimitedNatural" xmi:id="<?php print $object;?>_<?php print $property;?>_upper" value="-1"/>                        
<?php endif; ?>   
<?php if($cardinality == '4..n'): ?>
   <lowerValue xmi:type="uml:LiteralInteger"          xmi:id="<?php print $object;?>_<?php print $property;?>_lower" value="4"/>
   <upperValue xmi:type="uml:LiteralUnlimitedNatural" xmi:id="<?php print $object;?>_<?php print $property;?>_upper" value="-1"/>                        
<?php endif; ?>
<?php if($cardinality == '0..1'): ?>
   <lowerValue xmi:type="uml:LiteralInteger" xmi:id="<?php print $object;?>_<?php print $property;?>_lower"/>
   <upperValue xmi:type="uml:LiteralInteger" xmi:id="<?php print $object;?>_<?php print $property;?>_upper" value="1"/>                        
<?php endif; ?>                            
<?php if($cardinality == '1..1'): ?>
   <lowerValue xmi:type="uml:LiteralInteger" xmi:id="<?php print $object;?>_<?php print $property;?>_lower" value="1"/>
   <upperValue xmi:type="uml:LiteralInteger" xmi:id="<?php print $object;?>_<?php print $property;?>_upper" value="1"/>                        
<?php endif; ?>    