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


#include <cstdio>

#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladorpermisos.h>
#include <main/controllers/pacscontroller.h>
#include <main/controllers/dicommanager.h>


#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif

// For sime reason dcmtk-3.6.1-20150924 has an hiccup in
// dcmth/oflog/  logger.h and logmacro.h
// since they are not used we define the include guards here
#define DCMTK_LOG4CPLUS_TRACELOGGER_H
#define DCMTK_LOG4CPLUS_LOGGING_MACROS_HEADER_

#include "libi2d/document2dcm.h"
#include "libi2d/i2djpgs.h"
#include "libi2d/pdf2dsource.h"
#include "libi2d/i2dles.h"
#include "libi2d/i2dplsc.h"
#include "libi2d/i2dplvlp.h"
#include "libi2d/foooutpl.h"
#include "libi2d/i2dplnsc.h"
#include "dicomimg2dcm.h"
#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmdata/dchashdi.h"

#include <dcmtk/dcmsr/dsrdoc.h>
#include <dcmtk/dcmdata/dcfilefo.h>


#include <wx/image.h>
//en este grupo se almacenaran los atributos privados de ginkgo

#ifdef DCMTK_POST_20170228
#include <api/dicom/dicom_legacy.h>
#endif 

#define GINKGO_GROUP 0x0011

class DcmElement;

namespace GIL
{
namespace DICOM
{

//region "Helpers"

DcmElement* DICOMImg2DCM::CrearElementoConValor(const char* s)
{
        unsigned int g = 0xffff;
        unsigned int e = 0xffff;

        OFString dicName, valStr;
        OFString msg;

        OFString str = s;
        size_t pos = str.find('=');
        if (pos != OFString_npos) {
                valStr = str.substr(pos + 1, str.length());
                dicName = str.substr(0, pos);
        } else {
                dicName = str;
        }
        pos = dicName.find("|");
        if (pos != OFString_npos) {
#if defined(_WIN32)
                if (2 != _snscanf(dicName.c_str(), 11, "%x|%x", &g, &e))
#else
                if (2 != std::sscanf(dicName.c_str(), "%x|%x", &g, &e))
#endif
                {
                        LOG_ERROR("Dicomizador", "Error al interpretar el tag " << s);
                        return NULL;
                }
        } else {
                DcmTagKey key(0xffff, 0xffff);
                const DcmDataDictionary& globalDataDict = dcmDataDict.rdlock();
                const DcmDictEntry *dicent = globalDataDict.findEntry(dicName.c_str());
                dcmDataDict.rdunlock();
                if (dicent != NULL) {
                        // found dictionary name, copy group and element number
                        key = dicent->getKey();
                        g = key.getGroup();
                        e = key.getElement();
                } else {
                        // not found in dictionary
                        std::cerr <<  "bad key format or dictionary name not found in dictionary: " << dicName << std::endl;
                        LOG_ERROR("Dicomizador", "Error al obtener la entrada del diccionario para el tag " << dicName.c_str() << ". Cadena: " << s);
                        return NULL;
                }

        }
        DcmTag tag(g, e);
        if (tag.error() != EC_Normal) {
                std::cerr << "Tag desconocido: (" <<
                          std::hex << std::setw(4) << std::setfill('0') << g << "|" <<
                          std::hex << std::setw(4) << std::setfill('0') << e << ")" << std::endl;
                return NULL;
        }
        DcmElement *elem = newDicomElement(tag);
        if (elem == NULL) {
                std::cerr << "No se pudo crear el elemento para el tag: (" <<
                          std::hex << std::setw(4) << std::setfill('0') << g << "|" <<
                          std::hex << std::setw(4) << std::setfill('0') << e << ")" << std::endl;
                return NULL;
        }
        if (valStr.length() > 0) {
                if (elem->putString( wxString( valStr.c_str(), wxConvUTF8).mb_str(*m_pConv) ).bad()) {
                        std::cerr << "No se pudo asignar el valor al elemento: (" <<
                                  std::hex << std::setw(4) << std::setfill('0') << g << "|" <<
                                  std::hex << std::setw(4) << std::setfill('0') << e << ")=" << valStr.c_str() << std::endl;
                        delete elem;
                        return NULL;
                }
        }
        return elem;

}

DICOMImg2DCM::DICOMImg2DCM()
{
        i2d = NULL;
        outputPlug = NULL;
        inputPlug = NULL;
        resultObject = NULL;
        writeXfer = EXS_Unknown;

        std::string Charset;
        GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","DefaultCharset",Charset,"ISO_IR 100");
        if (Charset == "ISO_IR 192" || Charset == "ISO_IR 6") {
                m_pConv = new wxCSConv(wxFONTENCODING_UTF8);
        } else if (Charset == "ISO_IR 100") {
                m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_1);
        } else if (Charset == "ISO_IR 101") {
                m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_2);
        } else if (Charset == "ISO_IR 109") {
                m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_3);
        } else if (Charset == "ISO_IR 110") {
                m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_4);
        } else if (Charset == "ISO_IR 148") {
                m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_9);
        } else if (Charset == "ISO_IR 144") {
                m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_5);
        } else if (Charset == "ISO_IR 127") {
                m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_6);
        } else if (Charset == "ISO_IR 126") {
                m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_7);
        } else if (Charset == "ISO_IR 138") {
                m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_8);
        } else {
                m_pConv = new wxCSConv(wxFONTENCODING_ISO8859_1);
        }
};
DICOMImg2DCM::~DICOMImg2DCM()
{
        LiberarMemoria();
        delete m_pConv;
}

