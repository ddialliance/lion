<p>
<table>
    <tr>
        <td>Total number of objects</td><td><?php print $total;?></td>
    </tr>
    <tr style="color:#F2000D;">
        <td>Have errors</td><td><?php print $errors;?></td>
    </tr>
    <tr style="color:#D77D00;">
        <td>Have warnings</td><td><?php print $warnings;?></td>
    </tr>    
</table>
</p>
<?php foreach($objects as $package => $objs): ?>
    <hr/>
    <h2><?php print $package; ?></h2>
    <?php $count = 0;?>
    <?php foreach($objs as $object): ?>
        <?php if(array_key_exists("errors", $object) || array_key_exists("warnings", $object)): ?>
            <?php $count++;?>
            <h3><?php print l($object["name"], "node/".$object["nid"]); ?><?php if(user_is_logged_in()):?> <strong><?php print l("[edit]", "node/".$object["nid"]."/edit"); ?></strong><?php endif;?></h3>
            <p class="error">
                <ul>
                <?php foreach($object["errors"] as $error):?>
                    <li style="color:#F2000D;"><?php print $error;?></li>
                <?php endforeach; ?>
                <?php foreach($object["warnings"] as $warning):?>
                    <li style="color:#D77D00;"><?php print $warning;?></li>
                <?php endforeach; ?>                    
                </ul>
            </p>
        <?php endif;?>
    <?php endforeach; ?>
    <?php if($count == 0): ?>
        <div class="messages status">
            <strong>All objects in package is valid</strong>
        </div>
    <?php endif;?>
<?php endforeach; ?>
