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
#include <string>

#if !defined(GINKGO_VERSION)
#define GINKGO_VERSION "3.7.1"
#endif

#if !defined(GINKGO_REVISION)
#define GINKGO_REVISION 1573
#endif

#if !defined(GINKGO_BUILD)
#define GINKGO_BUILD "41"
#endif

#define GINKGO_COPYRIGHT   "© 2007-2016"
#define PRODUCT_NAME	"Ginkgo CADx"
#define PRODUCT_SUBTITLE _Std("Open Source DICOM CADx environment")
#define CREDITS "<a href=\"https://github.com/gerddie/ginkgocadx\">https://github.com/gerddie/ginkgocadx</a>"\
"<p>© 2015-2016 Gert Wollny</p>"\
"<p>© 2007-2014 <a href=\"http://healthcare.metaemotion.com/\">MetaEmotion Healthcare</a></p>"\
"<p>¡Queremos agradecer a Sacyl por todo el apoyo en la creación de Ginkgo CADx!</p>"\
"<h3>Traductores</h3>"\
"<ul>"\
"<li>Japonés: Noriya Yokohama</li>"\
"<li>Alemán: Sebastian Hilbert</li>"\
"<li>Turco: zeugma (sunder67)</li>"\
"<li>Español: JFLP (jluna80)</li>"\
"<li>Portugués (Brasil): Marcel Ribeiro Dantas</li>"\
"<li>Portugués (Portugal): Vitor Neves</li>"\
"</ul>"\
"<h3>Agradecimientos</h3>"\
"<ul>"\
"<li>Debian-Med team especialmente a Andreas Tille and Karlsten Hilbert: empaquetado, pruebas y reporte de errores</li>"\
"<li>Ross Boswell: pruebas de compatibilidad con PACS</li>"\
"</ul>"


//
//
// Permisos completos
// "<?xml version=\"1.0\"?>\n"
// "<permisos>\n"
// "<permiso namespace=\"core\" id=\"default\" accion=\"activar\"/>"
// "<permiso namespace=\"core.pacs.limits\" id=\"pacs\" accion=\"desactivar\"/>"
// "<permiso namespace=\"core.pacs.limits\" id=\"pacs_upload\" accion=\"activar\"/>"
// "<permiso namespace=\"core.menu\" id=\"configuracion\" accion=\"desactivar\"/>"
// "<permiso namespace=\"core.menu\" id=\"filtrado\" accion=\"activar\"/>"
// "<permiso namespace=\"atencionprimaria\" id=\"dicomizador\" accion=\"activar\"/>"
// "<permiso namespace=\"atencionprimaria\" id=\"vista2d\" accion=\"activar\"/>"
// "<permiso namespace=\"atencionprimaria\" id=\"vistasac\" accion=\"activar\"/>"
// "<permiso namespace=\"atencionprimaria.vistasimple\" id=\"guardar cambios\" accion=\"desactivar\"/>"
// "<permiso namespace=\"oftalmologia\" id=\"dicomizador\" accion=\"activar\"/>"
// "<permiso namespace=\"oftalmologia\" id=\"vistaoftal\" accion=\"activar\"/>"
// "<permiso namespace=\"oftalmologia.vistaoftal\" id=\"Marcado del arbol de venas\" accion=\"activar\"/>"
// "<permiso namespace=\"oftalmologia.vistaoftal\" id=\"Marcado de Oftalmologia\" accion=\"activar\"/>"
// "<permiso namespace=\"oftalmologia.vistaoftal\" id=\"CoreMarcado\" accion=\"activar\"/>"
// "<permiso namespace=\"oftalmologia.vistaoftal\" id=\"Pegado de imagenes de la serie\" accion=\"desactivar\"/>"
// "<permiso namespace=\"oftalmologia.vistaoftal\" id=\"CoreAnotacion\" accion=\"activar\"/>"
// "<permiso namespace=\"oftalmologia.vistaoftal\" id=\"CoreElevacion\" accion=\"desactivar\"/>"
// "<permiso namespace=\"oftalmologia.vistaoftal\" id=\"CoreVarita\" accion=\"desactivar\"/>"
// "<permiso namespace=\"oftalmologia.vistaoftal\" id=\"panel de herramientas\" accion=\"desactivar\"/>"
// "<permiso namespace=\"oftalmologia.vistaoftal\" id=\"guardar cambios\" accion=\"desactivar\"/>"
// "<permiso namespace=\"dermatologia\" id=\"dicomizador\" accion=\"activar\"/>"
// "<permiso namespace=\"dermatologia\" id=\"vistapasi\" accion=\"activar\"/>"
// "<permiso namespace=\"dermatologia.vistapasi\" id=\"CoreMarcado\" accion=\"desactivar\"/>"
// "<permiso namespace=\"dermatologia.vistapasi\" id=\"DermaAnonimizacion\" accion=\"desactivar\"/>"
// "<permiso namespace=\"dermatologia.vistapasi\" id=\"CoreNota\" accion=\"desactivar\"/>"
// "<permiso namespace=\"dermatologia.vistapasi\" id=\"CoreElevacion\" accion=\"desactivar\"/>"
// "<permiso namespace=\"dermatologia.vistapasi\" id=\"CoreVarita\" accion=\"desactivar\"/>"
// "<permiso namespace=\"dermatologia.vistapasi\" id=\"panel de herramientas pasi\" accion=\"activar\"/>"
// "<permiso namespace=\"dermatologia.vistapasi\" id=\"guardar cambios\" accion=\"desactivar\"/>"
// "</permisos>\n"