void DICOMImg2DCM::LiberarMemoria()
{
        if (resultObject != NULL) {
                delete resultObject;
                resultObject = NULL;
        }
        if (inputPlug != NULL) {
                delete inputPlug;
                inputPlug = NULL;
        }
        if (outputPlug != NULL) {
                delete outputPlug;
                outputPlug = NULL;
        }
        if (i2d != NULL) {
                delete i2d;
                i2d = NULL;
        }
};


//endregion


void DICOMImg2DCM::Inicializar(std::string& inputFile, IInspectCallBack* pICallback, bool debug, TDICOMOutputFormat outputFormat)
{
        LiberarMemoria();

        if (inputFile.length() == 0) {
                throw I2DException("No se ha especificado el fichero de entrada");
        }

        if (GIL::DICOM::PACSController::Instance()->EsPDF(inputFile)) { // Special PDF Dicomization
                //pdf => foo not need output plug
                if (outputFormat == TDOF_AUTO) {
                        outputFormat = TDOF_FOO;
                }

                PDF2DSource* pdfSource = new PDF2DSource();
                if (pdfSource == NULL) {
                        throw I2DException(_Std("Unable to reserve memory for PDF dicomization"));
                }
                pdfSource->setPDFFile(inputFile.c_str());
                Inicializar(pdfSource, pICallback, debug, outputFormat);
        } else {
                // Traditional Image dicomization

                //image => vlp
                if (outputFormat == TDOF_AUTO) {
                        outputFormat = TDOF_VLP;
                }

                GNC::GCS::Permisos::EstadoPermiso mantenerCodificacion = GNC::GCS::IControladorPermisos::Instance()->Get("core.importacion", "mantener");

                if (mantenerCodificacion.Activo()) {
                        //probamos a ver si nos vale cn un JPEG source
                        I2DJpegSource *jpgSource = new I2DJpegSource();
                        if (jpgSource == NULL) {
                                throw I2DException(_Std("Unable to reserve memory for image Dicomization"));
                        }
                        // Set JPEG Options
                        // Enable/Disable support for progressive JPEG
                        jpgSource->setProgrSupport(OFTrue);
                        // Enable/Disable support for extended sequential JPEG
                        jpgSource->setExtSeqSupport(OFTrue);
                        jpgSource->setImageFile(inputFile.c_str());
                        Inicializar(jpgSource, pICallback, debug, outputFormat);
                } else {
                        wxString nombreImagen = FROMPATH(inputFile);
                        wxImage img(nombreImagen);
                        if(!img.Ok()) {
                                throw I2DException(_Std("Image format not supported"));
                        }

                        unsigned char* imgData = img.GetData();
                        TImageFile rgbLittleEndian(imgData, img.GetWidth(), img.GetHeight());
                        Inicializar(rgbLittleEndian, pICallback, debug, outputFormat);
                }
        }
}

