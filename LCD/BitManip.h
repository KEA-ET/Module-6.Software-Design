#ifndef BitManip_h
#define BitManip_h
/***********************/
// Bit Manipulation
/***********************/

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))  // PORTA = PORTA or 0x08 (1 on pin 3) // bitset(PORTA, RS) => bitset(PORTA, 0x04) 
#define bit_clr(p,m) ((p) &= ~(m)) // PORTA = PORTA & 0x08 (!1 on pin3)
#define bit_flip(p,m) ((p) ^= (m)) // PORTA = PORTÂ ^ 0x08 (Toggle between 0 and 1 on pin 3)
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m)) 
#define BIT(x) (0x01 << (x)) 
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))

#endif
