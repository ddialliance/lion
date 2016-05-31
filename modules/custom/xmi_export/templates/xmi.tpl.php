<xmi:XMI xmi:version="2.4.1" xmlns:uml="http://www.omg.org/spec/UML/20110701" xmlns:xmi="http://www.omg.org/spec/XMI/20110701">
    <!-- 
        NOTE: exporter have to be "Enterprise Architect", EA wont read it if set do eg. Drupal 
        We will play along to get "Enterprise Architect" happy
    -->
    <xmi:Documentation exporter="Enterprise Architect" exporterVersion="6.5"/>
    <uml:Model xmi:type="uml:Model" xmi:id="42" name="ddi4">
        <!-- DDI4 packages -->
        <packagedElement xmi:type="uml:Package" xmi:id="ddi4_model" name="Class Model (Exported from Drupal)">
            <?php foreach ($objects as $package => $ddiobjects): ?>
                <packagedElement xmi:type="uml:Package" xmi:id="<?php print $package; ?>" name="<?php print $package; ?>">
					<ownedComment xmi:type="uml:Comment">
						<body><?php if(array_key_exists('definition',$package)){print $package['definition'];} ?></body>
					</ownedComment>
                    <?php if(array_key_exists($package, $datatypes)): ?>
                    <!-- Datatypes -->
                    <?php foreach($datatypes[$package] as $datatype): ?>
                        <?php if(is_array($datatype)):?>
                            <?php if(array_key_exists('enumeration', $datatype)): ?>
                                <packagedElement xmi:type="uml:Enumeration" xmi:id="<?php print $datatype['name']; ?>" name="<?php print $datatype['name']; ?>">
									<ownedComment xmi:type="uml:Comment">
										<body><?php if(array_key_exists('definition',$datatype)){print $datatype['definition'];} ?></body>
									</ownedComment>
                                    <?php foreach($datatype['enumeration'] as $enumeration): ?>
                                    <ownedLiteral xmi:type="uml:EnumerationLiteral" name="<?php print $enumeration['value']; ?>"/>
                                    <?php endforeach; ?>
                                </packagedElement>
                            <?php else: ?>
                                <packagedElement xmi:type="uml:DataType" xmi:id="<?php print $datatype['name']; ?>" name="<?php print $datatype['name']; ?>"/>
                            <?php endif;?>
                        <?php endif;?>
                    <?php endforeach; ?>
                    <!-- End Datatypes -->
                    <?php endif; ?>
                    <?php foreach ($ddiobjects as $object): ?>
                        <?php if(is_array($object)):?>
                            <packagedElement xmi:type="uml:Class" name="<?php print $object['name']; ?>" xmi:id="<?php print $object['name']; ?>" visibility="package" <?php if($object['is_abstract']): ?>isAbstract="true"<?php endif;?>>
								<ownedComment xmi:type="uml:Comment">
									<body><?php if(array_key_exists('definition',$object)){print $object['definition'];} ?></body>
								</ownedComment>
                                <?php if ($object['extends']): ?>
                                    <!-- extends -->
                                    <generalization xmi:type="uml:Generalization" xmi:id="<?php print $object['name']; ?>_extends_<?php print $object['extends']; ?>" general="<?php print $object['extends']; ?>"/>
                                <?php endif; ?>
                                <?php if(array_key_exists('properties', $object)): ?>
                                <!-- properties -->
                                <?php foreach ($object['properties'] as $item): ?>
                                    <ownedAttribute xmi:type="uml:Property" name="<?php print $item['name']; ?>" xmi:id="<?php print $object['name']; ?>_<?php print $item['name']; ?>">
                                        <?php if (array_key_exists('datatype', $item)): ?>
                                            <?php print _render_datatype($item['datatype']); ?>
                                        <?php endif; ?>
                                        <?php if (array_key_exists('cardinality', $item)): ?>
                                            <?php print theme('xmi_cardinality', array('object' => $object['name'], 'property' => $item['name'], 'cardinality' => $item['cardinality'])); ?>
                                        <?php endif; ?>
                                    </ownedAttribute>
                                <?php endforeach; ?>
                                <?php endif;?>

                                <?php if (array_key_exists('relationships', $object)): ?>
                                    <!-- relationships -->
                                    <?php foreach ($object['relationships'] as $relation): ?>
                                        <?php if ($relation['target_object']): ?>
                                            <ownedAttribute xmi:type="uml:Property" name="<?php print $object['name']; ?>" xmi:id="<?php print $object['name']; ?>_<?php print $relation['name']; ?>_source" association="<?php print $object['name']; ?>_<?php print $relation['name']; ?>_association">
                                                <?php if (array_key_exists('target_object', $relation)): ?>
                                                    <type xmi:idref="<?php print $relation['target_object']; ?>"/>
                                                <?php endif; ?>
                                                <?php if (array_key_exists('source_cardinality', $relation)): ?>
                                                    <?php print theme('xmi_cardinality', array('object' => $object['name'], 'property' => $relation['name'], 'cardinality' => $relation['source_cardinality'])); ?>
                                                <?php endif; ?>                    
                                            </ownedAttribute>
                                        <?php endif; ?>
                                    <?php endforeach; ?>
                                <?php endif; ?>
                            </packagedElement>
                            <?php foreach ($object['relationships'] as $relation): ?>
                                <?php if ($relation['target_object']): ?>
                        <!-- <?php print str_replace('-', '',$object['name']); ?>.<?php print str_replace('-', '',$relation['name']); ?> -->
                                    <packagedElement xmi:type="uml:Association" xmi:id="<?php print $object['name']; ?>_<?php print $relation['name']; ?>_association" name="<?php print $relation['name']; ?>">
										<ownedComment xmi:type="uml:Comment">
											<body><?php if(array_key_exists('description',$relation)){print htmlspecialchars($relation['description'], ENT_QUOTES);} ?></body>
										</ownedComment>
                                        <memberEnd xmi:idref="<?php print $object['name']; ?>_<?php print $relation['name']; ?>_source"/>
                                        <memberEnd xmi:idref="<?php print $object['name']; ?>_<?php print $relation['name']; ?>_target"/>
                                        <ownedEnd xmi:id="<?php print $object['name']; ?>_<?php print $relation['name']; ?>_target" xmi:type="uml:Property" association="<?php print $object['name']; ?>_<?php print $relation['name']; ?>_association" <?php if($relation['xmi_type'] != "none"):?> aggregation="<?php print $relation['xmi_type']; ?>"<?php endif;?>>
                                            <type xmi:idref="<?php print $object['name']; ?>"/>
                                            <?php if (array_key_exists('target_cardinality', $relation)): ?>
                                                <?php print theme('xmi_cardinality', array('object' => $object['name'], 'property' => $relation['name']."_packagedElement", 'cardinality' => $relation['target_cardinality'])); ?>
                                            <?php endif; ?> 
                                        </ownedEnd>
                                    </packagedElement>
                                <?php endif; ?>
                            <?php endforeach; ?>
                        <?php endif;?>
                    <?php endforeach; ?>
                </packagedElement>
            <?php endforeach; ?>
        </packagedElement>
        <!-- DDI4 views -->
        <packagedElement xmi:type="uml:Package" xmi:id="ddi4_views" name="Views (Exported from Drupal)">
            <?php foreach($views as $view): ?>
                <packagedElement xmi:type="uml:Package" xmi:id="<?php print $view['name'];?>" name="<?php print $view['name'];?>">
					<ownedComment xmi:type="uml:Comment">
						<body><?php if(array_key_exists('definition',$view)){print $view['definition'];} ?></body>
					</ownedComment>
				</packagedElement>
            <?php endforeach; ?>
        </packagedElement>
    </uml:Model>
    <xmi:Extension extender="Enterprise Architect" extenderID="6.5">
        <elements>
            <!-- Packages -->
            <?php foreach ($objects as $package => $ddiobjects): ?>
            <element xmi:idref="<?php print $package; ?>" xmi:type="uml:Package" name="Class Model for <?php print $package; ?> (Exported from Drupal)" scope="public">
                <properties isSpecification="false" sType="Package" nType="0" scope="public"/>
            </element>
            <?php endforeach;?>
            <!-- Objects -->
            <?php foreach ($objects as $package => $ddiobjects): ?>
                <?php foreach ($ddiobjects as $object): ?>
            <?php if(is_array($object)):?>
            <element xmi:idref="<?php print $object['name'];?>" xmi:type="uml:Class" name="<?php print $object['name'];?>" scope="package">
                <model package="<?php print $package;?>" tpos="0" ea_localid="<?php print $object['nid'];?>" ea_eleType="element"/>
                <!-- the documentation in escaped html -->
                <properties documentation="<?php if(array_key_exists('definition',$object)){print htmlspecialchars($object['definition'], ENT_QUOTES);} ?>"  isSpecification="false" sType="Class" nType="0" scope="package" isRoot="false" isLeaf="false" <?php if($object['is_abstract']): ?>isAbstract="true"<?php endif;?> isActive="false"/>
                <extendedProperties tagged="0" package_name="<?php print $package;?>"/>
                <code gentype="Java"/>
                
                <?php if(count($object['properties'])): ?>
                <attributes> 
                    <?php foreach ($object['properties'] as $item): ?>
                        <attribute xmi:idref="<?php print $object['name']; ?>_<?php print $item['name']; ?>" name="<?php print $item['name']; ?>" scope="Public">
                                <initial/>
                                <documentation value="<?php if(array_key_exists('description',$item)){print htmlspecialchars($item['description'], ENT_QUOTES);} ?>"/>
                                
                                <!-- <model ea_localid="2414" ea_guid="{31588E67-78D6-4a2a-8AAC-4EAAB311365C}"/> -->
                                <properties derived="0" collection="false" duplicates="1" changeability="changeable"/>
                                <coords ordered="0"/>
                                <containment position="0"/>
                                <stereotype/>
                                <?php if (array_key_exists('cardinality', $item)): ?>
                                    <?php $parts = explode('..', $string); ?>
                                    <?php if(count($parts) == 2): ?>
                                    <bounds lower="<?php print $parts[0];?>" upper="<?php print $parts[1];?>"/>
                                    <?php endif;?>
                                <?php endif;?>
                                <options/>
                                <style/>
                                <styleex value="IsLiteral=0;"/>
                                <tags/>
                                <xrefs/>
                        </attribute>      
                    <?php endforeach;?>
                </attributes>
                <?php endif;?>
                
                <?php if(count($object['relationships']) || $object['extends']):?>
                <links>
                    <?php if ($object['extends']): ?>
                    <!-- extends -->
                    <Generalization xmi:id="EA_<?php print $object['name']; ?>_extends_<?php print $object['extends']; ?>" start="<?php print $object['name'];?>" end="<?php print $object['extends']; ?>"/>
                    <?php endif; ?>

                    <?php foreach ($object['relationships'] as $relation): ?>
                        <?php if ($relation['target_object']): ?>
                        <?php endif;?>
                    <?php endforeach;?>
                </links>
                <?php endif;?>
            </element>
            <?php endif;?>
                <?php endforeach;?>
            <?php endforeach;?>
        </elements>
        <connectors>
            <?php foreach ($objects as $package => $ddiobjects): ?>
                <?php foreach ($ddiobjects as $object): ?>
                    <?php if(is_array($object) && (count($object['relationships']) || $object['extends'])):?>
                        <?php if ($object['extends']): ?>
                            <!-- extends connector -->
                            <connector xmi:idref="<?php print $object['name']; ?>_<?php print $relation['name']; ?>_source">
                                <source xmi:idref="<?php print $object['name']; ?>">
                                        <model ea_localid="1621" type="Class" name="<?php print $object['name']; ?>"/>
                                        <constraints/>
                                        <modifiers isNavigable="false"/>
                                        <style/>
                                        <documentation/>
                                        <xrefs/>
                                        <tags/>
                                </source>
                                <target xmi:idref="<?php print $relation['name']; ?>">
                                        <model ea_localid="1620" type="Class" name="<?php print $relation['name']; ?>"/>
                                        <constraints/>
                                        <modifiers isNavigable="false"/>
                                        <style/>
                                        <documentation/>
                                        <xrefs/>
                                        <tags/>
                                </target>
                                <model ea_localid="3342"/>
                                <properties ea_type="Generalization" direction="Source -&gt; Destination"/>
                                <parameterSubstitutions/>
                                <documentation/>
                                <appearance linemode="3" linecolor="0" linewidth="0" seqno="0" headStyle="0" lineStyle="0"/>
                                <labels/>
                                <extendedProperties/>
                                <style/>
                                <xrefs/>
                                <tags/>
                            </connector>
                        <?php endif;?>
                        <?php foreach ($object['relationships'] as $relation): ?>
                            
                        <?php endforeach;?>
                    <?php endif;?>
                <?php endforeach;?>
            <?php endforeach;?>
        </connectors>
        <diagrams>
            <!-- Begin views diagrams -->
            <?php foreach($views as $view): ?>
            <diagram xmi:id="<?php print $view['name'];?>Diagram">
             <model package="<?php print $view['name'];?>" owner="<?php print $view['name'];?>"/>
             <properties name="<?php print $view['name'];?> Diagram" />
             <elements>
               <?php $num = 0; ?>
               <?php foreach($view['objects'] as $object): ?>
               <element subject="<?php print $object['name'];?>" seqno="<?php print $num++;?>"/>
               <?php endforeach;?>
             </elements>
            </diagram>
            <?php endforeach; ?>
            <!-- End views diagrams -->
            
            <!-- TODO: Generate a diagram for each package -->
            <?php $i = 1; ?>
            <?php foreach ($objects as $package => $ddiobjects): ?>
                <?php if(is_array($object)):?>
                <diagram>
                    <model package="<?php print $package;?>" localID="<?php print $i;?>" owner="<?php print $package;?>"/>
                    <properties name="<?php print $package;?>" type="Logical"/>
                    <?php $i++;?>
                    <elements>
                    <?php $j = 1;?>
                    <?php foreach ($ddiobjects as $object): ?>
                        <element subject="<?php print $object['name'];?>" seqno="<?php print $j;?>" />
                        <?php $j++;?>
                    <?php endforeach;?>
                    </elements>
                </diagram>
            <?php endif;?>
            <?php endforeach;?>
            <!-- TODO: Generate a diagram for each view -->
        </diagrams>
    </xmi:Extension>
</xmi:XMI>