void DICOMImg2DCM::Inicializar(TImageFile& fileDefinition, IInspectCallBack* pICallback, bool debug, TDICOMOutputFormat outputFormat)
{
        I2DLittleEndianSource* littleEndianSource = new I2DLittleEndianSource(fileDefinition);
        Inicializar(littleEndianSource, pICallback, debug, outputFormat);
}

void DICOMImg2DCM::Inicializar(D2DSource* plugEntrada, IInspectCallBack* pICallback, bool debug, TDICOMOutputFormat outputFormat)
{
        LiberarMemoria();

        inputPlug = plugEntrada;

        i2d = new Document2Dcm();
        if (i2d == NULL) {
                throw I2DException("No se pudo reservar memoria para realizar la conversion");
        }

        OFBool dMode = OFFalse;
        OFBool vMode = OFFalse;

        if (debug) {
                dMode = OFTrue;
                vMode = OFTrue;
                i2d->setDebugMode(OFTrue);
                i2d->setLogStream(&ofConsole);
        }

        if (vMode)
                std::cout << inputPlug->inputFormat() << std::endl;

        if (outputFormat == TDOF_VLP || outputFormat == TDOF_AUTO) {
                outputPlug = new I2DOutputPlugVLP();
        } else if (outputFormat == TDOF_SC) {
                outputPlug = new I2DOutputPlugSC();
        } else if (outputFormat == TDOF_NSC) {
                outputPlug = new I2DOutputPlugNewSC();
        } else if (outputFormat == TDOF_FOO) {
                outputPlug = new FooOutputPlug();
        } else {
                throw I2DException("No existe ningun plugin para el formato de salida especificado");
        }


        if (vMode)
                std::cout << outputPlug->ident() << std::endl;

        outputPlug->setDebugMode(dMode);
        outputPlug->setLogStream(&ofConsole);

        ActualizarCampos(pICallback);

        // ISO Latin 1 option
        //  setISOLatin1(OFTrue)  => set latin-1 as standard character set
        //  setISOLatin1(OFFalse) => keep 7-bit ASCII as standard character set
        //  i2d->setISOLatin1(OFTrue);
        i2d->setUTF8(OFTrue);

        // attribute validity checking
        //   enabled/disabled
        OFBool doChecks = OFTrue;

        // insert missing type 2 attributes (Sólo si doChecks es True)
        //   enabled/disabled
        OFBool insertType2 = OFTrue;

        // invent missing type 1 attributes (Sólo si doChecks es True)
        //   enabled/disabled
        OFBool inventType1 = OFTrue;

        i2d->setValidityChecking(doChecks, insertType2, inventType1);
        outputPlug->setValidityChecking(doChecks, insertType2, inventType1);

        inputPlug->setDebugMode(dMode);
        inputPlug->setLogStream(&ofConsole);

        // make sure data dictionary is loaded
        if (!dcmDataDict.isDictionaryLoaded()) {
                LiberarMemoria();
                throw I2DException("No se ha cargado el diccionario de datos");
        }

        OFCondition cond;

        cond = i2d->convert(inputPlug, outputPlug, resultObject, writeXfer);

        if (cond.bad()) {
                //si es un jpegsource se trata de leer la imagen y convertirla desde little endian... (por aqui pasan los png, bmp,... cuando esta activo el mantener)
                I2DJpegSource* pJPeg = dynamic_cast<I2DJpegSource*>(inputPlug);
                if (pJPeg != NULL) {
                        OFString path = pJPeg->getImageFile();
                        LiberarMemoria();
                        wxString nombreImagen = FROMPATH(path);
                        wxImage img(nombreImagen);
                        if(!img.Ok()) {
                                throw I2DException(_Std("Image format not supported"));
                        }

                        unsigned char* imgData = img.GetData();
                        TImageFile rgbLittleEndian(imgData, img.GetWidth(), img.GetHeight());
                        Inicializar(rgbLittleEndian, pICallback, debug, outputFormat);
                } else {
                        LiberarMemoria();
                        throw I2DException(cond.text());
                }
        }

        if (pICallback != NULL) {

                // Dumping DICOM Tags...
                DcmDataDictionary& globalDataDict = dcmDataDict.wrlock();
                DcmHashDictIterator iter(globalDataDict.normalBegin());
                DcmHashDictIterator end(globalDataDict.normalEnd());

                std::string keyStr;
                std::string descStr;
                std::string valStr;

                char keyFormat[12];

                for (; iter != end; ++iter) {

                        DcmTagKey tagkey = (*iter)->getKey();
                        //std::cout << "Checking " << keyStr << std::endl;
                        if (sprintf(keyFormat, "%04X|%04X", tagkey.getGroup(), tagkey.getElement()) > 8) {
                                //std::cout << "Asigning " << keyFormat << std::endl;
                                keyStr.assign(keyFormat);
                                descStr = (*iter)->getTagName();
                                OFString val;
                                OFCondition c = resultObject->findAndGetOFStringArray(tagkey, val);
                                if (c.good()) {
                                        valStr = val.c_str();
                                        pICallback->Inspect(keyStr, descStr, valStr);
                                }
                        }
                }
                dcmDataDict.wrunlock();
        }



        if (pICallback != NULL) {
                pICallback->Done();
        }
}

