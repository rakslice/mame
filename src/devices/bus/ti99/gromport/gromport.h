// license:LGPL-2.1+
// copyright-holders:Michael Zapf
/***************************************************************************
    Gromport (Cartridge port) of the TI-99 consoles
    For details see gromport.cpp

    Michael Zapf
***************************************************************************/

#ifndef MAME_BUS_TI99_GROMPORT_GROMPORT_H
#define MAME_BUS_TI99_GROMPORT_GROMPORT_H

#pragma once

#include "bus/ti99/ti99defs.h"

namespace bus { namespace ti99 { namespace gromport {

struct pcb_type
{
	int id;
	const char* name;
};

class ti99_cartridge_device;
class cartridge_connector_device;

class gromport_device : public device_t, public device_slot_interface
{
public:
	gromport_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	DECLARE_READ8Z_MEMBER(readz);
	DECLARE_WRITE8_MEMBER(write);
	DECLARE_READ8Z_MEMBER(crureadz);
	DECLARE_WRITE8_MEMBER(cruwrite);
	DECLARE_WRITE_LINE_MEMBER(ready_line);
	DECLARE_WRITE_LINE_MEMBER(romgq_line);
	void set_gromlines(line_state mline, line_state moline, line_state gsq);
	DECLARE_WRITE_LINE_MEMBER(gclock_in);

	void set_mask(int mask) { m_mask = mask; }

	template <class Object> devcb_base &set_ready_callback(Object &&cb)  { return m_console_ready.set_callback(std::forward<Object>(cb)); }
	template <class Object> devcb_base &set_reset_callback(Object &&cb) { return m_console_reset.set_callback(std::forward<Object>(cb)); }

	void    cartridge_inserted();
	bool    is_grom_idle();

	auto ready_cb() { return m_console_ready.bind(); }
	auto reset_cb() { return m_console_reset.bind(); }
	void configure_slot(bool for998);

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual void device_config_complete() override;
	virtual ioport_constructor device_input_ports() const override;

private:
	cartridge_connector_device*    m_connector;
	bool                m_reset_on_insert;
	devcb_write_line   m_console_ready;
	devcb_write_line   m_console_reset;
	int             m_mask;
	int m_romgq;
};

class cartridge_connector_device : public device_t
{
public:
	virtual DECLARE_READ8Z_MEMBER(readz) = 0;
	virtual DECLARE_WRITE8_MEMBER(write) = 0;
	virtual DECLARE_SETADDRESS_DBIN_MEMBER( setaddress_dbin ) { }

	virtual DECLARE_READ8Z_MEMBER(crureadz) = 0;
	virtual DECLARE_WRITE8_MEMBER(cruwrite) = 0;

	virtual DECLARE_WRITE_LINE_MEMBER(romgq_line) = 0;
	virtual void set_gromlines(line_state mline, line_state moline, line_state gsq) =0;

	virtual DECLARE_WRITE_LINE_MEMBER(gclock_in) = 0;

	DECLARE_WRITE_LINE_MEMBER(ready_line);

	virtual void insert(int index, bus::ti99::gromport::ti99_cartridge_device* cart) { m_gromport->cartridge_inserted(); }
	virtual void remove(int index) { }
	virtual bool is_grom_idle() = 0;

protected:
	cartridge_connector_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);
	virtual void device_config_complete() override;

	gromport_device*    m_gromport;
	bool     m_grom_selected;
};

} } } // end namespace bus::ti99::gromport

DECLARE_DEVICE_TYPE_NS(TI99_GROMPORT, bus::ti99::gromport, gromport_device)

#endif // MAME_BUS_TI99_GROMPORT_GROMPORT_H
