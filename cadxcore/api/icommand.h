/*
 *
 *  $Id: icommand.h $
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
#include <api/controllers/iprogresscontroller.h>
#include "iproxynotificadorprogreso.h"

//---------------------------------------------------------------------------------------
//region Forward declarations
namespace GNC {
	class CommandLauncher;
	class CommandController;
	class INotificadorProgreso;

	namespace GCS {
		class IComando;
		class ICommandController;
	}
}
//endregion

namespace GNC {
	namespace GCS {

		//---------------------------------------------------------------------------------------
		class EXTAPI IComandoParams {
		public:
			IComandoParams();
			virtual ~IComandoParams();
		};

		//---------------------------------------------------------------------------------------
		class EXTAPI IComandoResult {
		public:
			IComandoResult();
			virtual ~IComandoResult();
		};


		//---------------------------------------------------------------------------------------
		class EXTAPI IComando : public GNC::IProxyNotificadorProgreso {
			//---------------------------------------------------------------------------------------
			//region Constructor y destructor
			typedef std::list< int> TipoListaIdComandos;

		public:

			IComando(IComandoParams* pParams, const std::string& name = "Comando", void* pOwner = NULL);

			virtual ~IComando();

			void Free();

			//endregion

			//---------------------------------------------------------------------------------------
			//region Interfaz especifica
		protected:

			virtual void Execute() = 0;

			virtual void Update() = 0;

		public:

			/** Notifica el progreso. Si devuelve true, el ejecutor dejara de procesarse **/
			virtual bool NotificarProgreso(float progresoNormalizado, const std::string& texto);
		protected:

			/** Callback que puede ser sobrecargado por el sujeto para incluir acciones especificas cuando se aborta el comando **/
			virtual void OnAbort();

			/** Callback que puede ser sobrecargado por el sujeto para incluir acciones especificas cuando se destruye el comando **/
			virtual void OnDestroy();

			//endregion

			//---------------------------------------------------------------------------------------
			//region Getters y helpers
		public:

			int GetId() const;

			void SetId(int id);

			void SetOwner(void* pOwner);

			void* GetOwner() const;

			const std::string& GetName() const;

			IComandoParams* GetParams();

			void Abort() ;

			bool EstaAbortado() const;

			//endregion

			//---------------------------------------------------------------------------------------
			//region Notificacion
			GNC::INotificadorProgreso* GetNotificadorProgreso();
		private:
			// Este metodo debe ser invocado solo por el lanzador de comandos.
			void SetNotificadorProgreso(GNC::INotificadorProgreso* pNotificador);
			//endregion
			

			//---------------------------------------------------------------------------------------
			//region "Gestion de Dependencias"

		protected:

			void EsperaA(int idDep);

			void AbortaSi(int idConflicto);

			void CancelaA(int idReemplazo);

			TipoListaIdComandos   m_Dependencias;
			TipoListaIdComandos   m_Conflictos;
			TipoListaIdComandos   m_Reemplazos;
			//endregion


			//---------------------------------------------------------------------------------------
			//region "Atributos"

		protected:
			IComandoParams*            m_pParams;
			IComandoResult*            m_pResult;

		private:
			int                        m_Id;
			std::string                m_Nombre;
			bool                       m_Abortado;
			void*                      m_pOwner;
			GNC::INotificadorProgreso* m_pNotificadorProgreso;

			friend class GNC::CommandLauncher;
			friend class GNC::CommandController;
			//endregion
		};

		//deprecated, persistent commands are deprecated
		class EXTAPI IPersistentCommand: public GNC::GCS::IComando
		{
		public:
			typedef enum {
				Pending = 0,
				Ok,
				Error
			} TStatus;

			IPersistentCommand(const std::list<long> seriesId, const std::string description);
			IPersistentCommand(long idBBDD, const std::list<long> seriesId, const std::string description);
			
			virtual ~IPersistentCommand();
			virtual std::string serialize() = 0;
			virtual void init(const std::string& serializedValue) = 0;
			virtual std::string getClassUID();
			long getIdBBDD();
			void setIdBBDD(long id);
			const std::list<long>& getSeriesId();
			const std::string& getErrorDescription();
			TStatus getStatus();
			const std::string& getDescription();

		protected:
			void setStatus(TStatus status);
			TStatus m_status;
			std::string m_description;
			std::list<long> m_seriesId;
			long m_idBBDD;
		
			std::string m_errorDescription;
		};
	}
}