int DICOMImg2DCM::InsertarTagsPrivados(TipoPrivateTags& tags)
{
        return InsertarTagsPrivados(tags,i2d->getOverrideKeys());
}

int DICOMImg2DCM::InsertarTagsPrivados(TipoPrivateTags& tags, DcmDataset* dcmDataSet)
{
        //se busca el uid en el rango (GINKGO_GROUP,0010-00FF)
        unsigned int g=GINKGO_GROUP;
        unsigned int e=0;
        OFCondition cond;
        DcmElement* element;

        e=GetElementIdentifier(tags,dcmDataSet);

        if(e>0x00FF || e==0) {
                //no deberia llegar aqui
                std::cerr<<"error al almacenar los tags privados, todos los slots ocupados" <<std::endl;
                return 0;
        }

        //ya tenemos el e del modulo hay que hacer un desplazamiento de 8 bits a la izquierda para obtener el rango
        //si el elemento es 00xx el rango sera xx00-xxFF
        e = e << 8;

        unsigned int eTemp;
        TipoPrivateTags::ListaTags& ListaTags = tags.GetListaTags();
        for (TipoPrivateTags::ListaTags::iterator it = ListaTags.begin(); it != ListaTags.end(); ++it) {
                eTemp = e | (*it).first;
                element = (*it).second->ToElement(g,eTemp);
                if (element != NULL) {
                        cond = dcmDataSet->insert(element, true, false);
                        if (cond.bad()) {
                                std::cerr << "error al almacenar los tags privados, error al escribir en el dataset: (" << g << ","<<eTemp<<")" <<std::endl;
                                return 0;
                        }
                } else {
                        std::cerr << "error al almacenar los tags privados, error al crear el elemento: (" << g << ","<<eTemp<<")" <<std::endl;
                        return 0;
                }
        }

        return 1;
}

int DICOMImg2DCM::InsertarJerarquia(DicomDataset& base)
{
        if (i2d == NULL) {
                throw I2DException("El conversor no se ha inicializado previamente");
        }

        int val = InsertarJerarquia(base,i2d->getOverrideKeys(), NULL, NULL);
        return val;
};

