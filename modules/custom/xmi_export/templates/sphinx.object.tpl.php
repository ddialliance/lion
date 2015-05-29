.. _<?php print $object['name'];?>:

<?php print _heading($object['name'], '*');?>

<?php print $object['definition'];?>


<?php if(!empty($object['extends'])):?>
Extends
=======
:ref:`<?php print $object['extends'];?>`
<?php endif;?>

<?php if(count($object['properties']) > 0): ?>

Properties
==========

.. csv-table:: 
   :header: "Name", "Type", "Cardinality"
   
<?php foreach($object['properties'] as $item):?>
   "<?php print $item['name']; ?>", "<?php print $item['datatype']; ?>", "<?php print $item['cardinality'];?>"
<?php endforeach;?>

<?php foreach($object['properties'] as $item):?>

<?php print _heading($item['name'], '#'); ?>

<?php print trim($item['description']); ?>


<?php endforeach;?>

<?php endif;?>


Graph
=====

.. graphviz:: /images/graph/<?php print $object['package']; ?>/<?php print $object['name'];?>.dot