/*
 *  
 *  $Id: helpertexto.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "striptexthelper.h"
#include <api/math/geometry.h>
#include <api/westilo.h>
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtkgl.h>
#include <cairo/cairo.h>

GNC::GCS::Widgets::StripDescription::StripDescription()
{
}
GNC::GCS::Widgets::StripDescription::StripDescription(GNC::GCS::Vector position, GNC::GCS::Vector size)
{
	this->rasterPosition = position;
	this->rasterSize = size;
}

GNC::GCS::Widgets::StripTextRenderer::StripTextRenderer()
{
	m_pFontOptions = cairo_font_options_create();
	tc.Redimensionar(2, 2);
	cairo_get_font_options(tc.cr, m_pFontOptions);
	cairo_font_options_set_antialias (m_pFontOptions, CAIRO_ANTIALIAS_NONE);
	cairo_set_font_options(tc.cr, m_pFontOptions);
}

GNC::GCS::Widgets::StripTextRenderer::~StripTextRenderer()
{
	cairo_font_options_destroy(m_pFontOptions);
	m_pFontOptions = NULL;
}

void GNC::GCS::Widgets::StripTextRenderer::Destroy()
{
	tc.Destruir();
}

void GNC::GCS::Widgets::StripTextRenderer::Render(const std::string& text, const GNC::GCS::Vector& renderPosition, bool leftAlign)
{
	tc.Actualizar();
	GNC::GCS::Vector curRenderPosition = renderPosition;

	GNC::GCS::Vector quadRaster[4];
	GNC::GCS::Vector quadTexture[4];
	
	GNC::GCS::Vector rasterPos = curRenderPosition.Redondeado();
		 
	quadRaster[0].x = rasterPos.x;
	quadRaster[0].y = rasterPos.y;
	
	glEnable(GL_TEXTURE_2D);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, tc.idTextura);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	for (std::string::const_iterator it1 = text.begin(); it1 != text.end(); it1++)
	{	
		if (m_CharMap.find((*it1)) == m_CharMap.end()) {
			continue;
		}
		const StripDescription& sd = m_CharMap[(*it1)];
		if (!leftAlign) {
			quadRaster[0].x -= sd.rasterSize.x;
		}
		//raster position
		quadRaster[2] = quadRaster[0] + sd.rasterSize;

		quadRaster[1].x = quadRaster[2].x;
		quadRaster[1].y = quadRaster[0].y;
	
		quadRaster[3].x = quadRaster[0].x;
		quadRaster[3].y = quadRaster[2].y;
		//texture position 0.0,1.0

		glBegin(GL_QUADS);
			glTexCoord2d(sd.textPosition.x, sd.textPosition.y);
			glVertex2d(quadRaster[0].x, quadRaster[0].y);

			glTexCoord2d(sd.textPosition.x + sd.textSize.x, sd.textPosition.y);
			glVertex2d(quadRaster[1].x, quadRaster[1].y);

			glTexCoord2d(sd.textPosition.x + sd.textSize.x, sd.textPosition.y + sd.textSize.y);
			glVertex2d(quadRaster[2].x, quadRaster[2].y);

			glTexCoord2d(sd.textPosition.x, sd.textPosition.y + sd.textSize.y);
			glVertex2d(quadRaster[3].x, quadRaster[3].y);
		glEnd();

		//increments raster position...
		if (leftAlign) {
			quadRaster[0].x += sd.rasterSize.x;
		} 
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void GNC::GCS::Widgets::StripTextRenderer::Update(const std::string& strip, const GNC::GCS::GLHelper::TColor& color, double fontSize, bool drawShadow)
{
	m_text = strip;

	if (tc.EsValida() && (m_text.size() > 0) )
	{
			
		///process strip...
		m_CharMap.clear();
		//measure each char...
			
		cairo_text_extents_t text_extents;
			
		_cairo* cr = tc.cr;
		//clear canvas...
		cairo_select_font_face(cr, "cairo:monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size(cr, fontSize);

		cairo_get_font_options(cr, m_pFontOptions);
		cairo_font_options_set_antialias (m_pFontOptions, CAIRO_ANTIALIAS_NONE);
		cairo_set_font_options(cr, m_pFontOptions);
		cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

		cairo_font_extents_t      font_extents;
		cairo_font_extents(cr, &font_extents);
		m_textHeigh = font_extents.height + 2;
			
		int elementsPerRow = std::ceil(sqrt((double)m_text.size()));
		GNC::GCS::Vector pos(0.0, 0.0);
		GNC::GCS::Vector size(0,0);
		std::string charString;
		int currElement = 0;
		for (std::string::const_iterator it1 = m_text.begin(); it1 != m_text.end(); it1++)
		{			
			charString = (*it1);
			if (m_CharMap.find((*it1)) != m_CharMap.end()) {
				continue;
			}
			//measure char
			cairo_text_extents(cr, charString.c_str(), &text_extents);
			GNC::GCS::Vector psize (text_extents.x_advance + 2, font_extents.height + 2);
			m_CharMap[(*it1)] = StripDescription(pos, psize);
						
			currElement++;
			if (currElement%elementsPerRow == 0) {
				size.x = std::max<double>(size.x, pos.x);
				pos.x = 0;
				pos.y += font_extents.height + 2;
			} else {
				pos.x += psize.x;
			}
			pos.RedondearAlza();
		}

		size.y = pos.y + font_extents.height;
		size.RedondearAlza();
		tc.Redimensionar(size.x, size.y);
		size.x = tc.anchoPOT;
		size.y = tc.altoPOT;
		cr = tc.cr;
		//clear canvas...
		cairo_select_font_face(cr, "cairo:monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size(cr, fontSize);

		cairo_get_font_options(cr, m_pFontOptions);
		cairo_font_options_set_antialias (m_pFontOptions, CAIRO_ANTIALIAS_NONE);
		cairo_set_font_options(cr, m_pFontOptions);
		cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

		cairo_set_source_rgba (cr, 1.0f, 1.0f, 0.0f, 0.0f);
		cairo_paint(cr);
		//

		currElement = 0;
		for (std::map<char, StripDescription>::iterator it = m_CharMap.begin(); it != m_CharMap.end(); ++it)
		{
			//draw char
			charString = (*it).first;
			GNC::GCS::Vector& pos = (*it).second.rasterPosition;

			if (drawShadow) {
				cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);			
				cairo_move_to(cr, std::ceil(pos.x + 1.0f), std::ceil(pos.y + font_extents.height + 1.0f));							
				cairo_set_source_rgba(cr, DEFAULT_TEXT_COLOR_INVERTED);		
				cairo_show_text(cr,charString.c_str());
				cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
			}
			else {
				cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
			}

			cairo_move_to(cr, std::ceil(pos.x), std::ceil(pos.y + font_extents.height));
				
			cairo_set_source_rgba(cr, color.RF(), color.GF(), color.BF(), color.AF());
				
			cairo_show_text(cr, charString.c_str());
			//update position and size...
			(*it).second.textPosition = (*it).second.rasterPosition/ size;
			//(*it).second.textPosition.RedondearAlza();
			(*it).second.textSize = (*it).second.rasterSize/size;
			//(*it).second.textSize.RedondearAlza();
		}
			
	}
}

double GNC::GCS::Widgets::StripTextRenderer::GetTextHeigh()
{
	return m_textHeigh;
}