/*
*
*  $Id: iwindowlayoutcontract.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
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

