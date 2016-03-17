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
#include <wx/timer.h>

#include <api/api.h>
#include <api/observers/ieventsobserver.h>
#include <map>
#include <commands/thumbnailscommand.h>

class wxCriticalSection;
namespace GNC
{
namespace GCS
{
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
                TPetition(::GADAPI::ThumbnailsNotifier* pNotifier, long file_pk)
                {
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
