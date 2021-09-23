
I included USPI (https://github.com/rsta2/uspi) (library derived from circle) into the vectrexInterface library.
As of now only the keyboard is used, as that was at the moment, what I wanted to get to run.

Usually in order for USPI to work - it must use interrupts. 
However due to known reasons I can not allow any foreigner to issue interrupts, so I sought ways to prevent that
as of now it seems I succeeded.

I have implemented USPI into my program, and sort of did a small environment header like described in uspios.h.
Thus at the moment I can compile and use the complete uspi.

I run the "normal" setup routine, and it sets everything up fine. I can use the keyboard (yeah!)

But as said - I don't want it to disturb my own interrupts. So after the first initialization, I withdraw all interrupt handling and sort of
"disable" uspi that way.

During my own program I call now the "old" interrupt handler, as was set with the "ConnectInterrupt()" routine, 
approximately 50 times per second (once during WaitRecal) (I do not "wait" for an interrupt, I just keep calling the handler, 
since I have seen via output, that the USB device calls it also heaps and heaps of times without providing any information).

So effectively I am "polling" (or pushing???) the interrupt handler - without using an interrupt at all.
... and "oh wonder" - with this simple polling mechanism, 
I still am able to use the keyboard, since the called "handler" still "catches" all keystrokes and calls the set keyPressedHandler().

The "called" handler will call a set keyPressedHandler() - however, I do not want to use that either.

Some more background first:
- the USB implementation gets "raw" key events from the keyboard
- that are 0 -0x7f different key events
- modifiers can be set (like SHIFT, CTRL, ALT, LEFT SHIFT etc etc)
- the USB library supports different keymaps (translation maps from physical to "standard" chars)
- these maps are "hardcoded" and only one can be compiled into the system, I chose keymap_us (default)
- with some effort this can certainly be made more dynamic...
- the access to "physical" keys is faster, since than we do not have a translation table in between
- per default USPI has only a handler for keyPressed(), not for keyReleased()
- however internally this <can> be rectified
- the library can use handlers with two modes, "RAW" and "keymaped"
- I do not intend to use any of these handlers, but looking at the implementation one sees, that
  the USB keyboard generates status reports in a format, that one can extract all needed information without handlers from
- the status reports a 8 byte buffer, the first byte contains the modifiers, and bytes 3-7 contain all currently
  pressed keys (filled from bottom to top, the highest byte always contains the last pressed key, 0 otherwise)
- if the keyboard can not identify the keys (to many keys pressed on the keyboard grid) the keycode of all keys is "1"
- so - looking at that "status" buffer, one can at all times discern 
  * all modifiers
  * all currently pressed keys
  * the order of the currently pressed keys
  (all in physical key notation) 
- if you want to implement a keyReleasedHandler(), one just has to add a few compares...

For my future "projects" I added following functionality to USPI:


  unsigned char USPiKeyboardToPhysical(unsigned char key);
  unsigned char *USPiKeyboardPhysicalToKeyboard(unsigned char phys, unsigned char ucModifiers);
A mapping function which can be reached from the outside, to map between "keymap" and "physical" and vice/versa.

  unsigned char USPiKeyboardGetModifiers();
A function to get the current modifiers.

  // key in "current keymap"
  int USPiKeyboardIsDown(unsigned char key);

  // key in pyhsical
  int USPiKeyboardIsDownPhysical(unsigned char key);
A function to ask "is that key down" (or still down), physical and keymaped.

  unsigned char USPiKeyboardLastPhysicalKeyDown();
  unsigned char USPiKeyboardLastKeyDown();

Returns the last pressed char (or physical),
ONCE after the key press was handled by the handler: USBKeyboardDeviceCompletionRoutine() 
after that it will return 0, even if the key is still pressed!

The last pressed key wins!
I have not implemented a "buffering" of keystrokes (yet?). That means,  
if this is "seldom" called, this will lose keystrokes!

---

From any pitrex baremetal program, following lines initialize the keyboard, this should be
done at the earliest possible moment (but after the init functions).
(yea, sure, we must remember the "status"...)

    // Initialize USB system we will want keyboard 
    if (!USPiInitialize ())
    {
      printf("USB failed to initialize!\n\r");
    }
    if (!USPiKeyboardAvailable ())
    {
        printf("Keyboard not found\n\r");
    }
//    USPiKeyboardRegisterKeyPressedHandler (KeyPressedHandler);
    DisconnectUSBInterrupt();


The USPiInitialize() installs an own interrupt handler - after the initialization we manually remove that handler by 
calling: DisconnectUSBInterrupt();
However internally the vectrexInterface still remembers, that usb was "installed", and while in 
IRQMode it calls upon each WaitRecal the "interruptHandler" of USPI, to simulate its activity.

If you want to REALLY uninstall USPI (so that in IRQMode this handling is disabled, than you have to set
the internal variable "useUSB" in the vectrecInterface to 0, I have not implemented a function for that (yet).

However, once you did the above, you can in your own "game round" use above functions like:

      char lk = USPiKeyboardLastKeyDown();
      if (lk)
      {
        printf("%c", lk);
      }

Which well, just prints the just pressed key... but you get the drift :-).
