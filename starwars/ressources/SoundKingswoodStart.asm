http://www.wardclan.f9.co.uk/Star%20Wars%20Sound.html


Star Wars Sound


Sound disassembly

;    Working source assembler for Star Wars sound board. Dis-assembled form original binaries 136021.107 and 136021.208


;     Sound interrupt timer from 6532 is 4mS

;    Sound FX use 2 POKEYs for total of 8 channels. Uses POKEY CI/O 3 and CI/O 2 which are at PCB locations 2D and 3D respectively
;    Sound FX work from a pair of tables of 4 byte command data, one pair of tables for each POKEY channel
;    First table is for frequency, second table is for volume/distortion. This creates a frequency and volume envelope in timed steps
;    Volume tables seem to control when FX end point is, by last single data byte being 0
;    A sound data table starts with a byte that has a bit set for each channel to be used, then following that byte is a pair of 16 bit pointers
;    to the frequency and volume/distortion tables, one pair of pointers for each channel that is flagged in use
;
;    e.g. for 'Fire lasers' sound:
; byte_7354:      fcb $C0            Uses first 2 channels from
;                 fdb stru_6B46        Channel 1 frequency list
;                 fdb stru_6B4E        Channel 1 vol/dist list
;                 fdb stru_6B6B        Channel 2 frequency list
;                 fdb stru_6B73        Channel 2 vol/dist list



;
;    4 bytes in table represent:
;    Byte 0 = Timer count for each change (addition) to freq/volume
;    Byte 1 = Timer count for unchanging freq/volume (repeat last data)
;    Byte 2 = Frequency or volume/distortion data
;    Byte 3 = Addition/subtraction value to change freq or volume
;
;    When changing and no change timers have expired, increment table pointer to next 4 bytes of data
;   
;



;    Music uses 2 POKEYs but with 2 16 bit channels each, for a total of 4 16 bit music voices.
;    Uses POKEY CI/O 1 and CI/O 0 which are at PCB locations 4D and 5D respectively
;
;
;
;
;
;
;
;
;
;
; ---------------------------------------------------------------------------

;ptrRAM_Music    struc ;    (sizeof=0x8)
;ptrRAM:        fdb ?
;field1:        fcb ?
;POKEYReg1:    fdb ?
;field4:        fcb ?
;POKEYReg2:    fdb ?
;ptrRAM_Music    ends

; ---------------------------------------------------------------------------

;strMusicRAM    struc ;    (sizeof=0x19)
;field_0:    fcb ?
;field_1:    fcb ?
;ptrMusicData1:    fdb ?
;WORD_4:        fdb ?
;field_6:    fcb ?
;field_7:    fcb ?
;field_8:    fcb ?
;field_9:    fcb ?
;field_A:    fcb ?
;field_B:    fcb ?
;field_C:    fcb ?
;field_D:    fcb ?
;field_E:    fcb ?
;field_F:    fcb ?
;field_10:    fcb ?
;field_11:    fcb ?
;WORD_12:    fdb ?
;WORD_14:    fdb ?
;field_16:    fcb ?
;WORD_17:    fdb ?
;strMusicRAM    ends

; ---------------------------------------------------------------------------

;sndFX_RAM       struc ; (sizeof=0xA)
;ptrFXFr:        fdb ?
;FNoChTmr:       fcb ?
;FChTmr:         fcb ?
;Freq:           fcb ?
;ptrFXVl:        fdb ?
;VNoChTmr:       fcb ?
;VChTmr:         fcb ?
;Vol_Dist:       fcb ?
;sndFX_RAM       ends



; Processor:        6809
; Target assembler: AS09 V1.42 http://www.kingswood-consulting.co.uk/assemblers/

; ===========================================================================

    struct    sndFX_Table
    db    ChTmr                  ; Timer count for each change (addition) to freq/volume
    db    NoChTmr                  ; Timer count for unchanging freq/volume (repeat last data)
    db    InitValu                ; Frequency or volume/distortion data
    db    AdderVal                ; Addition/subtraction value to change freq or volume
    end    struct


;    8 bit port write back to main CPU

SOUT:    equ    $0000

;    8 bit port read from main CPU

SIN:    equ    $800


;    Direct page always at $1000 which is 6532 PIA RAM
;    Stack is from $107F down, so also in PIA RAM
;    PIA failure will therefore not allow sound board code to run


