/*
*  
*  $Id$
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*
*/
#include <wx/richtext/richtextxml.h>
#include "dialogolicenciasopensource.h"

#include <api/globals.h>
#include <main/entorno.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/license.h>
#include <main/controllers/controladorextensiones.h>


DialogoLicenciasOpenSource::DialogoLicenciasOpenSource(wxWindow* pPadre):DialogoLicenciasOpenSourceBase(pPadre)
{
	m_pListaLicencias->Insert(wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetApplicationName().c_str()), 0);
	wxRichTextBuffer::AddHandler(new wxRichTextXMLHandler());
	m_pListaLicencias->Select(0);

	GNC::ControladorExtensiones::ListaModulos listaModulos = GNC::ControladorExtensiones::Instance()->Modulos();
	for (GNC::ControladorExtensiones::ListaModulos::iterator it = listaModulos.begin(); it != listaModulos.end(); ++it)
	{
		GNC::GCS::IModuleController::TExtraLicensesMap extraLicenses = (*it).second->GetExtraLicenses();
		ExtraMap.insert(extraLicenses.begin(), extraLicenses.end());
	}
	for (GNC::GCS::IModuleController::TExtraLicensesMap::iterator it = ExtraMap.begin(); it != ExtraMap.end(); ++it) {
		m_pListaLicencias->Append(wxString::FromUTF8((*it).first.c_str()));
	}
	EscribeLicenciaGinkgo();
}

DialogoLicenciasOpenSource::~DialogoLicenciasOpenSource()
{

}

void DialogoLicenciasOpenSource::OnListaLicencias(wxCommandEvent &)
{
	wxString licencia = m_pListaLicencias->GetStringSelection();
	std::string nombre (licencia.ToUTF8());
	if(nombre == GNC::GCS::IEntorno::Instance()->GetApplicationName()) {
		EscribeLicenciaGinkgo();
	}else if(nombre == "Cairo") {
		EscribeLicenciaCairo();
	} else if (nombre == "DCMTK") {
		EscribeLicenciaDCMTK();
	} else if (nombre == "FamFamFam Icons") {
		EscribeLicenciaFamFam();
	} else if (nombre == "Fugue Icons") {
		EscribeLicenciaFugue();
	} else if (nombre == "ITK") {
		EscribeLicenciaITK();
	} else if (nombre == "LibJpeg") {
		EscribeLicencialibjpeg();
	} else if (nombre == "LibPng") {
		EscribeLicencialibpng();
	} else if (nombre == "OpenSSL") {
		EscribeLicenciaOpenssl();
	} else if (nombre == "VTK") {
		EscribeLicenciaVTK();
	} else if (nombre == "WxWidgets") {
		EscribeLicenciawxWidgets();
	} else if (nombre == "Zlib") {
		EscribeLicenciazlib();
	} else if(nombre == "SQL Lite") {
		EscribeLicenciaSQLLite();
	} else if (nombre == "CharLS"){
		EscribeLicenciaCharLS();
	}else if (nombre == "Dark Glass Icons") {
		WriteDarkGlassLicense();
	} else if (nombre == "Libcurl") {
		WriteLibcurl();
	} else if (nombre == "JsonCpp") {
		WriteLibJsonCpp();
	} else if (nombre == "AIRS") {
		WriteLibAIRS();
	} else if (ExtraMap.find(nombre) != ExtraMap.end()) {
		EscribeTitulo(licencia + wxT("\n\n\n"));
		wxString licenseText = wxString::FromUTF8(ExtraMap[nombre].c_str());
		EscribeLicencia(licenseText);
	} else {
		m_pRichTextTextoLicencia->Clear();
	}
}

void DialogoLicenciasOpenSource::EscribeTitulo(wxString titulo)
{
	m_pRichTextTextoLicencia->Clear();
	m_pRichTextTextoLicencia->SetDefaultStyle(wxRichTextAttr());
	m_pRichTextTextoLicencia->BeginBold();
	m_pRichTextTextoLicencia->BeginFontSize(14);
	m_pRichTextTextoLicencia->WriteText(titulo);
	m_pRichTextTextoLicencia->EndFontSize();
	m_pRichTextTextoLicencia->EndBold();
}

void DialogoLicenciasOpenSource::EscribeLicencia(wxString& licencia)
{
	m_pRichTextTextoLicencia->BeginFontSize(9);
	m_pRichTextTextoLicencia->WriteText(licencia);
	m_pRichTextTextoLicencia->EndFontSize();
	m_pRichTextTextoLicencia->Enable(true);
}

