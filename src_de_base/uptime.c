#include "uptime.h"

uint32_t uptime_ms = 0;

void init_clock()
{
	// Send a command to the Clock, to say we will set a new frequency value
        outb(0x34, 0x43);
	// Send the 8 less significant bits of the freq value
        outb((QUARTZ / CLOCKFREQ) % 256, 0x40); // %256 = >>8
	// Send the 8 Most Significant bits of the freq value
        outb((QUARTZ / CLOCKFREQ) & 0xFF, 0x40);	
}

void print_uptime(char * time)
{
	// Get the string format
	struct char_format f = {0, 0, 15};

	// Display each char (one by one) on the screen
        uint32_t i = 0;
        while(time[i]){
		ecrit_car(0, 80 - strlen(time) + i, time[i], &f);
		++i;
        }
}

void tic_PIT(void)
{
	/* (This function is called every 20ms (1/clock frequency)). */

	// Sends an ack (to the interruption received) to the Interrupt Controller
	outb(0x20, 0x20);
	// update the uptime
	uptime_ms += 20;
	// convert the uptime (in ms) into a string of format HH:MM:SS
	char uptime_string[8]; 
	int ss = (int) (uptime_ms / 1000) % 60 ;
	int mm = (int) ((uptime_ms / (1000*60)) % 60);
	int hh = (int) ((uptime_ms / (1000*60*60)) % 24);
	sprintf(uptime_string, "%02d:%02d:%02d", hh,mm,ss);
	// print the uptime in the right corner of the screen.
	print_uptime(uptime_string); 

}


void init_traitant_IT(int32_t num_IT, void (*traitant)(void))
{
	/* 
	 * Binds a specific interrupt handler to the 
	 * cell number "num_IT" of the Interrupt Descriptor Table.
	 * |-> Cell Pattern (64 bits) : 
	 *       | KERNEL_CS | Low Addr (8 bits) | High Addr (8bits) | 0x8E00 |
	 */
	uint32_t* table_addr = (uint32_t*) 0x1000;
	table_addr[num_IT * 2] =  (((uint32_t) KERNEL_CS) << 16) | ((uint32_t) traitant & 0xFFFF); 
	table_addr[(num_IT * 2) + 1] = ((uint32_t) traitant & 0xFFFF0000) | ((uint32_t)0x8E00);
}

void masque_IRQ(uint32_t num_IRQ, bool masque)
{
	/* Forces the value of the bit "num_IRQ" to the value of "masque" */

	// Get the current value of the IRQ mask
	uint8_t mask_val = inb(0x21);
	// Modify the value of the "num_IRQ" bit with the new mask
	if(masque == 1)
		mask_val |= (1<<num_IRQ);
	else 
		mask_val &= ~(1<<num_IRQ);

	// Set the new value
	outb(mask_val, 0x21);  
}


