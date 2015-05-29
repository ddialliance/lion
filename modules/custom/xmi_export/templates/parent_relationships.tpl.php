<table class="sticky-enabled tableheader-processed sticky-table">
    <thead>
        <tr>Inherited from: <?php print $title; ?></tr>
        <tr>
            <th class="field_relationship_name">Name</th>
            <th class="field_relationship_target_object">Target Object</th>
            <th class="field_relationship_description_l">Description</th>
            <th class="field_relationship_source_cardin">Source cardinality</th>
            <th class="field_relationship_target_cardin">Target cardinality</th>
            <th class="field_relationship_type">Relationship type</th>
        </tr>
    </thead>
    <tbody>
        
        <?php $row_color = "odd"; ?>
        <?php foreach($relationships as $relationship) : ?>
                
        <tr class="field_collection_item odd">
            <td class="field_relationship_name">
                <div class="field field-name-field-relationship-name field-type-text field-label-hidden">
                    <div class="field-items">
                        <div class="field-item even"><?php print $relationship["name"]; ?></div>  
                    </div>
                </div>
            </td>
            <td class="field_relationship_target_object">
                <div class="field field-name-field-relationship-target-object field-type-entityreference field-label-hidden">
                    <div class="field-items">
                        <div class="field-item even">
                            <a href="/ddiobjects/<?php print $relationship["target_object"]; ?>"><?php print $relationship["target_object"]; ?></a>
                        </div>
                    </div>
                </div>
            </td>
            <td class="field_relationship_description_l">
                <div class="field field-name-field-relationship-description-l field-type-text-long field-label-hidden">
                    <div class="field-items">
                        <div class="field-item even"><?php print $relationship["description"]; ?></div>
                    </div>                        
                </div>
            </td>
            <td class="field_relationship_source_cardin">
                <div class="field field-name-field-relationship-source-cardin field-type-list-text field-label-hidden">
                    <div class="field-items">
                        <div class="field-item even"><?php print $relationship["source_cardinality"]; ?></div>
                    </div>            
                </div>
            </td>
            <td class="field_relationship_target_cardin">
                <div class="field field-name-field-relationship-target-cardin field-type-list-text field-label-hidden">
                    <div class="field-items">
                        <div class="field-item even"><?php print $relationship["target_cardinality"]; ?></div>  
                    </div>
                </div>
            </td>
            <td class="field_relationship_type">
                <div class="field field-name-field-relationship-type field-type-list-text field-label-hidden">
                    <div class="field-items">
                        <div class="field-item even"><?php print $relationship["relationship_type"]; ?></div>  
                    </div>            
                </div>
            </td>
        </tr>
        
        <?php             
            if($row_color == "odd")
                $row_color = "even";
            else $row_color = "odd";
        ?>
        <?php endforeach; ?>
        
    </tbody>
</table>