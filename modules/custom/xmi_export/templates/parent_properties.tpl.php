<br/>
<table class="sticky-enabled tableheader-processed sticky-table" style="background-color:aliceblue;">  
    <thead>
        <tr>Inherited from: <?php print $title?></tr>
        <tr>
            <th class="field_property_name">Name</th>
            <th class="field_property_cardinality">Cardinality</th>
            <th class="field_property_datatype">Datatype</th>
            <th class="field_property_description_long">Description</th>
        </tr>
    </thead>
    <tbody>
        <?php $row_color = "odd"; ?>
        <?php foreach($properties as $property) : ?>
        
            <tr class="field_collection_item <?php print $row_color ?>">
        
                <td class="field_property_name">
                    <div class="field field-name-field-property-name field-type-text field-label-hidden">
                        <div class="field-items">
                            <div class="field-item even"><?php print $property["name"]; ?></div>
                        </div>
                    </div>
                </td>
                <td class="field_property_cardinality">
                    <div class="field field-name-field-property-cardinality field-type-text field-label-hidden">
                        <div class="field-items">
                            <div class="field-item even"><?php print $property["cardinality"]; ?></div>
                        </div>
                    </div>
                </td>
                <td class="field_property_datatype">
                    <div class="field field-name-field-property-datatype field-type-text field-label-hidden">
                        <div class="field-items">
                            <div class="field-item even">
                                <a href="/ddiobjects/<?php print $property["datatype"]; ?>"><?php print $property["datatype"]; ?></a>
                            </div>
                        </div>
                    </div>
                </td>
                <td class="field_property_description_long">
                    <div class="field field-name-field-property-description_long field-type-text field-label-hidden">
                        <div class="field-items">
                            <div class="field-item even"><?php print $property["description"]; ?></div>
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