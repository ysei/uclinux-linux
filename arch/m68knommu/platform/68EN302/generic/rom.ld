MEMORY 
	{
	romvec : ORIGIN = 0x10c10000, LENGTH = 0x00400
	flash  : ORIGIN = 0x10c10400, LENGTH = 0xfec00
	eflash : ORIGIN = 0x10d00000, LENGTH = 1
	ramvec : ORIGIN = 0x00000000, LENGTH = 1024
	ram    : ORIGIN = 0x00020000, LENGTH = 0x00800000 - 0x20000
	eram   : ORIGIN = 0x00800000, LENGTH = 1
	}

SECTIONS
{
	.romvec :
	{
	_romvec = . ;
	_flashstart = . ;
	} > romvec
        .text :
	{
	text_start = . ;
        *(.text)
	_etext = . ;
	__data_rom_start = ALIGN ( 4 ) ;
        } > flash
	.eflash :
	{
	_flashend = . ;
	} > eflash
	.ramvec :
	{
	_ramstart = . ;
	_ramvec = . ;
	} > ramvec
        .data :
	{
	_sdata = . ;
	__data_start = . ;
        *(.data)
	_edata = . ;
	edata = ALIGN( 0x10 ) ;
        } > ram
        .bss :
        {
	_sbss = ALIGN( 0x10 ) ;
	__bss_start = ALIGN( 0x10 ) ;
	__data_end = ALIGN( 0x10 ) ;
	*(.bss)
	*(COMMON)
	_ebss = . ;
	end = ALIGN( 0x10 ) ;
	_end = ALIGN( 0x10 ) ;
	} > ram
	.eram :
	{
	_ramend = . ;
	} > eram
}