int DICOMImg2DCM::InsertarJerarquia(DicomDataset& base,DcmDataset* dcmDataSet, DcmItem* itemPadre, DcmSequenceOfItems* seqPadre)
{

        int numTotalInsertados = 0;
        int numTagsInsertados  = 0;
        int numItemsInsertados = 0;
        int numSeqsInsertadas  = 0;

        // Insertamos todos los tags correspondientes a este nivel en la raiz (de haberla, si no: en el dataset).
        for (ListaTags::iterator it = base.tags.begin(); it != base.tags.end(); ++it) {
                DcmElement* e = this->CrearElementoConValor((*it).first.c_str());
                if (e != NULL) {
                        e->putString( wxString( (*it).second.c_str(), wxConvUTF8).mb_str(*m_pConv) );

                        OFCondition cond;
                        if (itemPadre == NULL) {
                                cond = dcmDataSet->insert(e, OFTrue);
                                char* str = NULL;
                                e->getString(str);
                                /*if(str!=NULL){
                                	std::cout << "raiz << " << e->getTag().toString() <<  "=" << str << std::endl;
                                }
                                else{
                                	std::cout << "raiz << " << e->getTag().toString() <<  "="  << std::endl;
                                }*/
                        } else {
                                cond = itemPadre->insert(e, OFTrue);
                                char* str = NULL;
                                e->getString(str);
                                /*if(str!=NULL){
                                	std::cout << itemPadre->getTag().toString().c_str() << " << " << e->getTag().toString() << " = " << str << std::endl;
                                }
                                else{
                                	std::cout << itemPadre->getTag().toString().c_str() << " << " << e->getTag().toString() << " = " << std::endl;
                                }*/


                        }
                        if (cond.bad()) {
                                std::cerr << "No se pudo insertar el elemento: (" << e->getTag().toString().c_str() << "): " << cond.text() << std::endl;
                        } else {
                                numTotalInsertados++;
                                numTagsInsertados++;
                        }
                }
        }

        // Insertamos todos los items correspondientes a este nivel en la raiz (de haberla, si no: en el dataset).

        for (auto it = base.items.begin(); it != base.items.end(); ++it) {

                DcmItem *item = new DcmItem();

                int nItems = InsertarJerarquia((*it),dcmDataSet, item, NULL);

                if (nItems > 0) {

                        if (seqPadre == NULL) {
                                //cond = dcmDataSet->insert(item, OFTrue);
                                std::cerr << "No se pudo insertar el item directamente a la raiz. "
                                          << "Deben insertarse en secuencias o en otros items.  "
                                          << nItems << " elementos perdidos: " << std::endl;
                                delete item;
                        } else {
                                OFCondition cond = seqPadre->insert(item, OFTrue);

                                if (cond.bad()) {
                                        std::cerr << "No se pudo insertar el item a la raiz.  "
                                                  << nItems << " elementos perdidos: " << cond.text() << std::endl;
                                        delete item;
                                } else {
                                        numTotalInsertados += nItems + 1;
                                        numItemsInsertados++;
                                }
                        }
                } else {
                        delete item;
                }
        }

        // Insertamos todas las secuencias correspondientes a este nivel en la raiz (de haberla, si no: en el dataset).
        for (DicomDataset::DatasetList::iterator it = base.secuencias.begin(); it != base.secuencias.end(); ++it) {

                std::string claveSecuencia = (*it).tagName;
                DicomDataset& nbase = (*it);

                unsigned int sg = 0xffff;
                unsigned int se = 0xffff;
                int sn = 0;

                sn = sscanf(claveSecuencia.c_str(), "%x|%x", &sg, &se);
                if (sn < 2) {
                        std::cerr << "Formato invalido (" << claveSecuencia.c_str() << "). Solo se soporta (FFFF|FFFF) como formato de tag para secuencias" << std::endl;
                        continue;
                }
                DcmTag stag(sg, se);
                if (stag.error() != EC_Normal) {
                        std::cerr << "Tag desconocido: " << claveSecuencia << std::endl;
                        continue;
                }
                DcmSequenceOfItems* seq = new DcmSequenceOfItems(stag);
                if (seq == NULL) {
                        std::cerr << "No se pudo crear la secuencia para el tag: " << claveSecuencia << std::endl;
                        continue;
                }

                int nItems = InsertarJerarquia(nbase,dcmDataSet, NULL, seq);

                if (nItems > 0) {

                        OFCondition cond;

                        if (seqPadre != NULL) {
                                DcmItem* item = new DcmItem();
                                cond = item->insert(seq);
                                seqPadre->insert(item);
                                //		std::cout << seqPadre->getTag().toString().c_str() << " << " << seq->getTag().toString() << std::endl;
                        } else if (itemPadre != NULL) {
                                cond = itemPadre->insert(seq, OFTrue);
                                //		std::cout << itemPadre->getTag().toString().c_str() << " << " << seq->getTag().toString() << std::endl;

                        } else {
                                cond = dcmDataSet->insert(seq, OFTrue);
                                //		std::cout << "raiz << " << seq->getTag().toString() << std::endl;
                        }

                        if (cond.bad()) {
                                std::cerr << "No se pudo insertar el item a la raiz.  " << nItems << " elementos perdidos: " << cond.text() << std::endl;
                                delete seq;
                        } else {
                                numTotalInsertados += nItems + 1;
                                numSeqsInsertadas++;
                        }
                } else {
                        delete seq;
                }
        }
        return numTotalInsertados;
}