void DialogoLicenciasOpenSource::EscribeLicenciaGinkgo(){
	EscribeTitulo(wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetApplicationName().c_str()) +wxT("\n\n\n"));
	std::string eula = GNC::Entorno::Instance()->GetApplicationEula();
	wxString resultado = wxString::FromUTF8(eula.c_str());
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::EscribeLicenciaCairo(){
	EscribeTitulo(wxT("Cairo\n\n\n"));
	wxString resultado = wxEmptyString;

	resultado += wxT("Mozilla Public License Version 1.1\n");
	resultado += wxT("\n");
	resultado += wxT("1. Definitions.\n");
	resultado += wxT("\n");
	resultado += wxT("1.0.1. \"Commercial Use\"\n");
	resultado += wxT("means distribution or otherwise making the Covered Code available to a third party.\n");
	resultado += wxT("1.1. \"Contributor\"\n");
	resultado += wxT("means each entity that creates or contributes to the creation of Modifications.\n");
	resultado += wxT("1.2. \"Contributor Version\"\n");
	resultado += wxT("means the combination of the Original Code, prior Modifications used by a Contributor, and the Modifications made by that particular Contributor.\n");
	resultado += wxT("1.3. \"Covered Code\"\n");
	resultado += wxT("means the Original Code or Modifications or the combination of the Original Code and Modifications, in each case including portions thereof.\n");
	resultado += wxT("1.4. \"Electronic Distribution Mechanism\"\n");
	resultado += wxT("means a mechanism generally accepted in the software development community for the electronic transfer of data.\n");
	resultado += wxT("1.5. \"Executable\"\n");
	resultado += wxT("means Covered Code in any form other than Source Code.\n");
	resultado += wxT("1.6. \"Initial Developer\"\n");
	resultado += wxT("means the individual or entity identified as the Initial Developer in the Source Code notice required by Exhibit A.\n");
	resultado += wxT("1.7. \"Larger Work\"\n");
	resultado += wxT("means a work which combines Covered Code or portions thereof with code not governed by the terms of this License.\n");
	resultado += wxT("1.8. \"License\"\n");
	resultado += wxT("means this document.\n");
	resultado += wxT("1.8.1. \"Licensable\"\n");
	resultado += wxT("means having the right to grant, to the maximum extent possible, whether at the time of the initial grant or subsequently acquired, any and all of the rights conveyed herein.\n");
	resultado += wxT("1.9. \"Modifications\"\n");
	resultado += wxT("means any addition to or deletion from the substance or structure of either the Original Code or any previous Modifications. When Covered Code is released as a series of files, a Modification is:\n");
	resultado += wxT("Any addition to or deletion from the contents of a file containing Original Code or previous Modifications.\n");
	resultado += wxT("Any new file that contains any part of the Original Code or previous Modifications.\n");
	resultado += wxT("1.10. \"Original Code\"\n");
	resultado += wxT("means Source Code of computer software code which is described in the Source Code notice required by Exhibit A as Original Code, and which, at the time of its release under this License is not already Covered Code governed by this License.\n");
	resultado += wxT("1.10.1. \"Patent Claims\"\n");
	resultado += wxT("means any patent claim(s), now owned or hereafter acquired, including without limitation, method, process, and apparatus claims, in any patent Licensable by grantor.\n");
	resultado += wxT("1.11. \"Source Code\"\n");
	resultado += wxT("means the preferred form of the Covered Code for making modifications to it, including all modules it contains, plus any associated interface definition files, scripts used to control compilation and installation of an Executable, or source code differential comparisons against either the Original Code or another well known, available Covered Code of the Contributor's choice. The Source Code can be in a compressed or archival form, provided the appropriate decompression or de-archiving software is widely available for no charge.\n");
	resultado += wxT("1.12. \"You\" (or \"Your\")\n");
	resultado += wxT("means an individual or a legal entity exercising rights under, and complying with all of the terms of, this License or a future version of this License issued under Section 6.1. For legal entities, \"You\" includes any entity which controls, is controlled by, or is under common control with You. For purposes of this definition, \"control\" means (a) the power, direct or indirect, to cause the direction or management of such entity, whether by contract or otherwise, or (b) ownership of more than fifty percent (50%) of the outstanding shares or beneficial ownership of such entity.\n");
	resultado += wxT("2. Source Code License.\n");
	resultado += wxT("\n");
	resultado += wxT("2.1. The Initial Developer Grant.\n");
	resultado += wxT("\n");
	resultado += wxT("The Initial Developer hereby grants You a world-wide, royalty-free, non-exclusive license, subject to third party intellectual property claims:\n");
	resultado += wxT("\n");
	resultado += wxT("under intellectual property rights (other than patent or trademark) Licensable by Initial Developer to use, reproduce, modify, display, perform, sublicense and distribute the Original Code (or portions thereof) with or without Modifications, and/or as part of a Larger Work; and\n");
	resultado += wxT("under Patents Claims infringed by the making, using or selling of Original Code, to make, have made, use, practice, sell, and offer for sale, and/or otherwise dispose of the Original Code (or portions thereof).\n");
	resultado += wxT("the licenses granted in this Section 2.1 (a) and (b) are effective on the date Initial Developer first distributes Original Code under the terms of this License.\n");
	resultado += wxT("Notwithstanding Section 2.1 (b) above, no patent license is granted: 1) for code that You delete from the Original Code; 2) separate from the Original Code; or 3) for infringements caused by: i) the modification of the Original Code or ii) the combination of the Original Code with other software or devices.\n");
	resultado += wxT("2.2. Contributor Grant.\n");
	resultado += wxT("\n");
	resultado += wxT("Subject to third party intellectual property claims, each Contributor hereby grants You a world-wide, royalty-free, non-exclusive license\n");
	resultado += wxT("\n");
	resultado += wxT("under intellectual property rights (other than patent or trademark) Licensable by Contributor, to use, reproduce, modify, display, perform, sublicense and distribute the Modifications created by such Contributor (or portions thereof) either on an unmodified basis, with other Modifications, as Covered Code and/or as part of a Larger Work; and\n");
	resultado += wxT("under Patent Claims infringed by the making, using, or selling of Modifications made by that Contributor either alone and/or in combination with its Contributor Version (or portions of such combination), to make, use, sell, offer for sale, have made, and/or otherwise dispose of: 1) Modifications made by that Contributor (or portions thereof); and 2) the combination of Modifications made by that Contributor with its Contributor Version (or portions of such combination).\n");
	resultado += wxT("the licenses granted in Sections 2.2 (a) and 2.2 (b) are effective on the date Contributor first makes Commercial Use of the Covered Code.\n");
	resultado += wxT("Notwithstanding Section 2.2 (b) above, no patent license is granted: 1) for any code that Contributor has deleted from the Contributor Version; 2) separate from the Contributor Version; 3) for infringements caused by: i) third party modifications of Contributor Version or ii) the combination of Modifications made by that Contributor with other software (except as part of the Contributor Version) or other devices; or 4) under Patent Claims infringed by Covered Code in the absence of Modifications made by that Contributor.\n");
	resultado += wxT("3. Distribution Obligations.\n");
	resultado += wxT("\n");
	resultado += wxT("3.1. Application of License.\n");
	resultado += wxT("\n");
	resultado += wxT("The Modifications which You create or to which You contribute are governed by the terms of this License, including without limitation Section 2.2. The Source Code version of Covered Code may be distributed only under the terms of this License or a future version of this License released under Section 6.1, and You must include a copy of this License with every copy of the Source Code You distribute. You may not offer or impose any terms on any Source Code version that alters or restricts the applicable version of this License or the recipients' rights hereunder. However, You may include an additional document offering the additional rights described in Section 3.5.\n");
	resultado += wxT("\n");
	resultado += wxT("3.2. Availability of Source Code.\n");
	resultado += wxT("\n");
	resultado += wxT("Any Modification which You create or to which You contribute must be made available in Source Code form under the terms of this License either on the same media as an Executable version or via an accepted Electronic Distribution Mechanism to anyone to whom you made an Executable version available; and if made available via Electronic Distribution Mechanism, must remain available for at least twelve (12) months after the date it initially became available, or at least six (6) months after a subsequent version of that particular Modification has been made available to such recipients. You are responsible for ensuring that the Source Code version remains available even if the Electronic Distribution Mechanism is maintained by a third party.\n");
	resultado += wxT("\n");
	resultado += wxT("3.3. Description of Modifications.\n");
	resultado += wxT("\n");
	resultado += wxT("You must cause all Covered Code to which You contribute to contain a file documenting the changes You made to create that Covered Code and the date of any change. You must include a prominent statement that the Modification is derived, directly or indirectly, from Original Code provided by the Initial Developer and including the name of the Initial Developer in (a) the Source Code, and (b) in any notice in an Executable version or related documentation in which You describe the origin or ownership of the Covered Code.\n");
	resultado += wxT("\n");
	resultado += wxT("3.4. Intellectual Property Matters\n");
	resultado += wxT("\n");
	resultado += wxT("(a) Third Party Claims\n");
	resultado += wxT("\n");
	resultado += wxT("If Contributor has knowledge that a license under a third party's intellectual property rights is required to exercise the rights granted by such Contributor under Sections 2.1 or 2.2, Contributor must include a text file with the Source Code distribution titled \"LEGAL\" which describes the claim and the party making the claim in sufficient detail that a recipient will know whom to contact. If Contributor obtains such knowledge after the Modification is made available as described in Section 3.2, Contributor shall promptly modify the LEGAL file in all copies Contributor makes available thereafter and shall take other steps (such as notifying appropriate mailing lists or newsgroups) reasonably calculated to inform those who received the Covered Code that new knowledge has been obtained.\n");
	resultado += wxT("\n");
	resultado += wxT("(b) Contributor APIs\n");
	resultado += wxT("\n");
	resultado += wxT("If Contributor's Modifications include an application programming interface and Contributor has knowledge of patent licenses which are reasonably necessary to implement that API, Contributor must also include this information in the LEGAL file.\n");
	resultado += wxT("\n");
	resultado += wxT("(c) Representations.\n");
	resultado += wxT("\n");
	resultado += wxT("Contributor represents that, except as disclosed pursuant to Section 3.4 (a) above, Contributor believes that Contributor's Modifications are Contributor's original creation(s) and/or Contributor has sufficient rights to grant the rights conveyed by this License.\n");
	resultado += wxT("\n");
	resultado += wxT("3.5. Required Notices.\n");
	resultado += wxT("\n");
	resultado += wxT("You must duplicate the notice in Exhibit A in each file of the Source Code. If it is not possible to put such notice in a particular Source Code file due to its structure, then You must include such notice in a location (such as a relevant directory) where a user would be likely to look for such a notice. If You created one or more Modification(s) You may add your name as a Contributor to the notice described in Exhibit A. You must also duplicate this License in any documentation for the Source Code where You describe recipients' rights or ownership rights relating to Covered Code. You may choose to offer, and to charge a fee for, warranty, support, indemnity or liability obligations to one or more recipients of Covered Code. However, You may do so only on Your own behalf, and not on behalf of the Initial Developer or any Contributor. You must make it absolutely clear than any such warranty, support, indemnity or liability obligation is offered by You alone, and You hereby agree to indemnify the Initial Developer and every Contributor for any liability incurred by the Initial Developer or such Contributor as a result of warranty, support, indemnity or liability terms You offer.\n");
	resultado += wxT("\n");
	resultado += wxT("3.6. Distribution of Executable Versions.\n");
	resultado += wxT("\n");
	resultado += wxT("You may distribute Covered Code in Executable form only if the requirements of Sections 3.1, 3.2, 3.3, 3.4 and 3.5 have been met for that Covered Code, and if You include a notice stating that the Source Code version of the Covered Code is available under the terms of this License, including a description of how and where You have fulfilled the obligations of Section 3.2. The notice must be conspicuously included in any notice in an Executable version, related documentation or collateral in which You describe recipients' rights relating to the Covered Code. You may distribute the Executable version of Covered Code or ownership rights under a license of Your choice, which may contain terms different from this License, provided that You are in compliance with the terms of this License and that the license for the Executable version does not attempt to limit or alter the recipient's rights in the Source Code version from the rights set forth in this License. If You distribute the Executable version under a different license You must make it absolutely clear that any terms which differ from this License are offered by You alone, not by the Initial Developer or any Contributor. You hereby agree to indemnify the Initial Developer and every Contributor for any liability incurred by the Initial Developer or such Contributor as a result of any such terms You offer.\n");
	resultado += wxT("\n");
	resultado += wxT("3.7. Larger Works.\n");
	resultado += wxT("\n");
	resultado += wxT("You may create a Larger Work by combining Covered Code with other code not governed by the terms of this License and distribute the Larger Work as a single product. In such a case, You must make sure the requirements of this License are fulfilled for the Covered Code.\n");
	resultado += wxT("\n");
	resultado += wxT("4. Inability to Comply Due to Statute or Regulation.\n");
	resultado += wxT("\n");
	resultado += wxT("If it is impossible for You to comply with any of the terms of this License with respect to some or all of the Covered Code due to statute, judicial order, or regulation then You must: (a) comply with the terms of this License to the maximum extent possible; and (b) describe the limitations and the code they affect. Such description must be included in the LEGAL file described in Section 3.4 and must be included with all distributions of the Source Code. Except to the extent prohibited by statute or regulation, such description must be sufficiently detailed for a recipient of ordinary skill to be able to understand it.\n");
	resultado += wxT("\n");
	resultado += wxT("5. Application of this License.\n");
	resultado += wxT("\n");
	resultado += wxT("This License applies to code to which the Initial Developer has attached the notice in Exhibit A and to related Covered Code.\n");
	resultado += wxT("\n");
	resultado += wxT("6. Versions of the License.\n");
	resultado += wxT("\n");
	resultado += wxT("6.1. New Versions\n");
	resultado += wxT("\n");
	resultado += wxT("Netscape Communications Corporation (\"Netscape\") may publish revised and/or new versions of the License from time to time. Each version will be given a distinguishing version number.\n");
	resultado += wxT("\n");
	resultado += wxT("6.2. Effect of New Versions\n");
	resultado += wxT("\n");
	resultado += wxT("Once Covered Code has been published under a particular version of the License, You may always continue to use it under the terms of that version. You may also choose to use such Covered Code under the terms of any subsequent version of the License published by Netscape. No one other than Netscape has the right to modify the terms applicable to Covered Code created under this License.\n");
	resultado += wxT("\n");
	resultado += wxT("6.3. Derivative Works\n");
	resultado += wxT("\n");
	resultado += wxT("If You create or use a modified version of this License (which you may only do in order to apply it to code which is not already Covered Code governed by this License), You must (a) rename Your license so that the phrases \"Mozilla\", \"MOZILLAPL\", \"MOZPL\", \"Netscape\", \"MPL\", \"NPL\" or any confusingly similar phrase do not appear in your license (except to note that your license differs from this License) and (b) otherwise make it clear that Your version of the license contains terms which differ from the Mozilla Public License and Netscape Public License. (Filling in the name of the Initial Developer, Original Code or Contributor in the notice described in Exhibit A shall not of themselves be deemed to be modifications of this License.)\n");
	resultado += wxT("\n");
	resultado += wxT("7. DISCLAIMER OF WARRANTY\n");
	resultado += wxT("\n");
	resultado += wxT("COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN \"AS IS\" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.\n");
	resultado += wxT("\n");
	resultado += wxT("8. Termination\n");
	resultado += wxT("\n");
	resultado += wxT("8.1. This License and the rights granted hereunder will terminate automatically if You fail to comply with terms herein and fail to cure such breach within 30 days of becoming aware of the breach. All sublicenses to the Covered Code which are properly granted shall survive any termination of this License. Provisions which, by their nature, must remain in effect beyond the termination of this License shall survive.\n");
	resultado += wxT("\n");
	resultado += wxT("8.2. If You initiate litigation by asserting a patent infringement claim (excluding declatory judgment actions) against Initial Developer or a Contributor (the Initial Developer or Contributor against whom You file such action is referred to as \"Participant\") alleging that:\n");
	resultado += wxT("\n");
	resultado += wxT("such Participant's Contributor Version directly or indirectly infringes any patent, then any and all rights granted by such Participant to You under Sections 2.1 and/or 2.2 of this License shall, upon 60 days notice from Participant terminate prospectively, unless if within 60 days after receipt of notice You either: (i) agree in writing to pay Participant a mutually agreeable reasonable royalty for Your past and future use of Modifications made by such Participant, or (ii) withdraw Your litigation claim with respect to the Contributor Version against such Participant. If within 60 days of notice, a reasonable royalty and payment arrangement are not mutually agreed upon in writing by the parties or the litigation claim is not withdrawn, the rights granted by Participant to You under Sections 2.1 and/or 2.2 automatically terminate at the expiration of the 60 day notice period specified above.\n");
	resultado += wxT("any software, hardware, or device, other than such Participant's Contributor Version, directly or indirectly infringes any patent, then any rights granted to You by such Participant under Sections 2.1(b) and 2.2(b) are revoked effective as of the date You first made, used, sold, distributed, or had made, Modifications made by that Participant.\n");
	resultado += wxT("8.3. If You assert a patent infringement claim against Participant alleging that such Participant's Contributor Version directly or indirectly infringes any patent where such claim is resolved (such as by license or settlement) prior to the initiation of patent infringement litigation, then the reasonable value of the licenses granted by such Participant under Sections 2.1 or 2.2 shall be taken into account in determining the amount or value of any payment or license.\n");
	resultado += wxT("\n");
	resultado += wxT("8.4. In the event of termination under Sections 8.1 or 8.2 above, all end user license agreements (excluding distributors and resellers) which have been validly granted by You or any distributor hereunder prior to termination shall survive termination.\n");
	resultado += wxT("\n");
	resultado += wxT("9. LIMITATION OF LIABILITY\n");
	resultado += wxT("\n");
	resultado += wxT("UNDER NO CIRCUMSTANCES AND UNDER NO LEGAL THEORY, WHETHER TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE, SHALL YOU, THE INITIAL DEVELOPER, ANY OTHER CONTRIBUTOR, OR ANY DISTRIBUTOR OF COVERED CODE, OR ANY SUPPLIER OF ANY OF SUCH PARTIES, BE LIABLE TO ANY PERSON FOR ANY INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OF ANY CHARACTER INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF GOODWILL, WORK STOPPAGE, COMPUTER FAILURE OR MALFUNCTION, OR ANY AND ALL OTHER COMMERCIAL DAMAGES OR LOSSES, EVEN IF SUCH PARTY SHALL HAVE BEEN INFORMED OF THE POSSIBILITY OF SUCH DAMAGES. THIS LIMITATION OF LIABILITY SHALL NOT APPLY TO LIABILITY FOR DEATH OR PERSONAL INJURY RESULTING FROM SUCH PARTY'S NEGLIGENCE TO THE EXTENT APPLICABLE LAW PROHIBITS SUCH LIMITATION. SOME JURISDICTIONS DO NOT ALLOW THE EXCLUSION OR LIMITATION OF INCIDENTAL OR CONSEQUENTIAL DAMAGES, SO THIS EXCLUSION AND LIMITATION MAY NOT APPLY TO YOU.\n");
	resultado += wxT("\n");
	resultado += wxT("10. U.S. government end users\n");
	resultado += wxT("\n");
	resultado += wxT("The Covered Code is a \"commercial item,\" as that term is defined in 48 C.F.R. 2.101 (Oct. 1995), consisting of \"commercial computer software\" and \"commercial computer software documentation,\" as such terms are used in 48 C.F.R. 12.212 (Sept. 1995). Consistent with 48 C.F.R. 12.212 and 48 C.F.R. 227.7202-1 through 227.7202-4 (June 1995), all U.S. Government End Users acquire Covered Code with only those rights set forth herein.\n");
	resultado += wxT("\n");
	resultado += wxT("11. Miscellaneous\n");
	resultado += wxT("\n");
	resultado += wxT("This License represents the complete agreement concerning subject matter hereof. If any provision of this License is held to be unenforceable, such provision shall be reformed only to the extent necessary to make it enforceable. This License shall be governed by California law provisions (except to the extent applicable law, if any, provides otherwise), excluding its conflict-of-law provisions. With respect to disputes in which at least one party is a citizen of, or an entity chartered or registered to do business in the United States of America, any litigation relating to this License shall be subject to the jurisdiction of the Federal Courts of the Northern District of California, with venue lying in Santa Clara County, California, with the losing party responsible for costs, including without limitation, court costs and reasonable attorneys' fees and expenses. The application of the United Nations Convention on Contracts for the International Sale of Goods is expressly excluded. Any law or regulation which provides that the language of a contract shall be construed against the drafter shall not apply to this License.\n");
	resultado += wxT("\n");
	resultado += wxT("12. Responsibility for claims\n");
	resultado += wxT("\n");
	resultado += wxT("As between Initial Developer and the Contributors, each party is responsible for claims and damages arising, directly or indirectly, out of its utilization of rights under this License and You agree to work with Initial Developer and Contributors to distribute such responsibility on an equitable basis. Nothing herein is intended or shall be deemed to constitute any admission of liability.\n");
	resultado += wxT("\n");
	resultado += wxT("13. Multiple-licensed code\n");
	resultado += wxT("\n");
	resultado += wxT("Initial Developer may designate portions of the Covered Code as \"Multiple-Licensed\". \"Multiple-Licensed\" means that the Initial Developer permits you to utilize portions of the Covered Code under Your choice of the MPL or the alternative licenses, if any, specified by the Initial Developer in the file described in Exhibit A.\n");
	resultado += wxT("\n");
	resultado += wxT("Exhibit A - Mozilla Public License.\n");
	resultado += wxT("\n");
	resultado += wxT("\"The contents of this file are subject to the Mozilla Public License\n");
	resultado += wxT("Version 1.1 (the \"License\"); you may not use this file except in\n");
	resultado += wxT("compliance with the License. You may obtain a copy of the License at\n");
	resultado += wxT("http://www.mozilla.org/MPL/\n");
	resultado += wxT("\n");
	resultado += wxT("Software distributed under the License is distributed on an \"AS IS\"\n");
	resultado += wxT("basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the\n");
	resultado += wxT("License for the specific language governing rights and limitations\n");
	resultado += wxT("under the License.\n");
	resultado += wxT("\n");
	resultado += wxT("The Original Code is ______________________________________.\n");
	resultado += wxT("\n");
	resultado += wxT("The Initial Developer of the Original Code is ________________________.\n");
	resultado += wxT("Portions created by ______________________ are Copyright (C) ______\n");
	resultado += wxT("_______________________. All Rights Reserved.\n");
	resultado += wxT("\n");
	resultado += wxT("Contributor(s): ______________________________________.\n");
	resultado += wxT("\n");
	resultado += wxT("Alternatively, the contents of this file may be used under the terms\n");
	resultado += wxT("of the _____ license (the  \"[___] License\"), in which case the\n");
	resultado += wxT("provisions of [______] License are applicable instead of those\n");
	resultado += wxT("above. If you wish to allow use of your version of this file only\n");
	resultado += wxT("under the terms of the [____] License and not to allow others to use\n");
	resultado += wxT("your version of this file under the MPL, indicate your decision by\n");
	resultado += wxT("deleting the provisions above and replace them with the notice and\n");
	resultado += wxT("other provisions required by the [___] License. If you do not delete\n");
	resultado += wxT("the provisions above, a recipient may use your version of this file\n");
	resultado += wxT("under either the MPL or the [___] License.\"\n");
	resultado += wxT("NOTE: The text of this Exhibit A may differ slightly from the text of the notices in the Source Code files of the Original Code. You should use the text of this Exhibit A rather than the text found in the Original Code Source Code for Your Modifications.\n\n");
	EscribeLicencia(resultado);
}


