<table>    
 <thead>
  <tr>
   <th><?php print $type; ?></th>
   <th>DDI4</th>
  </tr>
 </thead>
 <tbody>
  <?php foreach($mappings as $key => $value): ?>
  <tr>
   <td><?php print $key; ?></td>
   <td>
    <?php foreach($value as $v): ?>
       <?php print l($v, "ddiobjects/".strtolower($v)); //"<a href=/ddiobjects/".strtolower($v).">".$v."</a>,"; ?>
    <?php endforeach;?>
   </td>
  </tr>
  <?php endforeach;?>
  </tbody>
</table>