void DICOMImg2DCM::ActualizarCampos(IInspectCallBack* pICallback)
{
        if (i2d == NULL) {
                throw I2DException("El conversor no se ha inicializado previamente");
        }
        if (pICallback != NULL) {
                //DcmDataset *overrideKeys = i2d->getOverrideKeys();

                DicomDataset jerarquiaAInsertar;
                pICallback->ObtenerJerarquiaInserccion(jerarquiaAInsertar);
                InsertarJerarquia(jerarquiaAInsertar,i2d->getOverrideKeys(), NULL, NULL);
        }
}

bool DICOMImg2DCM::Convertir(std::string& outputFile)
{

        if (i2d == NULL || inputPlug == NULL || outputPlug == NULL || resultObject == NULL) {
                throw I2DException("El conversor no se ha inicializado previamente");
        }
        if (outputFile.length() == 0) {
                throw I2DException("No se ha especificado el fichero de salida");
        }

        // Group length encoding mode for output DICOM file
        E_GrpLenEncoding grpLengthEnc = EGL_recalcGL;
        // Item and Sequence encoding mode for output DICOM file
        E_EncodingType lengthEnc = EET_ExplicitLength;
        // Padding mode for output DICOM file
        E_PaddingEncoding padEnc = EPD_noChange;
        // File pad length for output DICOM file
        unsigned int filepad = 0;
        // Item pad length for output DICOM file
        unsigned int itempad = 0;

        // Group Length Encoding:
        //   EGL_recalcGL  => recalculate group lengths if present
        //   EGL_withGL    => always write with group length elements
        //   EGL_withoutGL => always write without group length elements

        grpLengthEnc = EGL_recalcGL;

        // Length Encoding in Sequences and Items:
        //   EET_ExplicitLength  => write with explicit lengths
        //   EET_UndefinedLength => write with undefined lengths
        lengthEnc = EET_ExplicitLength;

        filepad = 0;
        itempad = 0;

        OFCondition cond;

        i2d->updateOverrideKeys(resultObject);
        //overrride charset
        {
                std::string strTmp;
                GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","DefaultCharset",strTmp,"ISO_IR 100");
                cond = resultObject->putAndInsertString(DCM_SpecificCharacterSet, strTmp.c_str());
                if (cond.bad()) {
                        throw I2DException(cond.text());
                }
        }

        DcmFileFormat dcmff(resultObject);
        cond = dcmff.saveFile(outputFile.c_str(), writeXfer, lengthEnc, grpLengthEnc, padEnc, filepad, itempad);
        if (cond.bad()) {
                throw I2DException(cond.text());
        }

#ifdef _GINKGO_TRACE
        {
                std::cout << "----DUMPING DCM----------------------------------------" << std::endl;
                // Dumping DICOM Tags...
                const DcmDataDictionary& globalDataDict = dcmDataDict.rdlock();
                DcmHashDictIterator iter(globalDataDict.normalBegin());
                DcmHashDictIterator end(globalDataDict.normalEnd());

                std::string keyStr;
                std::string descStr;
                std::string valStr;

                char keyFormat[12];

                for (; iter != end; ++iter) {

                        DcmTagKey tagkey = (*iter)->getKey();
                        //std::cout << "Checking " << keyStr << std::endl;
                        if (sprintf(keyFormat, "%04X|%04X", tagkey.getGroup(), tagkey.getElement()) > 8) {
                                //std::cout << "Asigning " << keyFormat << std::endl;
                                keyStr.assign(keyFormat);
                                descStr = (*iter)->getTagName();
                                OFString val;
                                OFCondition c = resultObject->findAndGetOFStringArray(tagkey, val);
                                if (c.good()) {
                                        valStr = val.c_str();
                                        std::cout << "[" << keyStr.c_str() << "] " << valStr.c_str() << " // " << descStr.c_str() << std::endl;
                                }
                        }
                }
                dcmDataDict.unlock();
                std::cout << "-------------------------------------------------------" << std::endl;
        }
#endif
        return true;
}

