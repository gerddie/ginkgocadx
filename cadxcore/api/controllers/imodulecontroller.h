/*
 *  
 *  $Id: imodulecontroller.h $
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

#include <string>
#include <list>
#include <vector>
#include <map>
#include <exception>

#include <api/api.h>
#include <api/imgproxy.h>
#include <api/autoptr.h>
#include <api/controllers/ihistorycontroller.h>
#include <api/imodelointegracion.h>
#include <wx/bitmap.h>

class IPasoConfiguracion;
class IDialogoConfiguracion;
class IWizard;
class IPasoWizard;
class wxWindow;

namespace GNC {
	namespace GCS {
		class IEntorno;
		class IVista;
	}
}

class IPACSController;

#define GINKGO_PARAMS_SEPARATOR "="

namespace GNC {
	namespace GCS {
		class HangingProtocol;

		/* Appender / formatter para reordenar y/o añadir tags DICOM necesarios por el controlador o vista en la importacion */
		class EXTAPI TagsDICOM {
		public:
			typedef std::list<std::string> ListaArbolesPersonalizados;
			typedef std::map<std::string, int> MapaTags;

			// Vector de nombres de raices de tags DICOM que el modo necesita
			ListaArbolesPersonalizados ArbolesPersonalizados;
			// Mapa de tags a insertar/sobreescribir con sus id de raices asociadas (donde se insertaran)
			MapaTags TagsAInsertar;

		};

		class EXTAPI ModoControlador {
			public:
				typedef std::list<std::string> TipoListaModalidades;
				typedef std::list<std::string> TipoListaUIDsImportacion;
				typedef enum {
					THSL_FULL,
					THSL_ONLY_REGULAR,
					THSL_ONLY_1x1
				} T_HangingProtocolSupportLevel;

				ModoControlador(int id, const std::string& descripcion, const TipoListaModalidades& listaModalidades, const TipoListaUIDsImportacion& listaImportacion, wxBitmap * bitmap = NULL, T_HangingProtocolSupportLevel supportLevel = THSL_ONLY_1x1);

			virtual ~ModoControlador();

			const TipoListaModalidades GetModalidades();

			const std::string& GetDescripcion();

			wxBitmap* GetBitmap();

			int GetId() const;

			T_HangingProtocolSupportLevel GetHangingSupportLevel() const;

			virtual bool SupportsModalityFile(const std::string &modalidad, const std::string& transferSyntax) const;

			virtual bool SupportsImportationUID(const std::string &importationUID) const;

		protected:
			int id;
			std::string descripcion;
			TipoListaModalidades  m_listaModalidades;
			T_HangingProtocolSupportLevel m_hangingSupportLevel;
			
			TipoListaUIDsImportacion m_listImportationUIDs;
			wxBitmap* bitmap;
		};

		class EXTAPI IdHL7 {
		public:
			std::string etiqueta;        // Etiqueta que usa Ginkgo para localizar el codigo
			std::string codigo;          // Codigo formal para la etiqueta
			std::string namespaceid;     // NamespaceID de la entidad asignatoria
			std::string descripcion;     // Descripcion del codigo
			std::string tipoobservacion; // Tipo de observacion. Solo se usa para Ids transportados en OBX
			std::string tipodatos;
			std::string version;			  //version of the id

			IdHL7();

			IdHL7(const IdHL7& o);

			IdHL7(const std::string& e, const std::string& c, const std::string& d, const std::string& n, const std::string& t, const std::string& td, const std::string& v = "");

			IdHL7& operator = (const IdHL7& o);

			operator const std::string () const;
		};

		/** Module controller. Responsable de proveer una factoría de controladores de estudios y registrarlos mediante el controlador de vistas. **/
		class EXTAPI IModuleController {
		public:
			typedef struct TPrivateTagReemplazable
			{
				typedef enum {STD, XML} TType;
				unsigned char id;
				TType type;
				TPrivateTagReemplazable (unsigned char _id, TType _type) : id(_id), type(_type)
				{
				}
			} TPrivateTagReemplazable;
			typedef std::list<TPrivateTagReemplazable> TListaIdsTagsPrivados;
			//first element uid provider, second list of tags
			typedef std::map<std::string, TListaIdsTagsPrivados> TMapaTagsPrivados;

			typedef std::list<IdHL7> TipoListaIdsHL7;
			typedef std::map<std::string,IdHL7> TipoMapaIdsHL7;
			typedef std::map<std::string, std::string> TExtraLicensesMap;

			/* Constructor */
			IModuleController(GNC::GCS::IEntorno* pEntorno, const std::string& uid, int priority);

			/* Destructor */
			virtual ~IModuleController();

			/* Obtiene el UID único del módulo */
			const std::string& GetUID() const;

			virtual bool SoportaImportacion() const;

			//si importa series true, si importa estudios false
			virtual bool ImportaSeries() const;

			const std::string& GetImporterDescription() const;
			virtual wxBitmap GetImporterIcon() const;


			virtual void GetPasosImportacion(IWizard* , std::list<IPasoWizard*> &, wxWindow* , std::string &, GNC::GCS::Ptr<GIL::IModeloIntegracion> );
			virtual void GetPasosConfiguracion( std::list<IPasoConfiguracion*> &, wxWindow* , IDialogoConfiguracion* ) const;

			// Metodos de inicializacion de configuracion del modulo
			void RegistrarConfiguracion();

			//registra las vistas
			virtual void RegistrarVistas() = 0;

			const IdHL7 GetIdInforme(const std::string& key) const;

			/* Abre un estudio de manera no interactiva desde un directorio y devuelve su controlador específico */
			virtual GNC::GCS::IVista* OpenView(int modo, GNC::GCS::IHistoryController::SeriesModelList& series, GNC::GCS::Ptr<GNC::GCS::HangingProtocol> hangingProtocol) = 0;


			typedef std::vector<ModoControlador*> ListaModos;
			typedef std::vector<ModoControlador*>::iterator IteradorListaModos;

			virtual ModoControlador* GetModo(int id);

			virtual ListaModos& GetListaModos();

			virtual int GetPriority();

			virtual void GetTagsPrivadosReplace(GNC::GCS::IModuleController::TMapaTagsPrivados& mapaTags);
			virtual bool SupportsImportationUID(const std::string &importationUID);
			virtual bool MakeThumbnail(const GNC::GCS::IHistoryController::FileModel& model, ImgProxy<UCHAR3>& thumbnail);
			virtual TExtraLicensesMap GetExtraLicenses();

		protected:
			GNC::GCS::IEntorno*  m_pEntorno;
			ListaModos           m_ListaModos;

			TipoListaIdsHL7      m_ListaInformes;

			std::string          m_UID;
			std::string          m_ImporterDescription;
			int		             m_priority;
		};
	}
}
