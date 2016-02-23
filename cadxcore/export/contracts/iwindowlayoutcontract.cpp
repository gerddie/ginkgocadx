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


#include "iwindowlayoutcontract.h"

GNC::GCS::IWindowLayoutContract::IWindowLayoutContract()
{
	m_MinSlice = 0;
	m_MaxSlice = 0;
}

GNC::GCS::IWindowLayoutContract::~IWindowLayoutContract()
{
}

void GNC::GCS::IWindowLayoutContract::Setup(int minSlice, int maxSlice)
{
	m_MinSlice = minSlice;
	m_MaxSlice = maxSlice;
}

void GNC::GCS::IWindowLayoutContract::AddRow()
{
	int filas = GetRows()+1;
	int columnas = GetColumns();
	SetWindowLayout(filas,columnas);
}

void GNC::GCS::IWindowLayoutContract::AddColumn()
{
	int filas = GetRows();
	int columnas = GetColumns()+1;
	SetWindowLayout(filas,columnas);
}

void GNC::GCS::IWindowLayoutContract::DelRow()
{
	int filas = GetRows()-1;
	int columnas = GetColumns();
	SetWindowLayout(filas,columnas);
}

void GNC::GCS::IWindowLayoutContract::DelColumn()
{
	int filas = GetRows();
	int columnas = GetColumns()-1;
	SetWindowLayout(filas,columnas);
}

void GNC::GCS::IWindowLayoutContract::SplitActive(bool )
{
}

bool GNC::GCS::IWindowLayoutContract::SupportsSplit()
{
	return false;
}

