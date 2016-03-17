/*
 *  Ginkgo CADx Project
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
#include <api/math/geometry.h>
#include <api/iwidgets.h>
#include <widgets/openglhelper.h>

namespace GNC
{
namespace GCS
{
class Contexto3D;
}
}
//endregion
struct _cairo_font_options;

namespace GNC
{
namespace GCS
{
namespace Widgets
{
struct StripDescription {
        StripDescription() = default;
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
