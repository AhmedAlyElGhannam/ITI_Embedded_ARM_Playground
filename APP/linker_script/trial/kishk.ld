ENTRY(ResetHandler) /* the first thing that runs in my code (entry point) */

__stack_end = ORIGIN(RAM) + LENGTH(RAM);

__stack_size = 0x400;
__heap_size = 0x200;

MEMORY 
{
    FLASH (xr ):  ORIGIN = 0x08000000, LENGTH = 256K   
    RAM   (xrw):  ORIGIN = 0x20000000, LENGTH = 64k    
}

/*
    SECTIONS can contain:
        1. section name
        1. memory
        1. group
        1. flags
*/

SECTIONS
{
    .bss :
    {
        . = ALIGN(4); /* memory alignment for better performance */
        __bss_start = .; /* storing the address that marks the start of bss*/
        FILL(0xFF); /* the value of padded bytes for alignment */
        . += (*.bss); /* a command like this will be ignored if no variables are present under this section name */
        . += (*._bss); /*some compilers produce the section name like this*/
        . = ALIGN(4); /* memory alignment for better performance */
        __bss_end = .; /* storing the address that marks the end of bss*/

    } > RAM0; /* WHICH MEMORY THIS SECTION WILL BE IN */
    .data :
    {

    } > RAM AT > FLASH; /* reserve section size in RAM but initial values are in FLASH */
}