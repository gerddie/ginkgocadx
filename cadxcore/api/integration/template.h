/*
*  
*  $Id: template.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#pragma once
#define DEFAULT_INTEGRATION_MESSAGE_TEMPLATE \
"<msg>" \
"		<seg>MSH|^~\\&amp;|<repl key=\"ginkgocadx.conf:gingocadx.application.id\"/>|<repl key=\"referring.physician:institution.id\"/>|<repl key=\"hce:id\"/>|<repl key=\"ginkgocadx.conf:receiving.organization.id\"/>|<repl key=\"message:timestamp.hl7\"/>||ORU^R01^ORU_R01|GNK.<repl key=\"message:timestamp.epoch\"/>|P|2.5|||AL|NE</seg>" \
"		<seg>PID|1||<repl key=\"patient:id\"/>^^^HIS^PI^^^^<repl key=\"referring.physician:institution.id\"/>&amp;&amp;99CENTROSACYL||<repl key=\"patient:family.name\"/>^<repl key=\"patient:name\"/>|<repl key=\"patient:family.second.name\"/></seg>" \
"		<seg>PV1|1||||||||||||||||||<repl key=\"hce:episode.number\"/>^^^<repl key=\"jimena:ambito.peticion\"/>^VN^^^^<repl key=\"referring.physician:institution.id\"/>&amp;&amp;99CENTROSACYL</seg>" \
"		<for.study>" \
"			<seg>ORC|SC|<repl key=\"hce:petition.id\"/>|<repl key=\"message:timestamp.epoch\"/>.<repl key=\"global:pacs.local.aet\"/>^<repl key=\"ginkgocadx.conf:filler.application.id\"/>|<repl key=\"hce:petition.id\"/>|CM|||||||<repl key=\"referring.physician:id\"/>^<repl key=\"referring.physician:family.name\"/>^<repl key=\"referring.physician:name\"/>^<repl key=\"referring.physician:family.second.name\"/>^^^^^MI^^^^NNESP^^^^^^^^^ESP&amp;&amp;ISO3166|||||^^||||\"\"^^^^^^^^^\"\"</seg>" \
"			<seg>OBR|<idx expr=\"1+study:idx\"/>|<repl key=\"hce:petition.id\"/>|<repl key=\"message:timestamp.epoch\"/>.<repl key=\"global:pacs.local.aet\"/>^<repl key=\"ginkgocadx.conf:filler.application.id\"/>|<repl key=\"ginkgocadx:module.tid\"/>||||||||||||||<repl key=\"study:accession.number\"/>|||||||||||</seg>" \
"			<seg>TQ1|1||||||||R</seg>" \
"			<sequence key=\"obxseq\" val=\"0\"/>" \
"			<sequence key=\"imgseq\" val=\"0\"/>" \
"			<for.serie>" \
"				<for.image>" \
"					<seg>OBX|<idx expr=\"obxseq.nextval\"/>|HD|121018^Study Instance UID^DCM|<idx expr=\"imgseq.nextval\"/>|^<repl key=\"study:instance.uid\"/>^ISO||||||F|||||<repl key=\"referring.physician:id\"/>^<repl key=\"referring.physician:family.name\"/>^<repl key=\"referring.physician:name\"/>^<repl key=\"referring.physician:family.second.name\"/>^^^^^MI^^^^NNESP^^^^^^^^^ESP&amp;&amp;ISO3166</seg>" \
"					<seg>OBX|<idx expr=\"obxseq.nextval\"/>|HD|112002^Series Instance UID^DCM|<idx expr=\"imgseq.curval\"/>|^<repl key=\"serie:instance.uid\"/>^ISO||||||F|||||<repl key=\"referring.physician:id\"/>^<repl key=\"referring.physician:family.name\"/>^<repl key=\"referring.physician:name\"/>^<repl key=\"referring.physician:family.second.name\"/>^^^^^MI^^^^NNESP^^^^^^^^^ESP&amp;&amp;ISO3166</seg>" \
"					<seg>OBX|<idx expr=\"obxseq.nextval\"/>|HD|121126^SOP Instance UID^DCM|<idx expr=\"imgseq.curval\"/>|^<repl key=\"image:sop.instance.uid\"/>^ISO||||||F|||||<repl key=\"referring.physician:id\"/>^<repl key=\"referring.physician:family.name\"/>^<repl key=\"referring.physician:name\"/>^<repl key=\"referring.physician:family.second.name\"/>^^^^^MI^^^^NNESP^^^^^^^^^ESP&amp;&amp;ISO3166</seg>" \
"					<seg>OBX|<idx expr=\"obxseq.nextval\"/>|HD|121127^SOP Class ID^DCM|<idx expr=\"imgseq.curval\"/>|^<repl key=\"image:sop.class.uid\"/>^ISO||||||F|||||<repl key=\"referring.physician:id\"/>^<repl key=\"referring.physician:family.name\"/>^<repl key=\"referring.physician:name\"/>^<repl key=\"referring.physician:family.second.name\"/>^^^^^MI^^^^NNESP^^^^^^^^^ESP&amp;&amp;ISO3166</seg>" \
"					<seg>OBX|<idx expr=\"obxseq.nextval\"/>|HD|ARCHIVE^IDPACS^DCM|<idx expr=\"imgseq.curval\"/>|<repl key=\"global:pacs.store.sid\"/>^<repl key=\"global:pacs.store.sid\"/>^DNS|||||||||||<repl key=\"referring.physician:id\"/>^<repl key=\"referring.physician:family.name\"/>^<repl key=\"referring.physician:name\"/>^<repl key=\"referring.physician:family.second.name\"/>^^^^^MI^^^^NNESP^^^^^^^^^ESP&amp;&amp;ISO3166</seg>" \
"				</for.image>" \
"			</for.serie>" \
"		</for.study>" \
"		</msg>"


