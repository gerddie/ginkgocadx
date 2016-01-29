/*
 *  
 *  $Id: parserxmlenglish.h $
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
#pragma once
#include <api/ientorno.h>
#include <main/controllers/integrationcontroller.h>
#include <api/imodelohl7.h>

namespace GIL {
	class ParserXMLEnglish : public GIL::IXMLIntegrationParser {
	public:
		typedef std::map<std::string, GIL::IModeloPACSServer> MapaServers;

		ParserXMLEnglish();
		~ParserXMLEnglish();
		virtual std::list<std::string> GetKeys();
		virtual void ParseIntegrationXML(GIL::IntegrationModelList& models, wxXmlNode* pRoot);	
		virtual GIL::IModeloIntegracion* ParseGnkWorkflow(wxXmlNode* pNode, MapaServers& servers);		
	protected:
	};
}
