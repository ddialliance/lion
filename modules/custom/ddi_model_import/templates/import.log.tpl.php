<table>
    <thead>
        <tr>
            <th>Object</th>
            <th>Extends</th>
            <th>Properties</th>
            <th>Relationships</th>
            <th>Import Status</th>
        </tr>
    </thead>
    <tbody><?php //dsm($log);?>
        <?php foreach($log['object'] as $l): ?>
            <?php if($l['status'] == 'ok'):?>
            <tr style="background-color:lightgreen;">
            <?php elseif($l['status'] == 'notice'):?>
            <tr style="background-color: orange;">
            <?php else: ?>
            <tr style="background-color:lightcoral;">    
            <?php endif;?>

                <td><?php print $l['Object Name'];?></td>
                <td><?php print $l['Object Extends'];?></td>

                <td>
                    <?php if(array_key_exists('Property', $l)){print count($l['Property']);}else{print 0;}?>
                </td>
                <td>
                    <?php if(array_key_exists('Relationship', $l)){print count($l['Relationship']);}else{print 0;}?>
                </td>                
                
                <td class="<?php print $l['status'];?>"><?php print $l['status message'];?></td>
            </tr>
        <?php endforeach;?>
    </tbody>
</table>