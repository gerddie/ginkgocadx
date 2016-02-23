/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; version 3. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "selectpacsserver.h"
#include <main/controllers/dcmtk/dicomservers.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/ginkgostyle/ginkgostyle.h>

namespace GNC {
	namespace GUI {
		SelectPacsServer::SelectPacsServer(wxWindow* pParent) : SelectPacsServerBase(pParent)
		{
			wxArrayString pacsList;
			DicomServerList::TServerList ListOfServers = DicomServerList::Instance()->GetServerList();
			for (DicomServerList::TServerList::iterator it = ListOfServers.begin(); it != ListOfServers.end(); ++it)
			{
				pacsList.push_back(wxString::FromAscii((*it)->ID.c_str()));
			}
			m_pServer->Append(pacsList);

			if(m_pServer->GetCount()>0){
				m_pServer->SetSelection(0);
				std::string pacsDef;
				GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/SubirPACS", "PACSDefecto", pacsDef);
				
				for (unsigned int i =0; i< m_pServer->GetCount(); ++i) {
					if (m_pServer->GetString(i) == wxString::FromUTF8(pacsDef.c_str())) {
						m_pServer->Select(i);
						break;
					}
				}
			} else {
				m_pServer->AppendString(_("There aren't any PACS configured"));
				m_pServer->SetSelection(0);
				m_pServer->Disable();
				m_pOkButton->Disable();
			}

			m_pPanelMessage->Layout();
			m_pBody->Layout();
		}
		SelectPacsServer::~SelectPacsServer()
		{
		}


		std::string SelectPacsServer::GetSelectedServer()
		{
			int selection = m_pServer->GetSelection();
			if (selection >=0){
				std::string idServer(m_pServer->GetString(selection).ToUTF8());
				GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/SubirPACS", "PACSDefecto", idServer);
				return idServer;
			} else {
				return "";
			}
		}
	}
}
