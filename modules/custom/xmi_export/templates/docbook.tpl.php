<book xmlns="http://docbook.org/ns/docbook" xmlns:xlink="http://www.w3.org/1999/xlink" version="5.0">
    <info>
        <title>DDI Moving Forward: Object Description</title>
        <author>
            <orgname>the Data Documentation Initiative</orgname>
        </author>
    </info>

    <?php foreach($objects as $package=>$ddiobjects): ?>
    <part xml:id="ddi4-<?php print str_replace(' ', '',$package);?>">
            <title><?php print $package;?></title>
            <?php foreach($ddiobjects as $object):?>
            <chapter xml:id="ddi4-<?php print $object['uuid']; ?>">
                <title><?php print $object['name']; ?></title>
                <?php if($object['extends']):?>
                <section  revision="" role="extends">
                    <title>Extends</title>
                    <?php if(substr($object['extends'], 0, 3 ) === "xs:"):?>
                    <para>This object extends <?php print $object['extends']; ?></para>
                    <?php else: ?>
                    <para>This object extends <link linkend="ddi4-<?php print $object['extends_uuid']; ?>"><?php print $object['extends']; ?></link></para>
                    <?php endif;?>
                </section>
                <?php endif;?>
                <section xml:id="ddi4-<?php print $object['uuid']; ?>-definition"  revision="" role="definition">
                    <title>Definition</title>
                    <para>
                        <?php if(array_key_exists('definition',$object)){print $object['definition'];} ?>
                    </para>
                </section>
                <?php if(count($object['properties']) > 0): ?>
                <section xml:id="ddi4-<?php print $object['uuid']; ?>-properties" revision="" role="properties">
                    <title>Properties</title>
                    <table label="<?php print $object['name']; ?>">    
                        <title>list of properties</title>
                        <tgroup cols="4">
                            <thead>
                                <row>
                                    <entry>Name</entry>
                                    <entry>Datatype</entry>
                                    <entry>Description</entry>
                                    <entry>Cardinality</entry>
                                </row>
                            </thead>
                            <tbody>
                                <?php foreach($object['properties'] as $item):?>
                                <row>
                                    <entry role="name"><?php print $item['name']; ?></entry>
                                    <entry role="datatype"><?php if(array_key_exists('datatype',$item)){print $item['datatype'];} ?></entry>
                                    <entry role="description"><?php if(array_key_exists('description',$item)){print $item['description'];} ?></entry>
                                    <entry role="cardinality"><?php if(array_key_exists('datatype',$item)){print $item['cardinality'];} ?></entry>
                                </row>
                                <?php endforeach;?>
                            </tbody>
                        </tgroup>
                    </table>
                </section>
                <?php endif;?>
                <?php if(count($object['relationships']) > 0): ?>
                <section xml:id="ddi4-<?php print $object['uuid']; ?>-relationships" revision="" role="relationships">
                    <title>Relationships</title>
                    <table label="<?php print $object['name']; ?>">
                        <title>list of relationships</title>
                        <tgroup cols="6">
                            <thead>
                                <row>
                                    <entry>Name</entry>
                                    <entry>Target</entry>
                                    <entry>Description</entry>
                                    <entry>Type</entry>
                                    <entry>Source cardinality</entry>
                                    <entry>Target cardinality</entry>
                                </row>
                            </thead>
                            <tbody>
                                <?php foreach($object['relationships'] as $item):?>
                                <row>
                                    <entry><?php print $item['name'];?></entry>
                                    <?php if($item['target_object']):?>
                                    <entry><link linkend="ddi4-<?php print $item['target_object_uuid'];?>"><?php print $item['target_object'];?></link></entry>
                                    <?php else: ?>
                                    <entry>NOT DEFINED</entry>
                                    <?php endif;?>
                                    <entry><?php print $item['description'];?></entry>
                                    <entry><?php print $item['type'];?></entry>
                                    <entry><?php print $item['source_cardinality'];?></entry>
                                    <entry><?php print $item['target_cardinality'];?></entry>
                                </row>
                                <?php endforeach;?>
                            </tbody>
                        </tgroup>
                    </table>
                </section>
                <?php endif; ?>
            </chapter>
            <?php endforeach; ?>
            </part>
        <?php endforeach; ?>
</book>
