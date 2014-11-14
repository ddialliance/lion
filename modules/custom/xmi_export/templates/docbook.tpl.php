<book xmlns="http://docbook.org/ns/docbook" xmlns:xlink="http://www.w3.org/1999/xlink" version="5.0">
    <info>
        <title>DDI Moving Forward: Object Description</title>
        <author>
            <orgname>the Data Documentation Initiative</orgname>
        </author>
    </info>

    <?php foreach($objects as $package=>$ddiobjects): ?>
    <chapter xml:id="ddi4-<?php print str_replace(' ', '',$package);?>">
        <title><?php print $package;?></title>
        <?php foreach($ddiobjects as $object):?>
        <section xml:id="ddi4-<?php print $object['uuid']; ?>">
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
                <?php foreach($object['properties'] as $item):?>
                <table label="<?php print $object['name']; ?>" frame="all">
                    <title/>
                    <tgroup cols="2">
                        <colspec colname="c1" colnum="1" colwidth="1*"/>
                        <colspec colname="c2" colnum="2" colwidth="8.14*"/>
                        <thead/>
                        <tbody>
                            <row>
                                <entry>Name</entry>
                                <entry role="name"><?php print $item['name']; ?></entry>
                            </row>
                            <row>
                                <entry>Datatype</entry>
                                <entry role="datatype"><?php if(array_key_exists('datatype',$item)){print $item['datatype'];} ?></entry>
                            </row>
                            <row>
                                <entry>Cardinality</entry>
                                <entry role="cardinality"><?php if(array_key_exists('datatype',$item)){print $item['cardinality'];} ?></entry>
                            </row>
                            <row>
                                <entry role="description" namest="c1" nameend="c2">
                                    <?php if(array_key_exists('description',$item)){print $item['description'];} ?>
                                </entry>
                            </row>
                        </tbody>
                    </tgroup>                    
                </table>
                <?php endforeach;?>
            </section>
            <?php endif;?>
            <?php if(count($object['relationships']) > 0): ?>
            <section xml:id="ddi4-<?php print $object['uuid']; ?>-relationships" revision="" role="relationships">
                <title>Relationships</title>
                
                <?php foreach($object['relationships'] as $item):?>
                <table frame="all">
                    <title/>
                    <tgroup cols="2">
                        <colspec colname="c1" colnum="1" colwidth="1*"/>
                        <colspec colname="c2" colnum="2" colwidth="8.14*"/>
                        <thead/>
                        <tbody>
                            <row>
                                <entry>Name</entry>
                                <entry><?php print $item['name'];?></entry>
                            </row>
                            <row>
                                <entry>Target</entry>
                                <?php if($item['target_object']):?>
                                <entry><link linkend="ddi4-<?php print $item['target_object_uuid'];?>"><?php print $item['target_object'];?></link></entry>
                                <?php else: ?>
                                <entry>NOT DEFINED</entry>
                                <?php endif;?>
                            </row>
                            <row>
                                <entry>Type</entry>
                                <entry><?php print $item['type'];?></entry>
                            </row>
                            <row>
                                <entry>Source Cardinality</entry>
                                <entry><?php print $item['source_cardinality'];?></entry>
                            </row>
                            <row>
                                <entry>Target Cardinality</entry>
                                <entry><?php print $item['target_cardinality'];?></entry>
                            </row>                            
                            <row>
                                <entry namest="c1" nameend="c2">
                                    <?php print $item['description'];?>
                                </entry>
                            </row>
                        </tbody>
                    </tgroup>
                </table>
                <?php endforeach;?>
            </section>
            <?php endif; ?>
        </section>
        <?php endforeach; ?>
    </chapter>
    <?php endforeach; ?>
</book>
