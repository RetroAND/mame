#include "emu.h"
#include "cpu/m6502/w65c02s.h"
#include "machine/6522via.h"


namespace
{
	class cr124_state : driver_device
	{
		public:

			cr124_state(const machine_config &mconfig, device_type type, const char *tag)
				: driver_device(mconfig, type, tag),
				m_maincpu(*this,"maincpu"),
				m_rom(*this,"rom"),
				m_ram(*this,"ram"),
				m_via1(*this,"via1"),
				m_via2(*this,"via2")
				{

				}

			void cr124(machine_config &config);

		protected:
			virtual void machine_start() override ATTR_COLD;
			virtual void machine_reset() override ATTR_COLD;

		private:
			required_device<w65c02s_device> m_maincpu;
			required_memory_region m_rom;
			required_memory_region m_ram;
			required_device<6522via_device> m_via1;
			required_device<6522via_device> m_via2;

			void mem_map(address_map &map) ATTR_COLD;
	}

	void cr124_state::mem_map(address_map &map)
	{
		map(0x0000, 0x03ff).ram();
		map(0x0400, 0x040f).m(m_via1, FUNC(via6522_device::map)).mirror(0xf0);
		map(0x0800, 0x080f).m(m_via2, FUNC(via6522_device::map)).mirror(0xf0);
		map(0x8000, 0xffff).rom().region("maincpu", 0);
	}

	//This is the constructor of the class

	void cr124_state::cr124(machine_config &config)
	{
		W65C02S(config, m_maincpu, 1'000'000);
		m_maincpu->set_addrmap(AS_PROGRAM, &cr124_state::mem_map);

		MOS6522(config, m_via1, 1'000'000);

		MOS6522(config, m_via2, 1'000'000);
	}

	void cr124_state::machine_start()
	{

	}

	void cr124_state::machine_reset()
	{

	}

	//**************************************************************************
	//  INPUT PORT DEFINITIONS
	//**************************************************************************

	static INPUT_PORTS_START( cr124 )
	INPUT_PORTS_END

	ROM_START (cr124)
		ROM_REGION(0x8000, "maincpu", 0)
		ROM_LOAD("crc.bin", 0x8000, 0x0004, CRC(2d71bc67) SHA1(999f478cda5dc6b9da845ae8580789da6292fc75))
		ROM_LOAD("bp124.bin", 0x8004, 0xfffc, CRC(2d71bc67) SHA1(999f478cda5dc6b9da845ae8580789da6292fc75))
	ROM_END

}

COMP( 1980, cr124, 0,      0,      cr124, cr124,     cr124_state, empty_init, "COMELTA",   "CR-124", MACHINE_NOT_WORKING)
//COMP( 1986, jb176, 0,      0,      jb176, jb176,     jb176_state, empty_init, "J.B. Electrónica Industrial",   "JB-176", MACHINE_NOT_WORKING)
//COMP( 1986, ep324, 0,      0,      ep324, ep324,     ep324_state, empty_init, "PYCMESA",   "EP-324", MACHINE_NOT_WORKING)
//COMP( 1980, bp124, 0,      0,      bp124, bp124,     bp124_state, empty_init, "Bits Passats",   "BP-124", MACHINE_NOT_WORKING)
