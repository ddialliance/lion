<book xmlns="http://docbook.org/ns/docbook" xmlns:xlink="http://www.w3.org/1999/xlink" version="5.0">
    <info>
        <title>DDI Moving Forward: Object Description</title>
        <subtitle>
            <markup><?php print date("Y-m-d");?></markup>
        </subtitle>        
        <author>
            <orgname>the Data Documentation Initiative</orgname>
        </author>
        <date><?php print date("Y-m-d");?></date>
        <legalnotice>
            <para>Licence of all documentation is LGPL v3</para>
        </legalnotice>
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
            <?php if($object["is_abstract"]): ?>
            <section  revision="" role="is_abstract">
                <title>Abstract</title>
                <para>This class is abstract</para>
            </section>
            <?php endif;?>
            
            <?php if(!empty($object["ddi_3_2"])): ?>
            <section  revision="" role="ddi_3_2">
                <title>Based on DDI 3.2</title>
                <para><?php print $object["ddi_3_2"];?></para>
            </section>
            <?php endif;?>            
    
            <?php if($object["rdf_mapping"]): ?>
            <section  revision="" role="rdf_mapping">
                    <table frame="all">
                        <title>RDF Mapping</title>
                        <tgroup cols="3">
                            <colspec colname="c1" colnum="1" colwidth="1*"/>
                            <colspec colname="c2" colnum="2" colwidth="1.4*"/>
                            <colspec colname="c3" colnum="3" colwidth="2.68*"/>
                            <thead>
                                <row>
                                    <entry>Label</entry>
                                    <entry>Type</entry>
                                    <entry>URI</entry>
                                </row>
                            </thead>
                            <tbody>
                                <?php foreach($object["rdf_mapping"] as $mapping): ?>
                                <row>
                                    <entry><?php print $mapping['label'];?></entry>
                                    <entry><?php print $mapping['type'];?></entry>
                                    <entry><link xlink:href="<?php print $mapping['uri'];?>"><?php print $mapping['uri'];?></link></entry>
                                </row>
                                <?php endforeach;?>    
                            </tbody>
                        </tgroup>
                    </table>
            </section>
            <?php endif;?>  
            
            <?php if(!empty($object["gsim"])): ?>
            <section  revision="" role="gsim">
                <title>Corresponds to GSIM</title>
                <para><?php print $object["gsim"];?></para>
            </section>
            <?php endif;?>                   
            
            <section xml:id="ddi4-<?php print $object['uuid']; ?>-definition"  revision="" role="definition">
                <title>Definition</title>
                <para><?php if(array_key_exists('definition',$object)){print $object['definition'];} ?></para>
            </section>
            
            <?php if($object['example']):?>
            <section xml:id="ddi4-<?php print $object['uuid']; ?>-example"  revision="" role="example">
                <title>Example</title>
                <para>
                    <![CDATA[
                    <?php if(array_key_exists('example',$object)){print $object['example'];} ?>
                    ]]>
                </para>
            </section>            
            <?php endif;?>

            <?php if($object['explanatory_notes']):?>
            <section xml:id="ddi4-<?php print $object['uuid']; ?>-explanatory_notes"  revision="" role="explanatory_notes">
                <title>Example</title>
                <para>
                    <![CDATA[
                    <?php if(array_key_exists('explanatory_notes',$object)){print $object['explanatory_notes'];} ?>
                    ]]>
                </para>
            </section>            
            <?php endif;?>            
            
            <?php if(count($object['properties']) > 0): ?>
            <section xml:id="ddi4-<?php print $object['uuid']; ?>-properties" revision="" role="properties">
                <title>Properties</title>
                <?php foreach($object['properties'] as $item):?>
                <section xml:id="ddi4-<?php print $object['uuid']; ?>-properties-<?php print $item['name']; ?>">
                    <title><?php print $item['name']; ?></title>
                    <informaltable frame="all">
                        <tgroup cols="2">
                            <colspec colname="c1" colnum="1" colwidth="1*"/>
                            <colspec colname="c2" colnum="2" colwidth="7.87*"/>
                            <tbody>
                                <row>
                                    <entry>Datatype</entry>
                                    <entry role="datatype"><?php if(array_key_exists('datatype',$item)){print $item['datatype'];} ?></entry>
                                </row>
                                <row>
                                    <entry>Cardinality</entry>
                                    <entry role="cardinality"><?php if(array_key_exists('datatype',$item)){print $item['cardinality'];} ?></entry>
                                </row>
                                <?php if(!empty(trim($item['description']))): ?>
                                <row>
                                    <entry role="description" namest="c1" nameend="c2"><?php print trim($item['description']); ?></entry>
                                </row>
                                <?php endif;?>
                            </tbody>
                        </tgroup>                    
                    </informaltable>
                </section>                    
                <?php endforeach;?>
            </section>
            <?php endif;?>
            <?php if(count($object['relationships']) > 0): ?>
            <section xml:id="ddi4-<?php print $object['uuid']; ?>-relationships" revision="" role="relationships">
                <title>Relationships</title>
                
                <?php foreach($object['relationships'] as $item):?>
                <section  xml:id="ddi4-<?php print $object['uuid']; ?>-relationships-<?php print $item['name']; ?>">
                    <title><?php print $item['name'];?></title>
                    <informaltable frame="all">
                        <tgroup cols="2">
                            <colspec colname="c1" colnum="1" colwidth="1*"/>
                            <colspec colname="c2" colnum="2" colwidth="7.87*"/>

                            <tbody>
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
                                <?php if(!empty(trim($item['description']))): ?>
                                <row>
                                    <entry namest="c1" nameend="c2"><?php print trim($item['description']);?></entry>
                                </row>
                                <?php endif;?>
                            </tbody>
                        </tgroup>
                    </informaltable>
                    </section>
                <?php endforeach;?>
            </section>
            <?php endif; ?>
        </section>
        <?php endforeach; ?>
    </chapter>
    <?php endforeach; ?>
</book>
