/*
 *  
 *  $Id: dicomizecommand.h $
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
#include <commands/dicomizecommand.h>


namespace GADAPI {
	class SecondaryCaptureDicomizeCommandParameters : public ::GADAPI::DicomizeCommandParams
	{
	public:
		SecondaryCaptureDicomizeCommandParameters(GNC::GCS::IVista* pView, const std::string& screenShot);

		GNC::GCS::IVista* m_pView;
		std::string m_originalImagePath;
		std::string m_screenShot;
	};

	class SecondaryCaptureDicomizeCommand : public ::GADAPI::DicomizeCommand {
	public:
		SecondaryCaptureDicomizeCommand(::GADAPI::SecondaryCaptureDicomizeCommandParameters* pParams);

		protected:
			virtual void Update();
			virtual bool ImportarFicherosOriginales();

			SecondaryCaptureDicomizeCommandParameters* m_pSecondaryParams;
	};
}
