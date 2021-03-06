#include <cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include "put_bytes.h"
#include "uptime.h"
#include "scheduler.h"
// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

void kernel_start(void);
void test_module_put_bytes();
void test_module_uptime();


extern void ctx_sw(int32_t a, int32_t b);
extern void proc1(void);
extern void proc2(void);
extern void proc3(void);
extern void proc4(void);
extern void traitant_IT_32(void); 

void kernel_start(void)
{
	// Init of the printing parameters
	set_text_format(0, 3, 15);
	place_curseur(0,0);
	efface_ecran(); 

	// Initialisation du processus Proc1, 2, 3, 4, 5, 6
	init();

        // Setting of the clock frequency :
        init_clock();

        // Binds the interrupt handler "traitant_IT_32" to the cell number 32 of
	//  the IDT (Interrupt Descriptor Table) 
	init_traitant_IT(32, traitant_IT_32);

        // Unmask IRQ0 : the signals can go through the interrupt controller
	masque_IRQ(0, 0);
        
        // Lancement idle
	idle();
        
	// on ne doit jamais sortir de kernel_start
	while (1) {
		// cette fonction arrete le processeur
		hlt();
	}
}

/******************************************************************************/
/****************** FONCTIONS DE TEST DES MODULES IMPLÉMENTÉS *****************/
/******************************************************************************/

/**
 * Ces fonctions ont été utilisées durant le développement pour tester le bon 
 * fonctionnement des séances.
 */

void test_module_uptime()
{
        /* Fonction créée pour tester le bon fonctionnement du fichier uptime.c */
        
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
