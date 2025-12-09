#include "emu.h"
#include "cpu/m6502/w65c02s.h"
#include "machine/6522via.h"


namespace
{
	class cr124_state : public driver_device
	{
		public:

			cr124_state(const machine_config &mconfig, device_type type, const char *tag)
				: driver_device(mconfig, type, tag),
				m_maincpu(*this,"maincpu"),
				m_via1(*this,"via1"),
				m_via2(*this,"via2"),
				m_diag_digits(*this, "digit%u", 0U),
				m_diagnostic_leds(*this, "led%u", 5U)
				{

				}

			void cr124(machine_config &config);

		protected:
			virtual void machine_start() override ATTR_COLD;
			virtual void machine_reset() override ATTR_COLD;

		private:
			required_device<w65c02s_device> m_maincpu;
			required_device<via6522_device> m_via1;
			required_device<via6522_device> m_via2;
			output_finder<2> m_diag_digits;
			output_finder<3> m_diagnostic_leds;

			uint8_t diagnosticsPort = 0;
			uint8_t hex_seven_segment[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

			uint8_t readDiagnosticsPort();
			void writeDiagnosticsPort(uint8_t value);


			void mem_map(address_map &map) ATTR_COLD;
	};

	uint8_t cr124_state::readDiagnosticsPort()
	{
		return this->diagnosticsPort;
	}

	void cr124_state::writeDiagnosticsPort(uint8_t value)
	{
		this->diagnosticsPort = value;
		//m_diag_digits[0] = cr124_state::hex_seven_segment[value & 0x0f];
		//m_diag_digits[1] = cr124_state::hex_seven_segment[(value >> 4) & 0x0f];
	}

	void cr124_state::mem_map(address_map &map)
	{
		map(0x0000, 0x03ff).ram();
		map(0x0400, 0x040f).m(m_via1, FUNC(via6522_device::map)).mirror(0xf0);
		map(0x0800, 0x080f).m(m_via2, FUNC(via6522_device::map)).mirror(0xf0);
		map(0x1800, 0x7fff).rw(FUNC(cr124_state::readDiagnosticsPort),FUNC(cr124_state::writeDiagnosticsPort));
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
		diagnosticsPort = 0;
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
		ROM_LOAD("crc32.bin", 0x0000, 0x0004, CRC(71855991) SHA1(89887f1afc486919c575ed44dfb6ac4125fea29c))
		ROM_LOAD("bp124.bin", 0x0004, 0x7ffc, CRC(02770453) SHA1(06d93109dd303de5e45e5cb06f3b4513265531da))
	ROM_END

}

COMP( 1980, cr124, 0,      0,      cr124, cr124,     cr124_state, empty_init, "COMELTA",   "CR-124", MACHINE_NOT_WORKING | MACHINE_NO_SOUND_HW)
//COMP( 1986, jb176, 0,      0,      jb176, jb176,     jb176_state, empty_init, "J.B. Electrónica Industrial",   "JB-176", MACHINE_NOT_WORKING)
//COMP( 1986, ep324, 0,      0,      ep324, ep324,     ep324_state, empty_init, "PYCMESA",   "EP-324", MACHINE_NOT_WORKING)
//COMP( 1980, bp124, 0,      0,      bp124, bp124,     bp124_state, empty_init, "Bits Passats",   "BP-124", MACHINE_NOT_WORKING)
