/*
*  
*  $Id: thumbnailscommand.h $
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
#include <api/icommand.h>
#include <api/api.h>

class wxImage;

namespace GADAPI {	
	class ThumbnailsCommandParams;

	class EXTAPI ThumbnailsNotifier
	{
	public:
		ThumbnailsNotifier(){}
		virtual ~ThumbnailsNotifier(){}

		virtual void SetImage(long file_pk, wxImage* pImage) = 0;
	};

	class EXTAPI ThumbnailsCommand : public GNC::GCS::IComando {
	public:
		ThumbnailsCommand(long file_pk, ThumbnailsNotifier* pNotificador);

		wxImage* GetImage();

    protected:
		void Execute();
		void Update();

	protected:
		wxImage* GetImageFromBD();
		void GuardarImagen();

		ThumbnailsCommandParams* m_pThumbParams;
		bool                            m_Error;

	};
}