//devuelve el element identifier donde el modulo puede escribir sus tags privados
unsigned int DICOMImg2DCM::GetElementIdentifier(GIL::DICOM::TipoPrivateTags& tags, DcmDataset* dataset)
{
        OFCondition cond;
        DcmElement* element;
        unsigned int g=GINKGO_GROUP;
        unsigned int e=0x0010;

        for(; e<=0x00FF; e++) {
                DcmTagKey key(g,e);
                cond = dataset->findAndGetElement(key,element,false);
                if(element==NULL) {
                        //esta libre el slot
                        //se introduce el uid
                        DcmTagKey key(g,e);
                        DcmTag tag(key);
                        //tipo short string!!!!
                        DcmVR vr(EVR_SH);
                        tag.setVR(vr);
                        if (tag.error() != EC_Normal) {
                                std::cerr << "error al almacenar los tags privados, tag desconocido: (" << g << ","<<e<<")" <<std::endl;
                                return 0;
                        }

                        element = newDicomElement(tag);

                        if(element==NULL) {
                                std::cerr << "error al almacenar los tags privados, error al crear el elemento uid: (" << g << ","<<e<<")" <<std::endl;
                                return 0;
                        }

                        cond=element->putString(tags.UIDModulo.c_str());

                        if (cond.bad()) {
                                std::cerr << "error al almacenar los tags privados, error al escribir el uid: (" << g << ","<<e<<")" <<std::endl;
                                return 0;
                        }

                        cond = dataset->insert(element, true, false);
                        break;
                }

                if(element!=NULL) {
                        char* cadena;
                        std::string ov;

                        cond = element->getString(cadena);
                        if (cond.good()) {
                                ov = std::string(cadena);
                        } else {
                                continue;
                        }

                        if(ov==tags.UIDModulo) {
                                //estamos en el g y e del modulo deseado
                                break;
                        }
                }
        }

        return e;
}

bool DICOMImg2DCM::CrearSRDoc(std::string& outputFile, DicomDataset& base, std::list<GNC::GCS::Ptr<TipoPrivateTags> >& tagsPrivados)
{
        DSRDocument *doc = new DSRDocument();
        if (doc != NULL) {
                {
                        std::string strTmp;
                        GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","DefaultCharset",strTmp,"ISO_IR 100");
                        doc->setSpecificCharacterSet(strTmp.c_str());
                }
                doc->createNewDocument(DSRTypes::DT_BasicTextSR);
                doc->setSpecificCharacterSetType(DSRTypes::CS_UTF8);

                doc->setManufacturer("Ginkgo");
                doc->setPatientName("Last Name^First Name");
                doc->setPatientSex("O");
                doc->setReferringPhysicianName("Last Name^First Name");

                doc->getTree().addContentItem(DSRTypes::RT_isRoot, DSRTypes::VT_Container);

                DcmFileFormat *fileformat = new DcmFileFormat();

                DcmDataset *dataset = NULL;

                OFCondition cond;

                if (fileformat != NULL)
                        dataset = fileformat->getDataset();
                if (dataset != NULL) {
                        cond = doc->write(*dataset);
                        if (cond.good()) {
                                //utf-8
                                /*if( dataset->putAndInsertOFStringArray(DCM_SpecificCharacterSet, "ISO_IR 100").bad()) {
                                	LOG_ERROR("Dicomizacion", "Error al establecer la codificacion en el fichero DICOM");
                                }*/
                                InsertarJerarquia(base,dataset,NULL,NULL);
                                for(std::list<GNC::GCS::Ptr<TipoPrivateTags> >::iterator it = tagsPrivados.begin(); it!= tagsPrivados.end(); ++it) {
                                        InsertarTagsPrivados(*(*it),dataset);
                                }
                                //SE ESCRIBE LOS TAGS DEL SR...

                                cond = fileformat->saveFile(outputFile.c_str(), EXS_LittleEndianExplicit);
                        }
                }
                delete fileformat;

                delete doc;

                if (cond.good()) {
                        return true;
                } else {
                        return false;
                }
        }
        return false;
}
}
}
