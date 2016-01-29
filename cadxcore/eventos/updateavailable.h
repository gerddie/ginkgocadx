/*
*  
*  $Id: updateavailable.h $
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

#include <api/ievento.h>
#include "eventosginkgo.h"

namespace GNC {
	namespace GCS {
		namespace Events {

			class EXTAPI EventoUpdateAvailable : public GNC::GCS::Events::IEvent
			{
			private:
				std::string m_VersionString;
				int         m_Revision;
				std::string m_Description;
				std::string m_URL;
				bool        m_Informar;

			public:
				EventoUpdateAvailable() :
						GNC::GCS::Events::IEvent(ginkgoEVT_Core_UpdateAvailable, 0, 100, NULL),
						m_VersionString(),
						m_Revision(0),
						m_Description(),
						m_URL(),
						m_Informar(false)
				{
					m_Nombre = "UpdateAvailable";
				}

				EventoUpdateAvailable(std::string& versionString, int revision, const std::string& description, const std::string& url, bool informar):
						GNC::GCS::Events::IEvent(ginkgoEVT_Core_UpdateAvailable, 0, 100, NULL),
						m_VersionString(versionString),
						m_Revision(revision),
						m_Description(description),
						m_URL(url),
						m_Informar(informar)

				{
					m_Nombre = "UpdateAvailable";
				}

				~EventoUpdateAvailable()
				{
				}

				const std::string& GetURL() {
					return m_URL;
				}

				bool Informar() {
					return m_Informar;
				}

				int GetRevision() {
					return m_Revision;
				}

				const std::string& GetVersionString() {
					return m_VersionString;
				}

				const std::string& GetDescription() {
					return m_Description;
				}

				virtual void pushInfo(std::ostream& out) const {
					out << "UpdateAvailable (" << m_VersionString << ") from " << m_URL;
				}
			};
		}
	}
}