void DialogoLicenciasOpenSource::EscribeLicenciaDCMTK()
{
	EscribeTitulo(wxT("DCMTK\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("COPYRIGHT\n");
	resultado += wxT("\n");
	resultado += wxT("Unless otherwise specified, the DCMTK software package has the\n");
	resultado += wxT("following copyright:\n");
	resultado += wxT("\n");
	resultado += wxT("/*\n");
	resultado += wxT(" *  Copyright (C) 1994-2004, OFFIS\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  This software and supporting documentation were developed by\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *    Kuratorium OFFIS e.V.\n");
	resultado += wxT(" *    Healthcare Information and Communication Systems\n");
	resultado += wxT(" *    Escherweg 2\n");
	resultado += wxT(" *    D-26121 Oldenburg, Germany\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY\n");
	resultado += wxT(" *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR\n");
	resultado += wxT(" *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR\n");
	resultado += wxT(" *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND\n");
	resultado += wxT(" *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  Copyright of the software  and  supporting  documentation  is,  unless\n");
	resultado += wxT(" *  otherwise stated, owned by OFFIS, and free access is hereby granted as\n");
	resultado += wxT(" *  a license to  use  this  software,  copy  this  software  and  prepare\n");
	resultado += wxT(" *  derivative works based upon this software.  However, any  distribution\n");
	resultado += wxT(" *  of this software source code or supporting documentation or derivative\n");
	resultado += wxT(" *  works  (source code and  supporting documentation)  must  include  the\n");
	resultado += wxT(" *  three paragraphs of this copyright notice.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" */\n");
	resultado += wxT("\n");
	resultado += wxT("Some portions of the DCMTK software package are derived from earlier\n");
	resultado += wxT("versions of this software with the following copyright, and can be\n");
	resultado += wxT("identifed by the following copyright notice located in each source file:\n");
	resultado += wxT("\n");
	resultado += wxT("/*  \n");
	resultado += wxT(" *  Copyright (C) 1993/1994, OFFIS, Oldenburg University and CERIUM\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *  This software and supporting documentation were\n");
	resultado += wxT(" *  developed by\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *    Institut OFFIS\n");
	resultado += wxT(" *    Bereich Kommunikationssysteme\n");
	resultado += wxT(" *    Westerstr. 10-12\n");
	resultado += wxT(" *    26121 Oldenburg, Germany\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *    Fachbereich Informatik\n");
	resultado += wxT(" *    Abteilung Prozessinformatik\n");
	resultado += wxT(" *    Carl von Ossietzky Universitaet Oldenburg\n");
	resultado += wxT(" *    Ammerlaender Heerstr. 114-118\n");
	resultado += wxT(" *    26111 Oldenburg, Germany\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *    CERIUM\n");
	resultado += wxT(" *    Laboratoire SIM\n");
	resultado += wxT(" *    Faculte de Medecine\n");
	resultado += wxT(" *    2 Avenue du Pr. Leon Bernard\n");
	resultado += wxT(" *    35043 Rennes Cedex, France\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *  for CEN/TC251/WG4 as a contribution to the Radiological\n");
	resultado += wxT(" *  Society of North America (RSNA) 1993 Digital Imaging and\n");
	resultado += wxT(" *  Communications in Medicine (DICOM) Demonstration.\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *  THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER OFFIS,\n");
	resultado += wxT(" *  OLDENBURG UNIVERSITY NOR CERIUM MAKE ANY WARRANTY REGARDING\n");
	resultado += wxT(" *  THE SOFTWARE, ITS PERFORMANCE, ITS MERCHANTABILITY OR\n");
	resultado += wxT(" *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER\n");
	resultado += wxT(" *  DISEASES OR ITS CONFORMITY TO ANY SPECIFICATION.  THE\n");
	resultado += wxT(" *  ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF THE SOFTWARE\n");
	resultado += wxT(" *  IS WITH THE USER.\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *  Copyright of the software and supporting documentation\n");
	resultado += wxT(" *  is, unless otherwise stated, jointly owned by OFFIS,\n");
	resultado += wxT(" *  Oldenburg University and CERIUM and free access is hereby\n");
	resultado += wxT(" *  granted as a license to use this software, copy this\n");
	resultado += wxT(" *  software and prepare derivative works based upon this\n");
	resultado += wxT(" *  software. However, any distribution of this software\n");
	resultado += wxT(" *  source code or supporting documentation or derivative\n");
	resultado += wxT(" *  works (source code and supporting documentation) must\n");
	resultado += wxT(" *  include the three paragraphs of this copyright notice.\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" */ \n");
	resultado += wxT("\n");
	resultado += wxT("Some other parts of this software within the dcmtk/dcmnet\n");
	resultado += wxT("sub-package related to the DICOM Upper Layer Protocol are\n");
	resultado += wxT("derived from software developed for the RSNA'93 DICOM\n");
	resultado += wxT("demonstration and kindly made available to us by the Mallinckrodt\n");
	resultado += wxT("Institute of Radiology.  Such software can be identifed by the\n");
	resultado += wxT("following copyright notice located in each affected source file:\n");
	resultado += wxT("\n");
	resultado += wxT("/*\n");
	resultado += wxT(" *  Copyright (C) 1993, RSNA and Washington University\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  The software and supporting documentation for the Radiological\n");
	resultado += wxT(" *  Society of North America (RSNA) 1993 Digital Imaging and\n");
	resultado += wxT(" *  Communications in Medicine (DICOM) Demonstration were developed\n");
	resultado += wxT(" *  at the\n");
	resultado += wxT(" *          Electronic Radiology Laboratory\n");
	resultado += wxT(" *          Mallinckrodt Institute of Radiology\n");
	resultado += wxT(" *          Washington University School of Medicine\n");
	resultado += wxT(" *          510 S. Kingshighway Blvd.\n");
	resultado += wxT(" *          St. Louis, MO 63110\n");
	resultado += wxT(" *  as part of the 1993 DICOM Central Test Node project for, and\n");
	resultado += wxT(" *  under contract with, the Radiological Society of North America.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER RSNA NOR\n");
	resultado += wxT(" *  WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS\n");
	resultado += wxT(" *  PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR\n");
	resultado += wxT(" *  USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY\n");
	resultado += wxT(" *  SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF\n");
	resultado += wxT(" *  THE SOFTWARE IS WITH THE USER.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  Copyright of the software and supporting documentation is\n");
	resultado += wxT(" *  jointly owned by RSNA and Washington University, and free access\n");
	resultado += wxT(" *  is hereby granted as a license to use this software, copy this\n");
	resultado += wxT(" *  software and prepare derivative works based upon this software.\n");
	resultado += wxT(" *  However, any distribution of this software source code or\n");
	resultado += wxT(" *  supporting documentation or derivative works (source code and\n");
	resultado += wxT(" *  supporting documentation) must include the three paragraphs of\n");
	resultado += wxT(" *  the copyright notice.\n");
	resultado += wxT(" */\n");
	resultado += wxT("\n");
	resultado += wxT("The dcmjpeg sub-package includes an adapted version of the Independent JPEG\n");
	resultado += wxT("Group Toolkit Version 6b, which is contained in dcmjpeg/libijg8,\n");
	resultado += wxT("dcmjpeg/libijg12 and dcmjpeg/libijg16.  This toolkit is covered by the\n");
	resultado += wxT("following copyright.  The original README file for the Independent JPEG\n");
	resultado += wxT("Group Toolkit is located in dcmjpeg/docs/ijg_readme.txt.\n");
	resultado += wxT("\n");
	resultado += wxT("/*\n");
	resultado += wxT(" *  The authors make NO WARRANTY or representation, either express or implied,\n");
	resultado += wxT(" *  with respect to this software, its quality, accuracy, merchantability, or\n");
	resultado += wxT(" *  fitness for a particular purpose.  This software is provided \"AS IS\", and you,\n");
	resultado += wxT(" *  its user, assume the entire risk as to its quality and accuracy.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  This software is copyright (C) 1991-1998, Thomas G. Lane.\n");
	resultado += wxT(" *  All Rights Reserved except as specified below.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  Permission is hereby granted to use, copy, modify, and distribute this\n");
	resultado += wxT(" *  software (or portions thereof) for any purpose, without fee, subject to these\n");
	resultado += wxT(" *  conditions:\n");
	resultado += wxT(" *  (1) If any part of the source code for this software is distributed, then this\n");
	resultado += wxT(" *  README file must be included, with this copyright and no-warranty notice\n");
	resultado += wxT(" *  unaltered; and any additions, deletions, or changes to the original files\n");
	resultado += wxT(" *  must be clearly indicated in accompanying documentation.\n");
	resultado += wxT(" *  (2) If only executable code is distributed, then the accompanying\n");
	resultado += wxT(" *  documentation must state that \"this software is based in part on the work of\n");
	resultado += wxT(" *  the Independent JPEG Group\".\n");
	resultado += wxT(" *  (3) Permission for use of this software is granted only if the user accepts\n");
	resultado += wxT(" *  full responsibility for any undesirable consequences; the authors accept\n");
	resultado += wxT(" *  NO LIABILITY for damages of any kind.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  These conditions apply to any software derived from or based on the IJG code,\n");
	resultado += wxT(" *  not just to the unmodified library.  If you use our work, you ought to\n");
	resultado += wxT(" *  acknowledge us.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  Permission is NOT granted for the use of any IJG author's name or company name\n");
	resultado += wxT(" *  in advertising or publicity relating to this software or products derived from\n");
	resultado += wxT(" *  it.  This software may be referred to only as \"the Independent JPEG Group's\n");
	resultado += wxT(" *  software\".\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  We specifically permit and encourage the use of this software as the basis of\n");
	resultado += wxT(" *  commercial products, provided that all warranty or liability claims are\n");
	resultado += wxT(" *  assumed by the product vendor.\n");
	resultado += wxT(" */\n");
	resultado += wxT("\n");
	resultado += wxT("\n");
	resultado += wxT("The color quantization code in module dcmimage (dcmquant and the related\n");
	resultado += wxT("classes) is derived from code written by Jef Poskanzer for the NetPBM\n");
	resultado += wxT("toolkit which has the following copyright:\n");
	resultado += wxT("\n");
	resultado += wxT("/*\n");
	resultado += wxT(" * Copyright (C) 1989, 1991 by Jef Poskanzer.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" * Permission to use, copy, modify, and distribute this software and its\n");
	resultado += wxT(" * documentation for any purpose and without fee is hereby granted, provided\n");
	resultado += wxT(" * that the above copyright notice appear in all copies and that both that\n");
	resultado += wxT(" * copyright notice and this permission notice appear in supporting\n");
	resultado += wxT(" * documentation.  This software is provided \"as is\" without express or\n");
	resultado += wxT(" * implied warranty.\n");
	resultado += wxT(" */\n");
	resultado += wxT("\n");
	resultado += wxT("\n");
	resultado += wxT("The code for the OFStandard::strlcpy and OFStandard::strlcat helper\n");
	resultado += wxT("functions in ofstd/libsrc/ofstd.cc has been derived from the BSD\n");
	resultado += wxT("implementation of strlcpy() and strlcat() and which carries the\n");
	resultado += wxT("following copyright notice:\n");
	resultado += wxT("\n");
	resultado += wxT("/*\n");
	resultado += wxT(" *  Copyright (c) 1998 Todd C. Miller <Todd.Miller(at)courtesan.com>\n");
	resultado += wxT(" *  All rights reserved.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  Redistribution and use in source and binary forms, with or without\n");
	resultado += wxT(" *  modification, are permitted provided that the following conditions\n");
	resultado += wxT(" *  are met:\n");
	resultado += wxT(" *  1. Redistributions of source code must retain the above copyright\n");
	resultado += wxT(" *     notice, this list of conditions and the following disclaimer.\n");
	resultado += wxT(" *  2. Redistributions in binary form must reproduce the above copyright\n");
	resultado += wxT(" *     notice, this list of conditions and the following disclaimer in the\n");
	resultado += wxT(" *     documentation and/or other materials provided with the distribution.\n");
	resultado += wxT(" *  3. The name of the author may not be used to endorse or promote products\n");
	resultado += wxT(" *     derived from this software without specific prior written permission.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,\n");
	resultado += wxT(" *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY\n");
	resultado += wxT(" *  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL\n");
	resultado += wxT(" *  THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,\n");
	resultado += wxT(" *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,\n");
	resultado += wxT(" *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;\n");
	resultado += wxT(" *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,\n");
	resultado += wxT(" *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR\n");
	resultado += wxT(" *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF\n");
	resultado += wxT(" *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");
	resultado += wxT(" */\n");
	resultado += wxT("\n");
	resultado += wxT("\n");
	resultado += wxT("The code for the OFStandard::atof helper function in\n");
	resultado += wxT("ofstd/libsrc/ofstd.cc has been derived from an implementation which\n");
	resultado += wxT("carries the following copyright notice:\n");
	resultado += wxT("\n");
	resultado += wxT("/*\n");
	resultado += wxT(" *  Copyright 1988 Regents of the University of California\n");
	resultado += wxT(" *  Permission to use, copy, modify, and distribute this software and\n");
	resultado += wxT(" *  its documentation for any purpose and without fee is hereby granted,\n");
	resultado += wxT(" *  provided that the above copyright notice appear in all copies.  The\n");
	resultado += wxT(" *  University of California makes no representations about the\n");
	resultado += wxT(" *  suitability of this software for any purpose.  It is provided \"as\n");
	resultado += wxT(" *  is\" without express or implied warranty.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  The code for OFStandard::ftoa has been derived\n");
	resultado += wxT(" *  from an implementation which carries the following copyright notice:\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  Copyright (c) 1988 Regents of the University of California.\n");
	resultado += wxT(" *  All rights reserved.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  Redistribution and use in source and binary forms are permitted\n");
	resultado += wxT(" *  provided that the above copyright notice and this paragraph are\n");
	resultado += wxT(" *  duplicated in all such forms and that any documentation,\n");
	resultado += wxT(" *  advertising materials, and other materials related to such\n");
	resultado += wxT(" *  distribution and use acknowledge that the software was developed\n");
	resultado += wxT(" *  by the University of California, Berkeley.  The name of the\n");
	resultado += wxT(" *  University may not be used to endorse or promote products derived\n");
	resultado += wxT(" *  from this software without specific prior written permission.\n");
	resultado += wxT(" *  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR\n");
	resultado += wxT(" *  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED\n");
	resultado += wxT(" *  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n");
	resultado += wxT(" */\n");
	resultado += wxT("\n");
	resultado += wxT("The \"Base64\" encoder/decoder in ofstd/libsrc/ofstd.cc has been derived\n");
	resultado += wxT("from an implementation which carries the following copyright notice:\n");
	resultado += wxT("\n");
	resultado += wxT("/*\n");
	resultado += wxT(" *  Copyright (c) 1999, Bob Withers - bwit(at)pobox.com\n");
	resultado += wxT(" *\n");
	resultado += wxT(" *  This code may be freely used for any purpose, either personal or commercial,\n");
	resultado += wxT(" *  provided the authors copyright notice remains intact.\n");
	resultado += wxT(" */\n");
	resultado += wxT("\n");
	resultado += wxT("The dcmjp2k sub-package (which is currently not part of the free \n");
	resultado += wxT("toolkit) includes an adapted version of the JasPer JPEG 2000 toolkit, \n");
	resultado += wxT("which is contained in dcmjp2k/libjaspr. This toolkit is covered by \n");
	resultado += wxT("the following copyright.\n");
	resultado += wxT("\n");
	resultado += wxT("/*  JasPer License Version 2.0\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *  Copyright (c) 1999-2000 Image Power, Inc.\n");
	resultado += wxT(" *  Copyright (c) 1999-2000 The University of British Columbia\n");
	resultado += wxT(" *  Copyright (c) 2001-2003 Michael David Adams\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *  All rights reserved.\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *  Permission is hereby granted, free of charge, to any person (the\n");
	resultado += wxT(" *  \"User\") obtaining a copy of this software and associated documentation\n");
	resultado += wxT(" *  files (the \"Software\"), to deal in the Software without restriction,\n");
	resultado += wxT(" *  including without limitation the rights to use, copy, modify, merge,\n");
	resultado += wxT(" *  publish, distribute, and/or sell copies of the Software, and to permit\n");
	resultado += wxT(" *  persons to whom the Software is furnished to do so, subject to the\n");
	resultado += wxT(" *  following conditions:\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *  1.  The above copyright notices and this permission notice (which\n");
	resultado += wxT(" *  includes the disclaimer below) shall be included in all copies or\n");
	resultado += wxT(" *  substantial portions of the Software.\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *  2.  The name of a copyright holder shall not be used to endorse or\n");
	resultado += wxT(" *  promote products derived from the Software without specific prior\n");
	resultado += wxT(" *  written permission.\n");
	resultado += wxT(" *  \n");
	resultado += wxT(" *  THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS\n");
	resultado += wxT(" *  LICENSE.  NO USE OF THE SOFTWARE IS AUTHORIZED HEREUNDER EXCEPT UNDER\n");
	resultado += wxT(" *  THIS DISCLAIMER.  THE SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS\n");
	resultado += wxT(" *  \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING\n");
	resultado += wxT(" *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A\n");
	resultado += wxT(" *  PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.  IN NO\n");
	resultado += wxT(" *  EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL\n");
	resultado += wxT(" *  INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING\n");
	resultado += wxT(" *  FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,\n");
	resultado += wxT(" *  NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION\n");
	resultado += wxT(" *  WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.  NO ASSURANCES ARE\n");
	resultado += wxT(" *  PROVIDED BY THE COPYRIGHT HOLDERS THAT THE SOFTWARE DOES NOT INFRINGE\n");
	resultado += wxT(" *  THE PATENT OR OTHER INTELLECTUAL PROPERTY RIGHTS OF ANY OTHER ENTITY.\n");
	resultado += wxT(" *  EACH COPYRIGHT HOLDER DISCLAIMS ANY LIABILITY TO THE USER FOR CLAIMS\n");
	resultado += wxT(" *  BROUGHT BY ANY OTHER ENTITY BASED ON INFRINGEMENT OF INTELLECTUAL\n");
	resultado += wxT(" *  PROPERTY RIGHTS OR OTHERWISE.  AS A CONDITION TO EXERCISING THE RIGHTS\n");
	resultado += wxT(" *  GRANTED HEREUNDER, EACH USER HEREBY ASSUMES SOLE RESPONSIBILITY TO SECURE\n");
	resultado += wxT(" *  ANY OTHER INTELLECTUAL PROPERTY RIGHTS NEEDED, IF ANY.  THE SOFTWARE\n");
	resultado += wxT(" *  IS NOT FAULT-TOLERANT AND IS NOT INTENDED FOR USE IN MISSION-CRITICAL\n");
	resultado += wxT(" *  SYSTEMS, SUCH AS THOSE USED IN THE OPERATION OF NUCLEAR FACILITIES,\n");
	resultado += wxT(" *  AIRCRAFT NAVIGATION OR COMMUNICATION SYSTEMS, AIR TRAFFIC CONTROL\n");
	resultado += wxT(" *  SYSTEMS, DIRECT LIFE SUPPORT MACHINES, OR WEAPONS SYSTEMS, IN WHICH\n");
	resultado += wxT(" *  THE FAILURE OF THE SOFTWARE OR SYSTEM COULD LEAD DIRECTLY TO DEATH,\n");
	resultado += wxT(" *  PERSONAL INJURY, OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE (\"HIGH\n");
	resultado += wxT(" *  RISK ACTIVITIES\").  THE COPYRIGHT HOLDERS SPECIFICALLY DISCLAIM ANY\n");
	resultado += wxT(" *  EXPRESS OR IMPLIED WARRANTY OF FITNESS FOR HIGH RISK ACTIVITIES.\n");
	resultado += wxT(" */ \n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::EscribeLicenciaFamFam()
{
	EscribeTitulo(wxT("FamFamFam icons\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("Silk icon set 1.3\n");
	resultado += wxT("\n");
	resultado += wxT("_________________________________________\n");
	resultado += wxT("Mark James\n");
	resultado += wxT("http://www.famfamfam.com/lab/icons/silk/\n");
	resultado += wxT("_________________________________________\n");
	resultado += wxT("Creative Commons\n");
	resultado += wxT("\n");
	resultado += wxT("Attribution 2.5\n");
	resultado += wxT("\n");
	resultado += wxT("CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF THIS LICENSE DOES NOT CREATE AN ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS INFORMATION ON AN \"AS-IS\" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES REGARDING THE INFORMATION PROVIDED, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM ITS USE.\n");
	resultado += wxT("License\n");
	resultado += wxT("\n");
	resultado += wxT("THE WORK (AS DEFINED BELOW) IS PROVIDED UNDER THE TERMS OF THIS CREATIVE COMMONS PUBLIC LICENSE (\"CCPL\" OR \"LICENSE\"). THE WORK IS PROTECTED BY COPYRIGHT AND/OR OTHER APPLICABLE LAW. ANY USE OF THE WORK OTHER THAN AS AUTHORIZED UNDER THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.\n");
	resultado += wxT("\n");
	resultado += wxT("BY EXERCISING ANY RIGHTS TO THE WORK PROVIDED HERE, YOU ACCEPT AND AGREE TO BE BOUND BY THE TERMS OF THIS LICENSE. THE LICENSOR GRANTS YOU THE RIGHTS CONTAINED HERE IN CONSIDERATION OF YOUR ACCEPTANCE OF SUCH TERMS AND CONDITIONS.\n");
	resultado += wxT("\n");
	resultado += wxT("1. Definitions\n");
	resultado += wxT("\n");
	resultado += wxT("\"Collective Work\" means a work, such as a periodical issue, anthology or encyclopedia, in which the Work in its entirety in unmodified form, along with a number of other contributions, constituting separate and independent works in themselves, are assembled into a collective whole. A work that constitutes a Collective Work will not be considered a Derivative Work (as defined below) for the purposes of this License.\n");
	resultado += wxT("\"Derivative Work\" means a work based upon the Work or upon the Work and other pre-existing works, such as a translation, musical arrangement, dramatization, fictionalization, motion picture version, sound recording, art reproduction, abridgment, condensation, or any other form in which the Work may be recast, transformed, or adapted, except that a work that constitutes a Collective Work will not be considered a Derivative Work for the purpose of this License. For the avoidance of doubt, where the Work is a musical composition or sound recording, the synchronization of the Work in timed-relation with a moving image (\"synching\") will be considered a Derivative Work for the purpose of this License.\n");
	resultado += wxT("\"Licensor\" means the individual or entity that offers the Work under the terms of this License.\n");
	resultado += wxT("\"Original Author\" means the individual or entity who created the Work.\n");
	resultado += wxT("\"Work\" means the copyrightable work of authorship offered under the terms of this License.\n");
	resultado += wxT("\"You\" means an individual or entity exercising rights under this License who has not previously violated the terms of this License with respect to the Work, or who has received express permission from the Licensor to exercise rights under this License despite a previous violation.\n");
	resultado += wxT("2. Fair Use Rights. Nothing in this license is intended to reduce, limit, or restrict any rights arising from fair use, first sale or other limitations on the exclusive rights of the copyright owner under copyright law or other applicable laws.\n");
	resultado += wxT("\n");
	resultado += wxT("3. License Grant. Subject to the terms and conditions of this License, Licensor hereby grants You a worldwide, royalty-free, non-exclusive, perpetual (for the duration of the applicable copyright) license to exercise the rights in the Work as stated below:\n");
	resultado += wxT("\n");
	resultado += wxT("to reproduce the Work, to incorporate the Work into one or more Collective Works, and to reproduce the Work as incorporated in the Collective Works;\n");
	resultado += wxT("to create and reproduce Derivative Works;\n");
	resultado += wxT("to distribute copies or phonorecords of, display publicly, perform publicly, and perform publicly by means of a digital audio transmission the Work including as incorporated in Collective Works;\n");
	resultado += wxT("to distribute copies or phonorecords of, display publicly, perform publicly, and perform publicly by means of a digital audio transmission Derivative Works.\n");
	resultado += wxT("For the avoidance of doubt, where the work is a musical composition:\n");
	resultado += wxT("\n");
	resultado += wxT("Performance Royalties Under Blanket Licenses. Licensor waives the exclusive right to collect, whether individually or via a performance rights society (e.g. ASCAP, BMI, SESAC), royalties for the public performance or public digital performance (e.g. webcast) of the Work.\n");
	resultado += wxT("Mechanical Rights and Statutory Royalties. Licensor waives the exclusive right to collect, whether individually or via a music rights agency or designated agent (e.g. Harry Fox Agency), royalties for any phonorecord You create from the Work (\"cover version\") and distribute, subject to the compulsory license created by 17 USC Section 115 of the US Copyright Act (or the equivalent in other jurisdictions).\n");
	resultado += wxT("Webcasting Rights and Statutory Royalties. For the avoidance of doubt, where the Work is a sound recording, Licensor waives the exclusive right to collect, whether individually or via a performance-rights society (e.g. SoundExchange), royalties for the public digital performance (e.g. webcast) of the Work, subject to the compulsory license created by 17 USC Section 114 of the US Copyright Act (or the equivalent in other jurisdictions).\n");
	resultado += wxT("The above rights may be exercised in all media and formats whether now known or hereafter devised. The above rights include the right to make such modifications as are technically necessary to exercise the rights in other media and formats. All rights not expressly granted by Licensor are hereby reserved.\n");
	resultado += wxT("\n");
	resultado += wxT("4. Restrictions.The license granted in Section 3 above is expressly made subject to and limited by the following restrictions:\n");
	resultado += wxT("\n");
	resultado += wxT("You may distribute, publicly display, publicly perform, or publicly digitally perform the Work only under the terms of this License, and You must include a copy of, or the Uniform Resource Identifier for, this License with every copy or phonorecord of the Work You distribute, publicly display, publicly perform, or publicly digitally perform. You may not offer or impose any terms on the Work that alter or restrict the terms of this License or the recipients' exercise of the rights granted hereunder. You may not sublicense the Work. You must keep intact all notices that refer to this License and to the disclaimer of warranties. You may not distribute, publicly display, publicly perform, or publicly digitally perform the Work with any technological measures that control access or use of the Work in a manner inconsistent with the terms of this License Agreement. The above applies to the Work as incorporated in a Collective Work, but this does not require the Collective Work apart from the Work itself to be made subject to the terms of this License. If You create a Collective Work, upon notice from any Licensor You must, to the extent practicable, remove from the Collective Work any credit as required by clause 4(b), as requested. If You create a Derivative Work, upon notice from any Licensor You must, to the extent practicable, remove from the Derivative Work any credit as required by clause 4(b), as requested.\n");
	resultado += wxT("If you distribute, publicly display, publicly perform, or publicly digitally perform the Work or any Derivative Works or Collective Works, You must keep intact all copyright notices for the Work and provide, reasonable to the medium or means You are utilizing: (i) the name of the Original Author (or pseudonym, if applicable) if supplied, and/or (ii) if the Original Author and/or Licensor designate another party or parties (e.g. a sponsor institute, publishing entity, journal) for attribution in Licensor's copyright notice, terms of service or by other reasonable means, the name of such party or parties; the title of the Work if supplied; to the extent reasonably practicable, the Uniform Resource Identifier, if any, that Licensor specifies to be associated with the Work, unless such URI does not refer to the copyright notice or licensing information for the Work; and in the case of a Derivative Work, a credit identifying the use of the Work in the Derivative Work (e.g., \"French translation of the Work by Original Author,\" or \"Screenplay based on original Work by Original Author\"). Such credit may be implemented in any reasonable manner; provided, however, that in the case of a Derivative Work or Collective Work, at a minimum such credit will appear where any other comparable authorship credit appears and in a manner at least as prominent as such other comparable authorship credit.\n");
	resultado += wxT("5. Representations, Warranties and Disclaimer\n");
	resultado += wxT("\n");
	resultado += wxT("UNLESS OTHERWISE MUTUALLY AGREED TO BY THE PARTIES IN WRITING, LICENSOR OFFERS THE WORK AS-IS AND MAKES NO REPRESENTATIONS OR WARRANTIES OF ANY KIND CONCERNING THE WORK, EXPRESS, IMPLIED, STATUTORY OR OTHERWISE, INCLUDING, WITHOUT LIMITATION, WARRANTIES OF TITLE, MERCHANTIBILITY, FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR THE ABSENCE OF LATENT OR OTHER DEFECTS, ACCURACY, OR THE PRESENCE OF ABSENCE OF ERRORS, WHETHER OR NOT DISCOVERABLE. SOME JURISDICTIONS DO NOT ALLOW THE EXCLUSION OF IMPLIED WARRANTIES, SO SUCH EXCLUSION MAY NOT APPLY TO YOU.\n");
	resultado += wxT("\n");
	resultado += wxT("6. Limitation on Liability. EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.\n");
	resultado += wxT("\n");
	resultado += wxT("7. Termination\n");
	resultado += wxT("\n");
	resultado += wxT("This License and the rights granted hereunder will terminate automatically upon any breach by You of the terms of this License. Individuals or entities who have received Derivative Works or Collective Works from You under this License, however, will not have their licenses terminated provided such individuals or entities remain in full compliance with those licenses. Sections 1, 2, 5, 6, 7, and 8 will survive any termination of this License.\n");
	resultado += wxT("Subject to the above terms and conditions, the license granted here is perpetual (for the duration of the applicable copyright in the Work). Notwithstanding the above, Licensor reserves the right to release the Work under different license terms or to stop distributing the Work at any time; provided, however that any such election will not serve to withdraw this License (or any other license that has been, or is required to be, granted under the terms of this License), and this License will continue in full force and effect unless terminated as stated above.\n");
	resultado += wxT("8. Miscellaneous\n");
	resultado += wxT("\n");
	resultado += wxT("Each time You distribute or publicly digitally perform the Work or a Collective Work, the Licensor offers to the recipient a license to the Work on the same terms and conditions as the license granted to You under this License.\n");
	resultado += wxT("Each time You distribute or publicly digitally perform a Derivative Work, Licensor offers to the recipient a license to the original Work on the same terms and conditions as the license granted to You under this License.\n");
	resultado += wxT("If any provision of this License is invalid or unenforceable under applicable law, it shall not affect the validity or enforceability of the remainder of the terms of this License, and without further action by the parties to this agreement, such provision shall be reformed to the minimum extent necessary to make such provision valid and enforceable.\n");
	resultado += wxT("No term or provision of this License shall be deemed waived and no breach consented to unless such waiver or consent shall be in writing and signed by the party to be charged with such waiver or consent.\n");
	resultado += wxT("This License constitutes the entire agreement between the parties with respect to the Work licensed here. There are no understandings, agreements or representations with respect to the Work not specified here. Licensor shall not be bound by any additional provisions that may appear in any communication from You. This License may not be modified without the mutual written agreement of the Licensor and You.\n");
	resultado += wxT("Creative Commons is not a party to this License, and makes no warranty whatsoever in connection with the Work. Creative Commons will not be liable to You or any party on any legal theory for any damages whatsoever, including without limitation any general, special, incidental or consequential damages arising in connection to this license. Notwithstanding the foregoing two (2) sentences, if Creative Commons has expressly identified itself as the Licensor hereunder, it shall have all rights and obligations of Licensor.\n");
	resultado += wxT("\n");
	resultado += wxT("Except for the limited purpose of indicating to the public that the Work is licensed under the CCPL, neither party will use the trademark \"Creative Commons\" or any related trademark or logo of Creative Commons without the prior written consent of Creative Commons. Any permitted use will be in compliance with Creative Commons' then-current trademark usage guidelines, as may be published on its website or otherwise made available upon request from time to time.\n");
	resultado += wxT("\n");
	resultado += wxT("Creative Commons may be contacted at http://creativecommons.org/.\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::EscribeLicenciaFugue()
{
	EscribeTitulo(wxT("Fugue icons\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("_________________________________________\n");
	resultado += wxT("Yusuke Kamiyamane\n");
	resultado += wxT("http://p.yusukekamiyamane.com/\n");
	resultado += wxT("_________________________________________\n");
	resultado += wxT("\n");
	resultado += wxT("CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF THIS LICENSE DOES NOT CREATE AN ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS INFORMATION ON AN \"AS-IS\" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES REGARDING THE INFORMATION PROVIDED, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM ITS USE. ");
	resultado += wxT("\n");
	resultado += wxT("License\n");
	resultado += wxT("\n");
	resultado += wxT("THE WORK (AS DEFINED BELOW) IS PROVIDED UNDER THE TERMS OF THIS CREATIVE COMMONS PUBLIC LICENSE (\"CCPL\" OR \"LICENSE\"). THE WORK IS PROTECTED BY COPYRIGHT AND/OR OTHER APPLICABLE LAW. ANY USE OF THE WORK OTHER THAN AS AUTHORIZED UNDER THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.\n");
	resultado += wxT("\n");
	resultado += wxT("BY EXERCISING ANY RIGHTS TO THE WORK PROVIDED HERE, YOU ACCEPT AND AGREE TO BE BOUND BY THE TERMS OF THIS LICENSE. TO THE EXTENT THIS LICENSE MAY BE CONSIDERED TO BE A CONTRACT, THE LICENSOR GRANTS YOU THE RIGHTS CONTAINED HERE IN CONSIDERATION OF YOUR ACCEPTANCE OF SUCH TERMS AND CONDITIONS.\n");
	resultado += wxT("\n");
	resultado += wxT("1. Definitions\n");
	resultado += wxT("\n");
	resultado += wxT("   1. \"Adaptation\" means a work based upon the Work, or upon the Work and other pre-existing works, such as a translation, adaptation, derivative work, arrangement of music or other alterations of a literary or artistic work, or phonogram or performance and includes cinematographic adaptations or any other form in which the Work may be recast, transformed, or adapted including in any form recognizably derived from the original, except that a work that constitutes a Collection will not be considered an Adaptation for the purpose of this License. For the avoidance of doubt, where the Work is a musical work, performance or phonogram, the synchronization of the Work in timed-relation with a moving image (\"synching\") will be considered an Adaptation for the purpose of this License.\n");
	resultado += wxT("   2. \"Collection\" means a collection of literary or artistic works, such as encyclopedias and anthologies, or performances, phonograms or broadcasts, or other works or subject matter other than works listed in Section 1(f) below, which, by reason of the selection and arrangement of their contents, constitute intellectual creations, in which the Work is included in its entirety in unmodified form along with one or more other contributions, each constituting separate and independent works in themselves, which together are assembled into a collective whole. A work that constitutes a Collection will not be considered an Adaptation (as defined above) for the purposes of this License.\n");
	resultado += wxT("   3. \"Distribute\" means to make available to the public the original and copies of the Work or Adaptation, as appropriate, through sale or other transfer of ownership.\n");
	resultado += wxT("   4. \"Licensor\" means the individual, individuals, entity or entities that offer(s) the Work under the terms of this License.\n");
	resultado += wxT("   5. \"Original Author\" means, in the case of a literary or artistic work, the individual, individuals, entity or entities who created the Work or if no individual or entity can be identified, the publisher; and in addition (i) in the case of a performance the actors, singers, musicians, dancers, and other persons who act, sing, deliver, declaim, play in, interpret or otherwise perform literary or artistic works or expressions of folklore; (ii) in the case of a phonogram the producer being the person or legal entity who first fixes the sounds of a performance or other sounds; and, (iii) in the case of broadcasts, the organization that transmits the broadcast.\n");
	resultado += wxT("   6. \"Work\" means the literary and/or artistic work offered under the terms of this License including without limitation any production in the literary, scientific and artistic domain, whatever may be the mode or form of its expression including digital form, such as a book, pamphlet and other writing; a lecture, address, sermon or other work of the same nature; a dramatic or dramatico-musical work; a choreographic work or entertainment in dumb show; a musical composition with or without words; a cinematographic work to which are assimilated works expressed by a process analogous to cinematography; a work of drawing, painting, architecture, sculpture, engraving or lithography; a photographic work to which are assimilated works expressed by a process analogous to photography; a work of applied art; an illustration, map, plan, sketch or three-dimensional work relative to geography, topography, architecture or science; a performance; a broadcast; a phonogram; a compilation of data to the extent it is protected as a copyrightable work; or a work performed by a variety or circus performer to the extent it is not otherwise considered a literary or artistic work.\n");
	resultado += wxT("   7. \"You\" means an individual or entity exercising rights under this License who has not previously violated the terms of this License with respect to the Work, or who has received express permission from the Licensor to exercise rights under this License despite a previous violation.\n");
	resultado += wxT("   8. \"Publicly Perform\" means to perform public recitations of the Work and to communicate to the public those public recitations, by any means or process, including by wire or wireless means or public digital performances; to make available to the public Works in such a way that members of the public may access these Works from a place and at a place individually chosen by them; to perform the Work to the public by any means or process and the communication to the public of the performances of the Work, including by public digital performance; to broadcast and rebroadcast the Work by any means including signs, sounds or images.\n");
	resultado += wxT("   9. \"Reproduce\" means to make copies of the Work by any means including without limitation by sound or visual recordings and the right of fixation and reproducing fixations of the Work, including storage of a protected performance or phonogram in digital form or other electronic medium.\n");
	resultado += wxT("\n");
	resultado += wxT("2. Fair Dealing Rights. Nothing in this License is intended to reduce, limit, or restrict any uses free from copyright or rights arising from limitations or exceptions that are provided for in connection with the copyright protection under copyright law or other applicable laws.\n");
	resultado += wxT("\n");
	resultado += wxT("3. License Grant. Subject to the terms and conditions of this License, Licensor hereby grants You a worldwide, royalty-free, non-exclusive, perpetual (for the duration of the applicable copyright) license to exercise the rights in the Work as stated below:\n");
	resultado += wxT("\n");
	resultado += wxT("   1. to Reproduce the Work, to incorporate the Work into one or more Collections, and to Reproduce the Work as incorporated in the Collections;\n");
	resultado += wxT("   2. to create and Reproduce Adaptations provided that any such Adaptation, including any translation in any medium, takes reasonable steps to clearly label, demarcate or otherwise identify that changes were made to the original Work. For example, a translation could be marked \"The original work was translated from English to Spanish,\" or a modification could indicate \"The original work has been modified.\";\n");
	resultado += wxT("   3. to Distribute and Publicly Perform the Work including as incorporated in Collections; and,\n");
	resultado += wxT("   4. to Distribute and Publicly Perform Adaptations.\n");
	resultado += wxT("   5.\n");
	resultado += wxT("\n");
	resultado += wxT("      For the avoidance of doubt:\n");
	resultado += wxT("         1. Non-waivable Compulsory License Schemes. In those jurisdictions in which the right to collect royalties through any statutory or compulsory licensing scheme cannot be waived, the Licensor reserves the exclusive right to collect such royalties for any exercise by You of the rights granted under this License;\n");
	resultado += wxT("         2. Waivable Compulsory License Schemes. In those jurisdictions in which the right to collect royalties through any statutory or compulsory licensing scheme can be waived, the Licensor waives the exclusive right to collect such royalties for any exercise by You of the rights granted under this License; and,\n");
	resultado += wxT("         3. Voluntary License Schemes. The Licensor waives the right to collect royalties, whether individually or, in the event that the Licensor is a member of a collecting society that administers voluntary licensing schemes, via that society, from any exercise by You of the rights granted under this License.\n");
	resultado += wxT("\n");
	resultado += wxT("The above rights may be exercised in all media and formats whether now known or hereafter devised. The above rights include the right to make such modifications as are technically necessary to exercise the rights in other media and formats. Subject to Section 8(f), all rights not expressly granted by Licensor are hereby reserved.\n");
	resultado += wxT("\n");
	resultado += wxT("4. Restrictions. The license granted in Section 3 above is expressly made subject to and limited by the following restrictions:\n");
	resultado += wxT("\n");
	resultado += wxT("   1. You may Distribute or Publicly Perform the Work only under the terms of this License. You must include a copy of, or the Uniform Resource Identifier (URI) for, this License with every copy of the Work You Distribute or Publicly Perform. You may not offer or impose any terms on the Work that restrict the terms of this License or the ability of the recipient of the Work to exercise the rights granted to that recipient under the terms of the License. You may not sublicense the Work. You must keep intact all notices that refer to this License and to the disclaimer of warranties with every copy of the Work You Distribute or Publicly Perform. When You Distribute or Publicly Perform the Work, You may not impose any effective technological measures on the Work that restrict the ability of a recipient of the Work from You to exercise the rights granted to that recipient under the terms of the License. This Section 4(a) applies to the Work as incorporated in a Collection, but this does not require the Collection apart from the Work itself to be made subject to the terms of this License. If You create a Collection, upon notice from any Licensor You must, to the extent practicable, remove from the Collection any credit as required by Section 4(b), as requested. If You create an Adaptation, upon notice from any Licensor You must, to the extent practicable, remove from the Adaptation any credit as required by Section 4(b), as requested.\n");
	resultado += wxT("   2. If You Distribute, or Publicly Perform the Work or any Adaptations or Collections, You must, unless a request has been made pursuant to Section 4(a), keep intact all copyright notices for the Work and provide, reasonable to the medium or means You are utilizing: (i) the name of the Original Author (or pseudonym, if applicable) if supplied, and/or if the Original Author and/or Licensor designate another party or parties (e.g., a sponsor institute, publishing entity, journal) for attribution (\"Attribution Parties\") in Licensor's copyright notice, terms of service or by other reasonable means, the name of such party or parties; (ii) the title of the Work if supplied; (iii) to the extent reasonably practicable, the URI, if any, that Licensor specifies to be associated with the Work, unless such URI does not refer to the copyright notice or licensing information for the Work; and (iv) , consistent with Section 3(b), in the case of an Adaptation, a credit identifying the use of the Work in the Adaptation (e.g., \"French translation of the Work by Original Author,\" or \"Screenplay based on original Work by Original Author\"). The credit required by this Section 4 (b) may be implemented in any reasonable manner; provided, however, that in the case of a Adaptation or Collection, at a minimum such credit will appear, if a credit for all contributing authors of the Adaptation or Collection appears, then as part of these credits and in a manner at least as prominent as the credits for the other contributing authors. For the avoidance of doubt, You may only use the credit required by this Section for the purpose of attribution in the manner set out above and, by exercising Your rights under this License, You may not implicitly or explicitly assert or imply any connection with, sponsorship or endorsement by the Original Author, Licensor and/or Attribution Parties, as appropriate, of You or Your use of the Work, without the separate, express prior written permission of the Original Author, Licensor and/or Attribution Parties.\n");
	resultado += wxT("   3. Except as otherwise agreed in writing by the Licensor or as may be otherwise permitted by applicable law, if You Reproduce, Distribute or Publicly Perform the Work either by itself or as part of any Adaptations or Collections, You must not distort, mutilate, modify or take other derogatory action in relation to the Work which would be prejudicial to the Original Author's honor or reputation. Licensor agrees that in those jurisdictions (e.g. Japan), in which any exercise of the right granted in Section 3(b) of this License (the right to make Adaptations) would be deemed to be a distortion, mutilation, modification or other derogatory action prejudicial to the Original Author's honor and reputation, the Licensor will waive or not assert, as appropriate, this Section, to the fullest extent permitted by the applicable national law, to enable You to reasonably exercise Your right under Section 3(b) of this License (right to make Adaptations) but not otherwise.\n");
	resultado += wxT("\n");
	resultado += wxT("5. Representations, Warranties and Disclaimer\n");
	resultado += wxT("\n");
	resultado += wxT("UNLESS OTHERWISE MUTUALLY AGREED TO BY THE PARTIES IN WRITING, LICENSOR OFFERS THE WORK AS-IS AND MAKES NO REPRESENTATIONS OR WARRANTIES OF ANY KIND CONCERNING THE WORK, EXPRESS, IMPLIED, STATUTORY OR OTHERWISE, INCLUDING, WITHOUT LIMITATION, WARRANTIES OF TITLE, MERCHANTIBILITY, FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR THE ABSENCE OF LATENT OR OTHER DEFECTS, ACCURACY, OR THE PRESENCE OF ABSENCE OF ERRORS, WHETHER OR NOT DISCOVERABLE. SOME JURISDICTIONS DO NOT ALLOW THE EXCLUSION OF IMPLIED WARRANTIES, SO SUCH EXCLUSION MAY NOT APPLY TO YOU.\n");
	resultado += wxT("\n");
	resultado += wxT("6. Limitation on Liability. EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.\n");
	resultado += wxT("\n");
	resultado += wxT("7. Termination\n");
	resultado += wxT("\n");
	resultado += wxT("   1. This License and the rights granted hereunder will terminate automatically upon any breach by You of the terms of this License. Individuals or entities who have received Adaptations or Collections from You under this License, however, will not have their licenses terminated provided such individuals or entities remain in full compliance with those licenses. Sections 1, 2, 5, 6, 7, and 8 will survive any termination of this License.\n");
	resultado += wxT("   2. Subject to the above terms and conditions, the license granted here is perpetual (for the duration of the applicable copyright in the Work). Notwithstanding the above, Licensor reserves the right to release the Work under different license terms or to stop distributing the Work at any time; provided, however that any such election will not serve to withdraw this License (or any other license that has been, or is required to be, granted under the terms of this License), and this License will continue in full force and effect unless terminated as stated above.\n");
	resultado += wxT("\n");
	resultado += wxT("8. Miscellaneous\n");
	resultado += wxT("\n");
	resultado += wxT("   1. Each time You Distribute or Publicly Perform the Work or a Collection, the Licensor offers to the recipient a license to the Work on the same terms and conditions as the license granted to You under this License.\n");
	resultado += wxT("   2. Each time You Distribute or Publicly Perform an Adaptation, Licensor offers to the recipient a license to the original Work on the same terms and conditions as the license granted to You under this License.\n");
	resultado += wxT("   3. If any provision of this License is invalid or unenforceable under applicable law, it shall not affect the validity or enforceability of the remainder of the terms of this License, and without further action by the parties to this agreement, such provision shall be reformed to the minimum extent necessary to make such provision valid and enforceable.\n");
	resultado += wxT("   4. No term or provision of this License shall be deemed waived and no breach consented to unless such waiver or consent shall be in writing and signed by the party to be charged with such waiver or consent.\n");
	resultado += wxT("   5. This License constitutes the entire agreement between the parties with respect to the Work licensed here. There are no understandings, agreements or representations with respect to the Work not specified here. Licensor shall not be bound by any additional provisions that may appear in any communication from You. This License may not be modified without the mutual written agreement of the Licensor and You.\n");
	resultado += wxT("   6. The rights granted under, and the subject matter referenced, in this License were drafted utilizing the terminology of the Berne Convention for the Protection of Literary and Artistic Works (as amended on September 28, 1979), the Rome Convention of 1961, the WIPO Copyright Treaty of 1996, the WIPO Performances and Phonograms Treaty of 1996 and the Universal Copyright Convention (as revised on July 24, 1971). These rights and subject matter take effect in the relevant jurisdiction in which the License terms are sought to be enforced according to the corresponding provisions of the implementation of those treaty provisions in the applicable national law. If the standard suite of rights granted under applicable copyright law includes additional rights not granted under this License, such additional rights are deemed to be included in the License; this License is not intended to restrict the license of any rights under applicable law.\n");
	resultado += wxT("\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::EscribeLicenciaITK()
{
	EscribeTitulo(wxT("ITK\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("The license of the Insight Toolkit is an open-source BSD license. It allows unrestricted use, including use in commercial products. (The only exceptions are software modules found in the patented software directory.)\n");
	resultado += wxT("\n");
	resultado += wxT("Starting with ITK 3.6, The Insight Toolkit is distributed under the new and simplified BSD license approved by the Open Source Initiative(OSI). The complete license is shown below.\n");
	resultado += wxT("/*=========================================================================\n");
	resultado += wxT("Copyright (c) 1999-2008 Insight Software Consortium All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n");
	resultado += wxT("* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n");
	resultado += wxT("* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n");
	resultado += wxT("* Neither the name of the Insight Software Consortium nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.\n");
	resultado += wxT("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. ");
	resultado += wxT("IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, ");
	resultado += wxT("OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");
	resultado += wxT("=========================================================================*\n");
	resultado += wxT("\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::EscribeLicencialibjpeg()
{
	EscribeTitulo(wxT("LibJpeg\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("In plain English:\n");
	resultado += wxT("\n");
	resultado += wxT("1. We don't promise that this software works.  (But if you find any bugs,\n");
	resultado += wxT("   please let us know!)\n");
	resultado += wxT("2. You can use this software for whatever you want.  You don't have to pay us.\n");
	resultado += wxT("3. You may not pretend that you wrote this software.  If you use it in a\n");
	resultado += wxT("   program, you must acknowledge somewhere in your documentation that\n");
	resultado += wxT("   you've used the IJG code.\n");
	resultado += wxT("\n");
	resultado += wxT("In legalese:\n");
	resultado += wxT("\n");
	resultado += wxT("The authors make NO WARRANTY or representation, either express or implied,\n");
	resultado += wxT("with respect to this software, its quality, accuracy, merchantability, or\n");
	resultado += wxT("fitness for a particular purpose.  This software is provided \"AS IS\", and you,\n");
	resultado += wxT("its user, assume the entire risk as to its quality and accuracy.\n");
	resultado += wxT("\n");
	resultado += wxT("This software is copyright (C) 1991-2009, Thomas G. Lane, Guido Vollbeding.\n");
	resultado += wxT("All Rights Reserved except as specified below.\n");
	resultado += wxT("\n");
	resultado += wxT("Permission is hereby granted to use, copy, modify, and distribute this\n");
	resultado += wxT("software (or portions thereof) for any purpose, without fee, subject to these\n");
	resultado += wxT("conditions:\n");
	resultado += wxT("(1) If any part of the source code for this software is distributed, then this\n");
	resultado += wxT("README file must be included, with this copyright and no-warranty notice\n");
	resultado += wxT("unaltered; and any additions, deletions, or changes to the original files\n");
	resultado += wxT("must be clearly indicated in accompanying documentation.\n");
	resultado += wxT("(2) If only executable code is distributed, then the accompanying\n");
	resultado += wxT("documentation must state that \"this software is based in part on the work of\n");
	resultado += wxT("the Independent JPEG Group\".\n");
	resultado += wxT("(3) Permission for use of this software is granted only if the user accepts\n");
	resultado += wxT("full responsibility for any undesirable consequences; the authors accept\n");
	resultado += wxT("NO LIABILITY for damages of any kind.\n");
	resultado += wxT("\n");
	resultado += wxT("These conditions apply to any software derived from or based on the IJG code,\n");
	resultado += wxT("not just to the unmodified library.  If you use our work, you ought to\n");
	resultado += wxT("acknowledge us.\n");
	resultado += wxT("\n");
	resultado += wxT("Permission is NOT granted for the use of any IJG author's name or company name\n");
	resultado += wxT("in advertising or publicity relating to this software or products derived from\n");
	resultado += wxT("it.  This software may be referred to only as \"the Independent JPEG Group's\n");
	resultado += wxT("software\".\n");
	resultado += wxT("\n");
	resultado += wxT("We specifically permit and encourage the use of this software as the basis of\n");
	resultado += wxT("commercial products, provided that all warranty or liability claims are\n");
	resultado += wxT("assumed by the product vendor.\n");
	resultado += wxT("\n");
	resultado += wxT("\n");
	resultado += wxT("ansi2knr.c is included in this distribution by permission of L. Peter Deutsch,\n");
	resultado += wxT("sole proprietor of its copyright holder, Aladdin Enterprises of Menlo Park, CA.\n");
	resultado += wxT("ansi2knr.c is NOT covered by the above copyright and conditions, but instead\n");
	resultado += wxT("by the usual distribution terms of the Free Software Foundation; principally,\n");
	resultado += wxT("that you must include source code if you redistribute it.  (See the file\n");
	resultado += wxT("ansi2knr.c for full details.)  However, since ansi2knr.c is not needed as part\n");
	resultado += wxT("of any program generated from the IJG code, this does not limit you more than\n");
	resultado += wxT("the foregoing paragraphs do.\n");
	resultado += wxT("\n");
	resultado += wxT("The Unix configuration script \"configure\" was produced with GNU Autoconf.\n");
	resultado += wxT("It is copyright by the Free Software Foundation but is freely distributable.\n");
	resultado += wxT("The same holds for its supporting scripts (config.guess, config.sub,\n");
	resultado += wxT("ltmain.sh).  Another support script, install-sh, is copyright by X Consortium\n");
	resultado += wxT("but is also freely distributable.\n");
	resultado += wxT("\n");
	resultado += wxT("The IJG distribution formerly included code to read and write GIF files.\n");
	resultado += wxT("To avoid entanglement with the Unisys LZW patent, GIF reading support has\n");
	resultado += wxT("been removed altogether, and the GIF writer has been simplified to produce\n");
	resultado += wxT("\"uncompressed GIFs\".  This technique does not use the LZW algorithm; the\n");
	resultado += wxT("resulting GIF files are larger than usual, but are readable by all standard\n");
	resultado += wxT("GIF decoders.\n");
	resultado += wxT("\n");
	resultado += wxT("We are required to state that\n");
	resultado += wxT("    \"The Graphics Interchange Format(c) is the Copyright property of\n");
	resultado += wxT("    CompuServe Incorporated.  GIF(sm) is a Service Mark property of\n");
	resultado += wxT("    CompuServe Incorporated.\"\n");
	resultado += wxT("\n");
	resultado += wxT("\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::EscribeLicencialibpng()
{
	EscribeTitulo(wxT("Libpng\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("This copy of the libpng notices is provided for your convenience.  In case of\n");
	resultado += wxT("any discrepancy between this copy and the notices in the file png.h that is\n");
	resultado += wxT("included in the libpng distribution, the latter shall prevail.\n");
	resultado += wxT("\n");
	resultado += wxT("COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:\n");
	resultado += wxT("\n");
	resultado += wxT("If you modify libpng you may insert additional notices immediately following\n");
	resultado += wxT("this sentence.\n");
	resultado += wxT("\n");
	resultado += wxT("This code is released under the libpng license.\n");
	resultado += wxT("\n");
	resultado += wxT("libpng versions 1.2.6, August 15, 2004, through 1.2.40, September 10, 2009, are\n");
	resultado += wxT("Copyright (c) 2004, 2006-2009 Glenn Randers-Pehrson, and are\n");
	resultado += wxT("distributed according to the same disclaimer and license as libpng-1.2.5\n");
	resultado += wxT("with the following individual added to the list of Contributing Authors\n");
	resultado += wxT("\n");
	resultado += wxT("   Cosmin Truta\n");
	resultado += wxT("\n");
	resultado += wxT("libpng versions 1.0.7, July 1, 2000, through 1.2.5 - October 3, 2002, are\n");
	resultado += wxT("Copyright (c) 2000-2002 Glenn Randers-Pehrson, and are\n");
	resultado += wxT("distributed according to the same disclaimer and license as libpng-1.0.6\n");
	resultado += wxT("with the following individuals added to the list of Contributing Authors\n");
	resultado += wxT("\n");
	resultado += wxT("   Simon-Pierre Cadieux\n");
	resultado += wxT("   Eric S. Raymond\n");
	resultado += wxT("   Gilles Vollant\n");
	resultado += wxT("\n");
	resultado += wxT("and with the following additions to the disclaimer:\n");
	resultado += wxT("\n");
	resultado += wxT("   There is no warranty against interference with your enjoyment of the\n");
	resultado += wxT("   library or against infringement.  There is no warranty that our\n");
	resultado += wxT("   efforts or the library will fulfill any of your particular purposes\n");
	resultado += wxT("   or needs.  This library is provided with all faults, and the entire\n");
	resultado += wxT("   risk of satisfactory quality, performance, accuracy, and effort is with\n");
	resultado += wxT("   the user.\n");
	resultado += wxT("\n");
	resultado += wxT("libpng versions 0.97, January 1998, through 1.0.6, March 20, 2000, are\n");
	resultado += wxT("Copyright (c) 1998, 1999 Glenn Randers-Pehrson, and are\n");
	resultado += wxT("distributed according to the same disclaimer and license as libpng-0.96,\n");
	resultado += wxT("with the following individuals added to the list of Contributing Authors:\n");
	resultado += wxT("\n");
	resultado += wxT("   Tom Lane\n");
	resultado += wxT("   Glenn Randers-Pehrson\n");
	resultado += wxT("   Willem van Schaik\n");
	resultado += wxT("\n");
	resultado += wxT("libpng versions 0.89, June 1996, through 0.96, May 1997, are\n");
	resultado += wxT("Copyright (c) 1996, 1997 Andreas Dilger\n");
	resultado += wxT("Distributed according to the same disclaimer and license as libpng-0.88,\n");
	resultado += wxT("with the following individuals added to the list of Contributing Authors:\n");
	resultado += wxT("\n");
	resultado += wxT("   John Bowler\n");
	resultado += wxT("   Kevin Bracey\n");
	resultado += wxT("   Sam Bushell\n");
	resultado += wxT("   Magnus Holmgren\n");
	resultado += wxT("   Greg Roelofs\n");
	resultado += wxT("   Tom Tanner\n");
	resultado += wxT("\n");
	resultado += wxT("libpng versions 0.5, May 1995, through 0.88, January 1996, are\n");
	resultado += wxT("Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.\n");
	resultado += wxT("\n");
	resultado += wxT("For the purposes of this copyright and license, \"Contributing Authors\"\n");
	resultado += wxT("is defined as the following set of individuals:\n");
	resultado += wxT("\n");
	resultado += wxT("   Andreas Dilger\n");
	resultado += wxT("   Dave Martindale\n");
	resultado += wxT("   Guy Eric Schalnat\n");
	resultado += wxT("   Paul Schmidt\n");
	resultado += wxT("   Tim Wegner\n");
	resultado += wxT("\n");
	resultado += wxT("The PNG Reference Library is supplied \"AS IS\".  The Contributing Authors\n");
	resultado += wxT("and Group 42, Inc. disclaim all warranties, expressed or implied,\n");
	resultado += wxT("including, without limitation, the warranties of merchantability and of\n");
	resultado += wxT("fitness for any purpose.  The Contributing Authors and Group 42, Inc.\n");
	resultado += wxT("assume no liability for direct, indirect, incidental, special, exemplary,\n");
	resultado += wxT("or consequential damages, which may result from the use of the PNG\n");
	resultado += wxT("Reference Library, even if advised of the possibility of such damage.\n");
	resultado += wxT("\n");
	resultado += wxT("Permission is hereby granted to use, copy, modify, and distribute this\n");
	resultado += wxT("source code, or portions hereof, for any purpose, without fee, subject\n");
	resultado += wxT("to the following restrictions:\n");
	resultado += wxT("\n");
	resultado += wxT("1. The origin of this source code must not be misrepresented.\n");
	resultado += wxT("\n");
	resultado += wxT("2. Altered versions must be plainly marked as such and must not\n");
	resultado += wxT("   be misrepresented as being the original source.\n");
	resultado += wxT("\n");
	resultado += wxT("3. This Copyright notice may not be removed or altered from any\n");
	resultado += wxT("   source or altered source distribution.\n");
	resultado += wxT("\n");
	resultado += wxT("The Contributing Authors and Group 42, Inc. specifically permit, without\n");
	resultado += wxT("fee, and encourage the use of this source code as a component to\n");
	resultado += wxT("supporting the PNG file format in commercial products.  If you use this\n");
	resultado += wxT("source code in a product, acknowledgment is not required but would be\n");
	resultado += wxT("appreciated.\n");
	resultado += wxT("\n");
	resultado += wxT("\n");
	resultado += wxT("A \"png_get_copyright\" function is available, for convenient use in \"about\"\n");
	resultado += wxT("boxes and the like:\n");
	resultado += wxT("\n");
	resultado += wxT("   printf(\"%s\",png_get_copyright(NULL));\n");
	resultado += wxT("\n");
	resultado += wxT("Also, the PNG logo (in PNG format, of course) is supplied in the\n");
	resultado += wxT("files \"pngbar.png\" and \"pngbar.jpg (88x31) and \"pngnow.png\" (98x31).\n");
	resultado += wxT("\n");
	resultado += wxT("Libpng is OSI Certified Open Source Software.  OSI Certified Open Source is a\n");
	resultado += wxT("certification mark of the Open Source Initiative.\n");
	resultado += wxT("\n");
	resultado += wxT("Glenn Randers-Pehrson\n");
	resultado += wxT("glennrp at users.sourceforge.net\n");
	resultado += wxT("September 10, 2009\n");
	resultado += wxT("\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::EscribeLicenciaOpenssl()
{
	EscribeTitulo(wxT("OpenSSL\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("/* ====================================================================\n");
	resultado += wxT(" * Copyright (c) 1998-2008 The OpenSSL Project.  All rights reserved.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" * Redistribution and use in source and binary forms, with or without\n");
	resultado += wxT(" * modification, are permitted provided that the following conditions\n");
	resultado += wxT(" * are met:\n");
	resultado += wxT(" *\n");
	resultado += wxT(" * 1. Redistributions of source code must retain the above copyright\n");
	resultado += wxT(" *    notice, this list of conditions and the following disclaimer. \n");
	resultado += wxT(" *\n");
	resultado += wxT(" * 2. Redistributions in binary form must reproduce the above copyright\n");
	resultado += wxT(" *    notice, this list of conditions and the following disclaimer in\n");
	resultado += wxT(" *    the documentation and/or other materials provided with the\n");
	resultado += wxT(" *    distribution.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" * 3. All advertising materials mentioning features or use of this\n");
	resultado += wxT(" *    software must display the following acknowledgment:\n");
	resultado += wxT(" *    \"This product includes software developed by the OpenSSL Project\n");
	resultado += wxT(" *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)\"\n");
	resultado += wxT(" *\n");
	resultado += wxT(" * 4. The names \"OpenSSL Toolkit\" and \"OpenSSL Project\" must not be used to\n");
	resultado += wxT(" *    endorse or promote products derived from this software without\n");
	resultado += wxT(" *    prior written permission. For written permission, please contact\n");
	resultado += wxT(" *    openssl-core@openssl.org.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" * 5. Products derived from this software may not be called \"OpenSSL\"\n");
	resultado += wxT(" *    nor may \"OpenSSL\" appear in their names without prior written\n");
	resultado += wxT(" *    permission of the OpenSSL Project.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" * 6. Redistributions of any form whatsoever must retain the following\n");
	resultado += wxT(" *    acknowledgment:\n");
	resultado += wxT(" *    \"This product includes software developed by the OpenSSL Project\n");
	resultado += wxT(" *    for use in the OpenSSL Toolkit (http://www.openssl.org/)\"\n");
	resultado += wxT(" *\n");
	resultado += wxT(" * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY\n");
	resultado += wxT(" * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n");
	resultado += wxT(" * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n");
	resultado += wxT(" * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR\n");
	resultado += wxT(" * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,\n");
	resultado += wxT(" * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT\n");
	resultado += wxT(" * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n");
	resultado += wxT(" * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)\n");
	resultado += wxT(" * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,\n");
	resultado += wxT(" * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n");
	resultado += wxT(" * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED\n");
	resultado += wxT(" * OF THE POSSIBILITY OF SUCH DAMAGE.\n");
	resultado += wxT(" * ====================================================================\n");
	resultado += wxT(" *\n");
	resultado += wxT(" * This product includes cryptographic software written by Eric Young\n");
	resultado += wxT(" * (eay@cryptsoft.com).  This product includes software written by Tim\n");
	resultado += wxT(" * Hudson (tjh@cryptsoft.com).\n");
	resultado += wxT(" *\n");
	resultado += wxT(" */\n");
	resultado += wxT("\n");
	resultado += wxT(" Original SSLeay License\n");
	resultado += wxT(" -----------------------\n");
	resultado += wxT("\n");
	resultado += wxT("/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)\n");
	resultado += wxT(" * All rights reserved.\n");
	resultado += wxT(" *\n");
	resultado += wxT(" * This package is an SSL implementation written\n");
	resultado += wxT(" * by Eric Young (eay@cryptsoft.com).\n");
	resultado += wxT(" * The implementation was written so as to conform with Netscapes SSL.\n");
	resultado += wxT(" * \n");
	resultado += wxT(" * This library is free for commercial and non-commercial use as long as\n");
	resultado += wxT(" * the following conditions are aheared to.  The following conditions\n");
	resultado += wxT(" * apply to all code found in this distribution, be it the RC4, RSA,\n");
	resultado += wxT(" * lhash, DES, etc., code; not just the SSL code.  The SSL documentation\n");
	resultado += wxT(" * included with this distribution is covered by the same copyright terms\n");
	resultado += wxT(" * except that the holder is Tim Hudson (tjh@cryptsoft.com).\n");
	resultado += wxT(" * \n");
	resultado += wxT(" * Copyright remains Eric Young's, and as such any Copyright notices in\n");
	resultado += wxT(" * the code are not to be removed.\n");
	resultado += wxT(" * If this package is used in a product, Eric Young should be given attribution\n");
	resultado += wxT(" * as the author of the parts of the library used.\n");
	resultado += wxT(" * This can be in the form of a textual message at program startup or\n");
	resultado += wxT(" * in documentation (online or textual) provided with the package.\n");
	resultado += wxT(" * \n");
	resultado += wxT(" * Redistribution and use in source and binary forms, with or without\n");
	resultado += wxT(" * modification, are permitted provided that the following conditions\n");
	resultado += wxT(" * are met:\n");
	resultado += wxT(" * 1. Redistributions of source code must retain the copyright\n");
	resultado += wxT(" *    notice, this list of conditions and the following disclaimer.\n");
	resultado += wxT(" * 2. Redistributions in binary form must reproduce the above copyright\n");
	resultado += wxT(" *    notice, this list of conditions and the following disclaimer in the\n");
	resultado += wxT(" *    documentation and/or other materials provided with the distribution.\n");
	resultado += wxT(" * 3. All advertising materials mentioning features or use of this software\n");
	resultado += wxT(" *    must display the following acknowledgement:\n");
	resultado += wxT(" *    \"This product includes cryptographic software written by\n");
	resultado += wxT(" *     Eric Young (eay@cryptsoft.com)\"\n");
	resultado += wxT(" *    The word 'cryptographic' can be left out if the rouines from the library\n");
	resultado += wxT(" *    being used are not cryptographic related :-).\n");
	resultado += wxT(" * 4. If you include any Windows specific code (or a derivative thereof) from \n");
	resultado += wxT(" *    the apps directory (application code) you must include an acknowledgement:\n");
	resultado += wxT(" *    \"This product includes software written by Tim Hudson (tjh@cryptsoft.com)\"\n");
	resultado += wxT(" * \n");
	resultado += wxT(" * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND\n");
	resultado += wxT(" * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n");
	resultado += wxT(" * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n");
	resultado += wxT(" * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE\n");
	resultado += wxT(" * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n");
	resultado += wxT(" * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS\n");
	resultado += wxT(" * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)\n");
	resultado += wxT(" * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT\n");
	resultado += wxT(" * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY\n");
	resultado += wxT(" * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF\n");
	resultado += wxT(" * SUCH DAMAGE.\n");
	resultado += wxT(" * \n");
	resultado += wxT(" * The licence and distribution terms for any publically available version or\n");
	resultado += wxT(" * derivative of this code cannot be changed.  i.e. this code cannot simply be\n");
	resultado += wxT(" * copied and put under another distribution licence\n");
	resultado += wxT(" * [including the GNU Public Licence.]\n");
	resultado += wxT(" */\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::EscribeLicenciaVTK()
{
	EscribeTitulo(wxT("VTK\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("VTK is an open-source toolkit licensed under the BSD license.\n");
	resultado += wxT("Copyright (c) 1993-2008 Ken Martin, Will Schroeder, Bill Lorensen\n");
	resultado += wxT("All rights reserved.\n");
	resultado += wxT("Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n");
	resultado += wxT("Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n");
	resultado += wxT("Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n");
	resultado += wxT("Neither name of Ken Martin, Will Schroeder, or Bill Lorensen nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.\n");
	resultado += wxT("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");
	resultado += wxT("\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::EscribeLicenciawxWidgets()
{
	EscribeTitulo(wxT("wxWidgets\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("                wxWindows Library Licence, Version 3.1\n");
	resultado += wxT("                ======================================\n");
	resultado += wxT("\n");
	resultado += wxT("  Copyright (C) 1998-2005 Julian Smart, Robert Roebling et al\n");
	resultado += wxT("\n");
	resultado += wxT("  Everyone is permitted to copy and distribute verbatim copies\n");
	resultado += wxT("  of this licence document, but changing it is not allowed.\n");
	resultado += wxT("\n");
	resultado += wxT("                       WXWINDOWS LIBRARY LICENCE\n");
	resultado += wxT("     TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n");
	resultado += wxT("  \n");
	resultado += wxT("  This library is free software; you can redistribute it and/or modify it\n");
	resultado += wxT("  under the terms of the GNU Library General Public Licence as published by\n");
	resultado += wxT("  the Free Software Foundation; either version 2 of the Licence, or (at\n");
	resultado += wxT("  your option) any later version.\n");
	resultado += wxT("  \n");
	resultado += wxT("  This library is distributed in the hope that it will be useful, but\n");
	resultado += wxT("  WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	resultado += wxT("  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library\n");
	resultado += wxT("  General Public Licence for more details.\n");
	resultado += wxT("\n");
	resultado += wxT("  You should have received a copy of the GNU Library General Public Licence\n");
	resultado += wxT("  along with this software, usually in a file named COPYING.LIB.  If not,\n");
	resultado += wxT("  write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,\n");
	resultado += wxT("  Boston, MA 02111-1307 USA.\n");
	resultado += wxT("\n");
	resultado += wxT("  EXCEPTION NOTICE\n");
	resultado += wxT("\n");
	resultado += wxT("  1. As a special exception, the copyright holders of this library give\n");
	resultado += wxT("  permission for additional uses of the text contained in this release of\n");
	resultado += wxT("  the library as licenced under the wxWindows Library Licence, applying\n");
	resultado += wxT("  either version 3.1 of the Licence, or (at your option) any later version of\n");
	resultado += wxT("  the Licence as published by the copyright holders of version\n");
	resultado += wxT("  3.1 of the Licence document.\n");
	resultado += wxT("\n");
	resultado += wxT("  2. The exception is that you may use, copy, link, modify and distribute\n");
	resultado += wxT("  under your own terms, binary object code versions of works based\n");
	resultado += wxT("  on the Library.\n");
	resultado += wxT("\n");
	resultado += wxT("  3. If you copy code from files distributed under the terms of the GNU\n");
	resultado += wxT("  General Public Licence or the GNU Library General Public Licence into a\n");
	resultado += wxT("  copy of this library, as this licence permits, the exception does not\n");
	resultado += wxT("  apply to the code that you add in this way.  To avoid misleading anyone as\n");
	resultado += wxT("  to the status of such modified files, you must delete this exception\n");
	resultado += wxT("  notice from such code and/or adjust the licensing conditions notice\n");
	resultado += wxT("  accordingly.\n");
	resultado += wxT("\n");
	resultado += wxT("  4. If you write modifications of your own for this library, it is your\n");
	resultado += wxT("  choice whether to permit this exception to apply to your modifications. \n");
	resultado += wxT("  If you do not wish that, you must delete the exception notice from such\n");
	resultado += wxT("  code and/or adjust the licensing conditions notice accordingly.\n");
	resultado += wxT("\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::EscribeLicenciazlib()
{
	EscribeTitulo(wxT("ZLib\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("/* zlib.h -- interface of the 'zlib' general purpose compression library\n");
	resultado += wxT("  version 1.2.2, October 3rd, 2004\n");
	resultado += wxT("\n");
	resultado += wxT("  Copyright (C) 1995-2004 Jean-loup Gailly and Mark Adler\n");
	resultado += wxT("\n");
	resultado += wxT("  This software is provided 'as-is', without any express or implied\n");
	resultado += wxT("  warranty.  In no event will the authors be held liable for any damages\n");
	resultado += wxT("  arising from the use of this software.\n");
	resultado += wxT("\n");
	resultado += wxT("  Permission is granted to anyone to use this software for any purpose,\n");
	resultado += wxT("  including commercial applications, and to alter it and redistribute it\n");
	resultado += wxT("  freely, subject to the following restrictions:\n");
	resultado += wxT("\n");
	resultado += wxT("  1. The origin of this software must not be misrepresented; you must not\n");
	resultado += wxT("     claim that you wrote the original software. If you use this software\n");
	resultado += wxT("     in a product, an acknowledgment in the product documentation would be\n");
	resultado += wxT("     appreciated but is not required.\n");
	resultado += wxT("  2. Altered source versions must be plainly marked as such, and must not be\n");
	resultado += wxT("     misrepresented as being the original software.\n");
	resultado += wxT("  3. This notice may not be removed or altered from any source distribution.\n");
	resultado += wxT("\n");
	resultado += wxT("  Jean-loup Gailly jloup@gzip.org\n");
	resultado += wxT("  Mark Adler madler@alumni.caltech.edu\n");
	resultado += wxT("\n");
	resultado += wxT("*\n");
	EscribeLicencia(resultado);

}

void DialogoLicenciasOpenSource::EscribeLicenciaSQLLite()
{
	EscribeTitulo(wxT("SQL Lite Copyright\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("All of the deliverable code in SQLite has been dedicated to the public domain by the authors. All code authors, and representatives of the companies they work for, have signed affidavits dedicating their contributions to the public domain and originals of those signed affidavits are stored in a firesafe at the main offices of Hwaci. Anyone is free to copy, modify, publish, use, compile, sell, or distribute the original SQLite code, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.\n");
	resultado += wxT("\n");
	resultado += wxT("The previous paragraph applies to the deliverable code in SQLite - those parts of the SQLite library that you actually bundle and ship with a larger application. Portions of the documentation and some code used as part of the build process might fall under other licenses. The details here are unclear. We do not worry about the licensing of the documentation and build code so much because none of these things are part of the core deliverable SQLite library.\n");
	resultado += wxT("\n");
	resultado += wxT("All of the deliverable code in SQLite has been written from scratch. No code has been taken from other projects or from the open internet. Every line of code can be traced back to its original author, and all of those authors have public domain dedications on file. So the SQLite code base is clean and is uncontaminated with licensed code from other projects. \n");

	EscribeLicencia(resultado);
}


void DialogoLicenciasOpenSource::EscribeLicenciaCharLS()
{
	EscribeTitulo(wxT("CharLS\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado +=wxT("Copyright (c) 2007-2009, Jan de Vaan\n");
	resultado +=wxT("All rights reserved.\n");
	resultado +=wxT("\n");
	resultado +=wxT("Redistribution and use in source and binary forms, with or without\n");
	resultado +=wxT("modification, are permitted provided that the following conditions are met:\n");
	resultado +=wxT("\n");
	resultado +=wxT("* Redistributions of source code must retain the above copyright notice, this\n");
	resultado +=wxT("  list of conditions and the following disclaimer.\n");
	resultado +=wxT("\n");
	resultado +=wxT("* Redistributions in binary form must reproduce the above copyright notice,\n");
	resultado +=wxT("  this list of conditions and the following disclaimer in the documentation\n");
	resultado +=wxT("  and/or other materials provided with the distribution.\n");
	resultado +=wxT("\n");
	resultado +=wxT("* Neither the name of my employer, nor the names of its contributors may be\n");
	resultado +=wxT("  used to endorse or promote products derived from this software without\n");
	resultado +=wxT("  specific prior written permission.\n");
	resultado +=wxT("\n");
	resultado +=wxT("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n");
	resultado +=wxT("AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n");
	resultado +=wxT("IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n");
	resultado +=wxT("DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR\n");
	resultado +=wxT("ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n");
	resultado +=wxT("(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n");
	resultado +=wxT("LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON\n");
	resultado +=wxT("ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n");
	resultado +=wxT("(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n");
	resultado +=wxT("SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");

	EscribeLicencia(resultado);
}


void DialogoLicenciasOpenSource::WriteDarkGlassLicense()
{
	EscribeTitulo(wxT("Dark Glass Icons\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("*Credits must go to:\n");
	resultado +=  wxT("*Mentalrey for many glass icons.\n");
	resultado +=  wxT("\n");
	resultado +=  wxT("*AleXander Kirichev for Slackware kmenu icon.\n");
	resultado +=  wxT("*Amira Julia for the original art work that provided some printer and desktop icons.\n");
	resultado +=  wxT("*Dirceu for the user avatars.\n");
	resultado +=  wxT("*EKrava for the kmenu_Kubuntu icon.\n");
	resultado +=  wxT("*Everaldo Coelho (realistiK & Crystal) - Ultra high quality open source icons.\n");
	resultado +=  wxT("*Everaldo for the emotions.\n");
	resultado +=  wxT("*Futurosoft for a couple of their GPL icons.\n");
	resultado +=  wxT("*Guppetto (OS-L) - for (among other things) the best folder icons ever.\n");
	resultado +=  wxT("*hammergom for kde_fedora icon, K3B, XGame and many icons from his Crystal Glossy set.\n");
	resultado +=  wxT("*Keyser Soze for the Debian kmenu.\n");
	resultado +=  wxT("*Marco Martin (Glaze) - for many actions.\n");
	resultado +=  wxT("*Marco Martin for the kwifi icons.\n");
	resultado +=  wxT("*Marc Mironescu for some email icon.\n");
	resultado +=  wxT("*Matias Costa for creating some folder open and folder locked icons.\n");
	resultado +=  wxT("*Rowancompsciguy for the laptop battery icons\n");
	resultado +=  wxT("*Yaco for the Skype icon.\n");
	resultado += wxT("\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::WriteLibcurl()
{
	EscribeTitulo(wxT("Libcurl\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("COPYRIGHT AND PERMISSION NOTICE\n");
	resultado += wxT(" \n");
	resultado += wxT("Copyright (c) 1996 - 2012, Daniel Stenberg, <daniel@haxx.se>.\n");
	resultado += wxT(" \n");
	resultado += wxT("All rights reserved.\n");
	resultado += wxT(" \n");
	resultado += wxT("Permission to use, copy, modify, and distribute this software for any purpose");
	resultado += wxT("with or without fee is hereby granted, provided that the above copyright");
	resultado += wxT("notice and this permission notice appear in all copies.\n");
	resultado += wxT(" \n");
	resultado += wxT("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
	resultado += wxT("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
	resultado += wxT("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN");
	resultado += wxT("NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,");
	resultado += wxT("DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR");
	resultado += wxT("OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE");
	resultado += wxT("OR OTHER DEALINGS IN THE SOFTWARE.\n");
	resultado += wxT(" \n");
	resultado += wxT("Except as contained in this notice, the name of a copyright holder shall not");
	resultado += wxT("be used in advertising or otherwise to promote the sale, use or other dealings");
	resultado += wxT("in this Software without prior written authorization of the copyright holder.\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::WriteLibJsonCpp()
{
	EscribeTitulo(wxT("JsonCpp\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("The JsonCpp library's source code, including accompanying documentation, ");
	resultado += wxT("tests and demonstration applications, are licensed under the following");
	resultado += wxT("conditions...\n");
	resultado += wxT("\n");
	resultado += wxT("The author (Baptiste Lepilleur) explicitly disclaims copyright in all ");
	resultado += wxT("jurisdictions which recognize such a disclaimer. In such jurisdictions, ");
	resultado += wxT("this software is released into the Public Domain.\n");
	resultado += wxT("\n");
	resultado += wxT("In jurisdictions which do not recognize Public Domain property (e.g. Germany as of ");
	resultado += wxT("2010), this software is Copyright (c) 2007-2010 by Baptiste Lepilleur, and is ");
	resultado += wxT("released under the terms of the MIT License (see below).\n");
	resultado += wxT("\n");
	resultado += wxT("In jurisdictions which recognize Public Domain property, the user of this ");
	resultado += wxT("software may choose to accept it either as 1) Public Domain, 2) under the ");
	resultado += wxT("conditions of the MIT License (see below), or 3) under the terms of dual ");
	resultado += wxT("Public Domain/MIT License conditions described here, as they choose.\n");
	resultado += wxT("\n");
	resultado += wxT("The MIT License is about as close to Public Domain as a license can get, and is ");
	resultado += wxT("described in clear, concise terms at:\n");
	resultado += wxT("\n");
	resultado += wxT("http://en.wikipedia.org/wiki/MIT_License\n");
	resultado += wxT("\n");
	resultado += wxT("The full text of the MIT License follows:\n");
	resultado += wxT("\n");
	resultado += wxT("========================================================================\n");
	resultado += wxT("Copyright (c) 2007-2010 Baptiste Lepilleur\n");
	resultado += wxT("\n");
	resultado += wxT("Permission is hereby granted, free of charge, to any person ");
	resultado += wxT("obtaining a copy of this software and associated documentation ");
	resultado += wxT("files (the \"Software\"), to deal in the Software without ");
	resultado += wxT("restriction, including without limitation the rights to use, copy, ");
	resultado += wxT("modify, merge, publish, distribute, sublicense, and/or sell copies ");
	resultado += wxT("of the Software, and to permit persons to whom the Software is ");
	resultado += wxT("furnished to do so, subject to the following conditions:\n");
	resultado += wxT("\n");
	resultado += wxT("The above copyright notice and this permission notice shall be ");
	resultado += wxT("included in all copies or substantial portions of the Software.\n");
	resultado += wxT("\n");
	resultado += wxT("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, \n");
	resultado += wxT("EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF ");
	resultado += wxT("MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND ");
	resultado += wxT("NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS ");
	resultado += wxT("BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ");
	resultado += wxT("ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN ");
	resultado += wxT("CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE ");
	resultado += wxT("SOFTWARE.\n");
	resultado += wxT("========================================================================\n");
	resultado += wxT("(END LICENSE TEXT)\n");
	resultado += wxT("\n");
	resultado += wxT("The MIT license is compatible with both the GPL and commercial ");
	resultado += wxT("software, affording one all of the rights of Public Domain with the ");
	resultado += wxT("minor nuisance of being required to keep the above copyright notice ");
	resultado += wxT("and license text in the source code. Note also that by accepting the ");
	resultado += wxT("Public Domain \"license\" you can re-license your copy using whatever ");
	resultado += wxT("license you like.\n");
	EscribeLicencia(resultado);
}

void DialogoLicenciasOpenSource::WriteLibAIRS()
{
	EscribeTitulo(wxT("AIRS\n\n\n"));
	wxString resultado = wxEmptyString;
	resultado += wxT("==========================================================================\n");
	resultado += wxT("\n");
	resultado += wxT("Copyright (c) 2000-2010 Atamai, Inc.\n");
	resultado += wxT("\n");
	resultado += wxT("Use, modification and redistribution of the software, in source or\n");
	resultado += wxT("binary forms, are permitted provided that the following terms and\n");
	resultado += wxT("conditions are met:\n");
	resultado += wxT("\n");
	resultado += wxT("\t1) Redistribution of the source code, in verbatim or modified\n");
	resultado += wxT("\tform, must retain the above copyright notice, this license,\n");
	resultado += wxT("\tthe following disclaimer, and any notices that refer to this\n");
	resultado += wxT("\tlicense and/or the following disclaimer.\n");
	resultado += wxT("\n");
	resultado += wxT("\t2) Redistribution in binary form must include the above copyright\n");
	resultado += wxT("\tnotice, a copy of this license and the following disclaimer\n");
	resultado += wxT("\tin the documentation or with other materials provided with the\n");
	resultado += wxT("\tdistribution.\n");
	resultado += wxT("\n");
	resultado += wxT("\t3) Modified copies of the source code must be clearly marked as such,\n");
	resultado += wxT("\tand must not be misrepresented as verbatim copies of the source code.\n");
	resultado += wxT("\n");
	resultado += wxT("THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE SOFTWARE \"AS IS\"\n");
	resultado += wxT("WITHOUT EXPRESSED OR IMPLIED WARRANTY INCLUDING, BUT NOT LIMITED TO,\n");
	resultado += wxT("THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n");
	resultado += wxT("PURPOSE. IN NO EVENT SHALL ANY COPYRIGHT HOLDER OR OTHER PARTY WHO MAY\n");
	resultado += wxT("MODIFY AND/OR REDISTRIBUTE THE SOFTWARE UNDER THE TERMS OF THIS LICENSE\n");
	resultado += wxT("BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES\n");
	resultado += wxT("(INCLUDING, BUT NOT LIMITED TO, LOSS OF DATA OR DATA BECOMING INACCURATE\n");
	resultado += wxT("OR LOSS OF PROFIT OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF\n");
	resultado += wxT("THE USE OR INABILITY TO USE THE SOFTWARE, EVEN IF ADVISED OF THE\n");
	resultado += wxT("POSSIBILITY OF SUCH DAMAGES.\n");
	resultado += wxT("\n");
	resultado += wxT("=========================================================================\n");

	EscribeLicencia(resultado);
}