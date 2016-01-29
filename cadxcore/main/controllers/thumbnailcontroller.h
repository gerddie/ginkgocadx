/*
 *  
 *  $Id: thumbnailcontroller.h $
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
#include <wx/timer.h>

#include <api/api.h>
#include <api/observers/ieventsobserver.h>
#include <map>
#include <commands/thumbnailscommand.h>

class wxCriticalSection;
namespace GNC {
	namespace GCS {
		class EXTAPI ThumbnailController: public GNC::GCS::IEventsObserver, public wxTimer
		{
		public:
			static ThumbnailController* Instance();
			static void FreeInstance();

			
		protected:
			static ThumbnailController* m_pInstance;
			ThumbnailController();
			~ThumbnailController();

		public:
			void requestThumbnail(long file_pk, ::GADAPI::ThumbnailsNotifier* pNotifier);
			void removeRequest(::GADAPI::ThumbnailsNotifier* pNotifier);
		protected:
			virtual void Notify();

			virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

			typedef struct TPetition {
				GADAPI::ThumbnailsNotifier* m_pNotifier;
				long m_file_pk;
				TPetition(::GADAPI::ThumbnailsNotifier* pNotifier, long file_pk) {
					m_pNotifier = pNotifier;
					m_file_pk = file_pk;
				}
			} TPetition;

			typedef std::list<TPetition> TListPetitions;

			
			::GADAPI::ThumbnailsCommand* m_pCmd;
			TListPetitions m_petitions;
			wxCriticalSection* m_pCriticalSection;
		};
	}
}
