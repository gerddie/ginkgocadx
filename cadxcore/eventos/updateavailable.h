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
