/*
*  
*  $Id: ievento.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "ievento.h"
#include <wx/event.h>

GNC::GCS::Events::IEvent::IEvent(long codigoEvento, long codigoAmbito, long prioridad, GNC::GCS::IVista* pVista)
{
	m_codigoEvento = codigoEvento;
	m_codigoAmbito = codigoAmbito;
	m_pVista = pVista;
	m_skip = true;
	m_prioridad=prioridad;
}

GNC::GCS::Events::IEvent::~IEvent()
{
}

bool GNC::GCS::Events::IEvent::IsEqual(const IEvent& evt) const
{
	if(evt.GetCodigoEvento() == m_codigoEvento && evt.GetCodigoAmbito() == m_codigoAmbito && evt.GetVista() == m_pVista){
		return true;
	}
	else{
		return false;
	}
}

bool GNC::GCS::Events::IEvent::IsSkipped() const
{
	return m_skip;
}

void GNC::GCS::Events::IEvent::Skip(bool skip)
{
	m_skip=skip;
}

long GNC::GCS::Events::IEvent::GetCodigoEvento() const
{
	return m_codigoEvento;
}

long GNC::GCS::Events::IEvent::GetCodigoAmbito() const
{
	return m_codigoAmbito;
}

void GNC::GCS::Events::IEvent::SetAmbito(long codigoAmbito){
	m_codigoAmbito = codigoAmbito;
}

GNC::GCS::IVista* GNC::GCS::Events::IEvent::GetVista() const
{
	return m_pVista;
}

void GNC::GCS::Events::IEvent::SetVista(GNC::GCS::IVista* pVista)
{
	m_pVista = pVista;
}

long GNC::GCS::Events::IEvent::GetPrioridad() const
{
	return m_prioridad;
}

void GNC::GCS::Events::IEvent::SetPrioridad(long prioridad)
{
	m_prioridad = prioridad;
}

void GNC::GCS::Events::IEvent::SetNombre(const char* nombre)
{
	m_Nombre = nombre;
}

void GNC::GCS::Events::IEvent::SetNombre(std::string& nombre)
{
	m_Nombre = nombre;
}

const std::string& GNC::GCS::Events::IEvent::GetNombre() const
{
	return m_Nombre;
}


void GNC::GCS::Events::IEvent::pushInfo(std::ostream& ) const {
}

//====================================================================================================
//= Interfaz de evento de raton
//====================================================================================================


GNC::GCS::Events::EventoRaton::EventoRaton(wxMouseEvent* evt, Contexto3D* pContexto)
{
	c = pContexto;
	m_left = m_middle = m_right = m_any = false;
	m_leftDown = m_middleDown = m_rightDown = m_anyDown = false;
	m_tipo = ET_UNKNOWN;
	m_controlDown = m_shiftDown = m_altDown = m_metaDown = false;
	m_wheelRotation = m_wheelDelta = m_linesPerAction = 0;
	m_skip = true;
	sX = sY = 0;
	ParseWXEvent(evt);
}

GNC::GCS::Events::EventoRaton::~EventoRaton()
{

}

bool GNC::GCS::Events::EventoRaton::ButtonDown(Boton but) const
{
	if (m_tipo == ET_DOWN) {
		switch (but) {
			case EB_CUALQUIERA:
				return m_any;
			case EB_IZQUIERDO:
				return m_left;
			case EB_DERECHO:
				return m_right;
			case EB_CENTRAL:
				return m_middle;
			default:
				return false;
		}
	}
	else {
		return false;
	}
}

bool GNC::GCS::Events::EventoRaton::ButtonDown(const GNC::GCS::TriggerButton& button) const
{
	if (m_tipo == ET_DOWN) {
		return (m_left && button.IsLeftEnabled()) || (m_right && button.IsRightEnabled()) || (m_middle && button.IsMiddleEnabled());
	}
	else {
		return false;
	}
}

bool GNC::GCS::Events::EventoRaton::ButtonDClick(Boton but) const
{
	if (m_tipo == ET_DCLICK) {
		switch (but) {
			case EB_CUALQUIERA:
				return m_any;
			case EB_IZQUIERDO:
				return m_left;
			case EB_DERECHO:
				return m_right;
			case EB_CENTRAL:
				return m_middle;
			default:
				return false;
		}
	}
	else {
		return false;
	}
}

bool GNC::GCS::Events::EventoRaton::ButtonDClick(const GNC::GCS::TriggerButton& button) const
{
	if (m_tipo == ET_DCLICK) {
		return (m_left && button.IsLeftEnabled()) || (m_right && button.IsRightEnabled()) || (m_middle && button.IsMiddleEnabled());
	}
	else {
		return false;
	}
}

bool GNC::GCS::Events::EventoRaton::ButtonUp(Boton but) const
{
	if (m_tipo == ET_UP) {
		switch (but) {
			case EB_CUALQUIERA:
				return m_any;
			case EB_IZQUIERDO:
				return m_left;
			case EB_DERECHO:
				return m_right;
			case EB_CENTRAL:
				return m_middle;
			default:
				return false;
		}
	}
	else {
		return false;
	}
}

bool GNC::GCS::Events::EventoRaton::ButtonUp(const GNC::GCS::TriggerButton& button) const
{
	if (m_tipo == ET_UP) {
		return (m_left && button.IsLeftEnabled()) || (m_right && button.IsRightEnabled()) || (m_middle && button.IsMiddleEnabled());
	}
	else {
		return false;
	}
}

bool GNC::GCS::Events::EventoRaton::Button(Boton but) const
{
	switch (but) {
		case EB_CUALQUIERA:
			return m_any;
		case EB_IZQUIERDO:
			return m_left;
		case EB_DERECHO:
			return m_right;
		case EB_CENTRAL:
			return m_middle;
		default:
			return false;
	}
}

bool GNC::GCS::Events::EventoRaton::Button(const GNC::GCS::TriggerButton& button) const
{
	return  (m_left && button.IsLeftEnabled()) || (m_right && button.IsRightEnabled()) || (m_middle && button.IsMiddleEnabled());					
}

bool GNC::GCS::Events::EventoRaton::ButtonIsDown(Boton but) const
{
	if (m_anyDown) {
		switch (but) {
			case EB_CUALQUIERA:
				return m_any;
			case EB_IZQUIERDO:
				return m_left;
			case EB_DERECHO:
				return m_right;
			case EB_CENTRAL:
				return m_middle;
			default:
				return false;
		}
	}
	else {
		return false;
	}
}

bool GNC::GCS::Events::EventoRaton::ButtonIsDown(const GNC::GCS::TriggerButton& button) const
{
	return m_anyDown && ( (m_left && button.IsLeftEnabled()) || (m_right && button.IsRightEnabled()) || (m_middle && button.IsMiddleEnabled()) );
}

bool GNC::GCS::Events::EventoRaton::ControlDown() const
{ return m_controlDown; }

bool GNC::GCS::Events::EventoRaton::MetaDown() const
{ return m_metaDown; }

bool GNC::GCS::Events::EventoRaton::AltDown() const
{
	return m_altDown;
}

bool GNC::GCS::Events::EventoRaton::ShiftDown() const
{
	return m_shiftDown;
}

bool GNC::GCS::Events::EventoRaton::LeftDown() const
{
	return (m_tipo == ET_DOWN && m_left);
}

bool GNC::GCS::Events::EventoRaton::MiddleDown() const
{
	return (m_tipo == ET_DOWN && m_middle);
}

bool GNC::GCS::Events::EventoRaton::RightDown() const
{
	return (m_tipo == ET_DOWN && m_right);
}

bool GNC::GCS::Events::EventoRaton::LeftUp() const
{
	return (m_tipo == ET_UP && m_left);
}

bool GNC::GCS::Events::EventoRaton::MiddleUp() const
{
	return (m_tipo == ET_UP && m_middle);
}

bool GNC::GCS::Events::EventoRaton::RightUp() const
{
	return (m_tipo == ET_UP && m_right);
}

bool GNC::GCS::Events::EventoRaton::LeftDClick() const
{
	return (m_tipo == ET_DCLICK && m_left);
}

bool GNC::GCS::Events::EventoRaton::MiddleDClick() const
{
	return (m_tipo == ET_DCLICK && m_middle);
}

bool GNC::GCS::Events::EventoRaton::RightDClick() const
{
	return (m_tipo == ET_DCLICK && m_right);
}

bool GNC::GCS::Events::EventoRaton::LeftIsDown() const
{
	return m_leftDown;
}

bool GNC::GCS::Events::EventoRaton::MiddleIsDown() const
{
	return m_middleDown;
}

bool GNC::GCS::Events::EventoRaton::RightIsDown() const
{
	return m_rightDown;
}

bool GNC::GCS::Events::EventoRaton::AnyIsDown() const
{
	return m_anyDown;
}

bool GNC::GCS::Events::EventoRaton::Dragging() const
{
	return (m_tipo == ET_MOTION) && ButtonIsDown(EB_CUALQUIERA);
}

bool GNC::GCS::Events::EventoRaton::Moving() const
{
	return (m_tipo == ET_MOTION) && !ButtonIsDown(EB_CUALQUIERA);
}

bool GNC::GCS::Events::EventoRaton::Entering() const
{
	return (m_tipo == ET_ENTER);
}

bool GNC::GCS::Events::EventoRaton::Leaving() const
{
	return (m_tipo == ET_LEAVE);
}

int GNC::GCS::Events::EventoRaton::GetWheelRotation() const
{
	return m_wheelRotation;
}

int GNC::GCS::Events::EventoRaton::GetWheelDelta() const
{
	return m_wheelDelta;
}

int GNC::GCS::Events::EventoRaton::GetLinesPerAction() const
{
	return m_linesPerAction;
}

#ifdef _WINDOWS
bool GNC::GCS::Events::EventoRaton::IsPageScroll() const
{
	return ((unsigned int)m_linesPerAction == UINT_MAX);
}
#else
bool GNC::GCS::Events::EventoRaton::IsPageScroll() const
{
	return ((unsigned int)m_linesPerAction == std::numeric_limits<unsigned int>::max());
}
#endif

void GNC::GCS::Events::EventoRaton::Skip(bool skip)
{
	m_skip = skip;
}

bool GNC::GCS::Events::EventoRaton::GetSkipped() const
{
	return m_skip;
}

void GNC::GCS::Events::EventoRaton::ParseWXEvent(wxMouseEvent* evt)
{
	wP.x = (double) evt->m_x;
	wP.y = (double) evt->m_y;
	wP.z = 0.0f;

	m_leftDown   = evt->m_leftDown;
	m_middleDown = evt->m_middleDown;
	m_rightDown  = evt->m_rightDown;
	m_anyDown    = m_leftDown || m_middleDown || m_rightDown;
	int etype = evt->GetEventType();

	if (etype == wxEVT_MOTION) {
		m_tipo = ET_MOTION;
	}
	else if (etype == wxEVT_LEFT_DOWN) {
		m_tipo = ET_DOWN;
		m_left = true;
	}
	else if (etype == wxEVT_LEFT_UP) {
		m_tipo = ET_UP;
		m_left = true;
	}
	else if (etype == wxEVT_LEFT_DCLICK) {
		m_tipo = ET_DCLICK;
		m_left = true;
	}
	else if (etype == wxEVT_MIDDLE_DOWN) {
		m_tipo = ET_DOWN;
		m_middle = true;
	}
	else if (etype == wxEVT_MIDDLE_UP) {
		m_tipo = ET_UP;
		m_middle = true;
	}
	else if (etype == wxEVT_MIDDLE_DCLICK) {
		m_tipo = ET_DCLICK;
		m_middle = true;
	}
	else if (etype == wxEVT_RIGHT_DOWN) {
		m_tipo = ET_DOWN;
		m_right = true;
	}
	else if (etype == wxEVT_RIGHT_UP) {
		m_tipo = ET_UP;
		m_right = true;
	}
	else if (etype == wxEVT_RIGHT_DCLICK) {
		m_tipo = ET_DCLICK;
		m_right = true;
	}
	else if (etype == wxEVT_ENTER_WINDOW) {
		m_tipo = ET_ENTER;
	}
	else if (etype == wxEVT_LEAVE_WINDOW) {
		m_tipo = ET_LEAVE;
	}
	else {
		m_tipo = ET_UNKNOWN;
	}

	m_any = m_anyDown || m_left || m_right || m_middle;

	m_controlDown    = evt->m_controlDown;
	m_shiftDown      = evt->m_shiftDown;
	m_altDown        = evt->m_altDown;
	m_metaDown       = evt->m_metaDown;

	m_wheelRotation  = evt->m_wheelRotation;
	m_wheelDelta     = evt->m_wheelDelta;
	m_linesPerAction = evt->m_linesPerAction;
	sX               = evt->m_x;
	sY               = evt->m_y;

}

//====================================================================================================
//= Interfaz de evento de teclado
//====================================================================================================

GNC::GCS::Events::EventoTeclado::EventoTeclado(wxKeyEvent* evt, Contexto3D* pContexto)
{
	c = pContexto;
	m_keyCode = 0;

	m_controlDown = false;
	m_shiftDown = false;
	m_altDown = false;
	m_metaDown = false;
	
	m_uniChar = L'\0';

	m_rawCode = 0;
	m_rawFlags = 0;

	m_skip = true;

	ParseWXEvent(evt);
}

GNC::GCS::Events::EventoTeclado::~EventoTeclado()
{

}

bool GNC::GCS::Events::EventoTeclado::ControlDown() const
{
	return m_controlDown;
}

bool GNC::GCS::Events::EventoTeclado::ShiftDown() const
{
	return m_shiftDown;
}

bool GNC::GCS::Events::EventoTeclado::MetaDown() const
{
	return m_metaDown;
}

bool GNC::GCS::Events::EventoTeclado::AltDown() const
{
	return m_altDown;
}

bool GNC::GCS::Events::EventoTeclado::HasModifiers() const
{
	return ControlDown() || AltDown();
}

int GNC::GCS::Events::EventoTeclado::GetKeyCode() const
{
	return (int)m_keyCode;
}

wchar_t GNC::GCS::Events::EventoTeclado::GetUnicodeKey() const
{
	return m_uniChar;
}

unsigned int GNC::GCS::Events::EventoTeclado::GetRawKeyCode() const
{
	return m_rawCode;
}

unsigned int GNC::GCS::Events::EventoTeclado::GetRawKeyFlags() const
{
	return m_rawFlags;
}

void GNC::GCS::Events::EventoTeclado::Skip(bool skip)
{
	m_skip = skip;
}

bool GNC::GCS::Events::EventoTeclado::GetSkipped() const
{
	return m_skip;
}

void GNC::GCS::Events::EventoTeclado::ParseWXEvent(wxKeyEvent* evt)
{
	m_isKeyDown = evt->GetEventType() == wxEVT_KEY_DOWN;
	m_keyCode     = evt->m_keyCode;
	m_controlDown = evt->m_controlDown;
	m_shiftDown   = evt->m_shiftDown;
	m_altDown     = evt->m_altDown;
	m_metaDown    = evt->m_metaDown;
	
	m_rawCode     = evt->m_rawCode;
	m_rawFlags    = evt->m_rawFlags;
	m_uniChar     = evt->m_uniChar;
}


//====================================================================================================
//= Interfaz de subscriptor de ventos
//====================================================================================================

GNC::GCS::Events::ISubscriptorEventos::ISubscriptorEventos()
{
}

GNC::GCS::Events::ISubscriptorEventos::~ISubscriptorEventos()
{
}

// Propagacion de eventos de raton
void GNC::GCS::Events::ISubscriptorEventos::OnMouseEvents(GNC::GCS::Events::EventoRaton&)
{
}

// Propagacion de eventos de teclado
void GNC::GCS::Events::ISubscriptorEventos::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
}

//====================================================================================================
//= Interfaz de publicacion de eventos
//====================================================================================================

GNC::GCS::Events::IPublicadorEventos::IPublicadorEventos()
{
}

GNC::GCS::Events::IPublicadorEventos::~IPublicadorEventos()
{
}

void GNC::GCS::Events::IPublicadorEventos::InsertarListener(GNC::GCS::Events::ISubscriptorEventos* pListener)
{
	IteradorListaListeners it;
	for (it = m_Listeners.begin(); it != m_Listeners.end() && (*it) != pListener; ++it);
	if (it == m_Listeners.end()) {
		m_Listeners.push_back(pListener);
	}
}

void GNC::GCS::Events::IPublicadorEventos::EliminarListener(GNC::GCS::Events::ISubscriptorEventos* pListener)
{
	IteradorListaListeners it;
	for (it = m_Listeners.begin(); it != m_Listeners.end() && (*it) != pListener; ++it);
	if (it != m_Listeners.end()) {
		m_Listeners.erase(it);
	}
}

void GNC::GCS::Events::IPublicadorEventos::PropagarEvento(GNC::GCS::Events::EventoRaton& evt)
{
	for (IteradorListaListeners it = m_Listeners.begin(); it != m_Listeners.end(); ++it) {
		(*it)->OnMouseEvents(evt);
	}
}

void GNC::GCS::Events::IPublicadorEventos::PropagarEvento(GNC::GCS::Events::EventoTeclado& evt)
{
	for (IteradorListaListeners it = m_Listeners.begin(); it != m_Listeners.end(); ++it) {
		(*it)->OnKeyEvents(evt);
	}
}