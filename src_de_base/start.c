#include <cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include "put_bytes.h"
#include "uptime.h"
#include "scheduler_coop.h"
// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

uint32_t fact(uint32_t n);
void kernel_start(void);
void test_module_put_bytes();
void test_module_uptime();


// une fonction bien connue
uint32_t fact(uint32_t n)
{
	uint32_t res;
	if (n <= 1) {
		res = 1;
	} else {
		res = fact(n - 1) * n;
	}
	return res;
}

extern void ctx_sw(int32_t a, int32_t b);

void kernel_start(void)
{
	// Init of the printing parameters
	set_text_format(0, 3, 15);
	place_curseur(0,0);

	// Process management :
	struct Process* proc_table = get_proc_table();

	// Initialisation du processus Idle
	proc_table[0].pid = 0;
	strcpy(proc_table[0].name, "idle");
	proc_table[0].state = ELU;
	set_elected_proc(proc_table);

	// Initialisation du processus Proc1
	proc_table[1].pid = 1;
	strcpy(proc_table[1].name, "proc1");
	proc_table[1].state = ACTIVABLE;
	// At the first execution the context of proc1 has never been saved.
	// |- We need to store a pointer on the stack in the register save zone
	proc_table[1].save_zone[1] = 
		(int32_t)(proc_table[1].stack+(STACK_SIZE - 1));
	// |- The stack normally stocks the adress of the function that were 
	// |  running before the interruption of the process :
	proc_table[1].stack[STACK_SIZE - 1] = (int32_t)proc1; 
	// |- Rq : the stack is fulfilled from the higher index
		
	// Lancement idle
	efface_ecran(); 
	idle();

	//test_module_put_bytes();
	
	//efface_ecran();
	
	//test_module_uptime();

	// Démasquer les interruptions externes 
	//sti();
	
	// on ne doit jamais sortir de kernel_start
	while (1) {
		// cette fonction arrete le processeur
		hlt();
	}
}

// Used to say that traitant_IT_32 is defined in an external file (traitant.s)
extern void traitant_IT_32(void); 

void test_module_uptime()
{
	// [Unit Testing] Test print_uptime
	//print_uptime("10:29");

	// [Unit Testing] Test Tic_PIT function 
	//tic_PIT(); 



	// Setting of the clock frequency :
	init_clock();

	// Binds the interrupt handler "traitant_IT_32" to the cell number 32 of
	//  the IDT (Interrupt Descriptor Table) 
	init_traitant_IT(32, traitant_IT_32);
	
	/* DOC :
	   Traitant_IT_32 is the name of the assembly function coded in traitant.S
	    |--> This func will : 
	          - save the registers,
		  - call the true interrupt handler "tic_PIT()"
		  - restore the register values
	 */

	// Unmask IRQ0 : the signals can go through the interrupt controller
	masque_IRQ(0, 0);
	
}

void test_module_put_bytes()
{
	uint32_t x = fact(5);
	// quand on saura gerer l'ecran, on pourra afficher x
	(void)x; 

	/* Test of screen clearing */ 
	efface_ecran();

	/* Test of screen printing */ 
	struct char_format f = {0, 1, 2};
	ecrit_car(0, 1, '0', &f);
	ecrit_car(1, 1, '1', &f);
	ecrit_car(2, 1, '2', &f);
	ecrit_car(5, 5, 'a', &f);

	/* Test of the functions manipulating the cursor */ 
	// Should print an x in (10,10), (= 9,9 after defilement)
	place_curseur(10,10);
	uint32_t lig;
	uint32_t col;
	get_curseur(&lig, &col); 
	ecrit_car(lig, col, 'x', &f);

	/* Test of the function defilement */ 
	defilement(); // makes the 0 disappear.

	/* Test of the function traite_car */ 
	// should print 'a' at the cursor position (i.e. 10,10), in white over cyan
	set_text_format(0, 3, 15);
	traite_car('a'); 

	efface_ecran();
	place_curseur(0,0);
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");
	printf("truc facile un peu long de plus d'une ligne ici ou la bla bla bla bli bli bli bli blo blo blo\n");

	printf("Lorem ipsum\f dolor sit amet, \bconsectetur\n adipiscing\t elit. Maecenas quis mauris pharetra, consequat nunc quis, accumsan velit. Sed sed consectetur quam. Integer eleifend eu dui placerat lobortis. Nulla bibendum pulvinar ligula ut feugiat. \nDuis leo risus, egestas a ipsum vitae,\r Lobortis sollicitudin velit. Duis tempor felis vitae quam sodales, sit amet molestie odio porttitor. Donec nec nunc posuere, condimentum tortor eget, varius lorem. Aliquam ultricies odio est. Suspendisse tincidunt nec ante eget eleifend.");

}
