# nanokontrol_seq
A simple 16 step CV sequencer built around a Korg Nanokontrol

This project consist of a new firmware and hardware modifications to turn a Nanokontrol into a CV sequencer.
It serves as an example of how to interface the potentiometers, buttons and LEDs of the Nanokontrol.

To apply the firmware and modifications, a few steps shold be followed.
1. Create an SPI adapter as per the schematic
2. Flash the fuses with "-U lfuse:w:0x3f:m -U hfuse:w:0xd0:m"
3. Desolder the PDIUSBD12 chip
4. Jumper the crystal to the Atmega32
5. Wire in the MAX528
6. Flash the new firmware
