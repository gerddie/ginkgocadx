/*
 *  
 *  $Id: helpertexto.h $
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
#include <api/math/geometry.h>
#include <api/iwidgets.h>
#include <widgets/openglhelper.h>

namespace GNC {
	namespace GCS {
		class Contexto3D;
	}
}
//endregion
struct _cairo_font_options;

namespace GNC
{
	namespace GCS
	{
		namespace Widgets {
			struct StripDescription
			{
				StripDescription();
				StripDescription(GNC::GCS::Vector position, GNC::GCS::Vector size);

				GNC::GCS::Vector rasterPosition;
				GNC::GCS::Vector rasterSize;
				GNC::GCS::Vector textPosition;
				GNC::GCS::Vector textSize;
			};	

			class StripTextRenderer
			{
			public:
				StripTextRenderer();

				~StripTextRenderer();

				void Render(const std::string& text, const GNC::GCS::Vector& renderPosition, bool leftAlign = true);

				void Update(const std::string& strip, const GNC::GCS::Widgets::Color& color, double fontSize, bool drawShadow);

				double GetTextHeigh();

				void Destroy();

				std::map<char, StripDescription> m_CharMap;
				_cairo_font_options* m_pFontOptions;
				double m_textHeigh;
				std::string m_text;
				GNC::GCS::TexturaCairo tc;
			}; 
		}
	}
}
