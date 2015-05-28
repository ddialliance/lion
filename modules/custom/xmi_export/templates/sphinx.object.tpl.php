<?php print $object['name'];?>

****************************

<?php print $object['definition'];?>

<?php if(count($object['properties']) > 0): ?>

Properties
==========

.. csv-table:: 
   :header: "Name", "Type", "Cardinality"
   
<?php foreach($object['properties'] as $item):?>
   "<?php print $item['name']; ?>", "<?php print $item['datatype']; ?>", "<?php print $item['cardinality'];?>"
<?php endforeach;?>


<?php foreach($object['properties'] as $item):?>


<?php print $item['name']; ?>

####################################

<?php print trim($item['description']); ?>


<?php endforeach;?>

<?php endif;?>


