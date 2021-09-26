This directory contains a the enhanced 
[vecx](http://www.valavan.net/vectrex.html) (Vectrex emulator) base package for PiTrex.

The processor 6809 is emulated - but all accesses to VIA are passed thru to the "real" VIA that the PiTrex is connected to.
This makes this emulator near "perfect".

The only "watchout" is, that the timing of the instructions must also be "perfect" in relation to the VIA - which is a major hassle to do right.

As of now interrupts coming from the vectrex are only partly supported (lightpen works, 3d-imager does not correctly work)
