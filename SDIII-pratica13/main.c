
//----------------------------- Bibliotecas ------------------------------------
//#include  <msp430.h>
#include "io430.h"
#include "teclado.h"

# define CICLOS	999	//Define um per�odo de 1000 pulsos de clock (F=1kHZ) 
# define CICLOS_METADE 473
//******************************************************************************

unsigned char tecla;                          // Cria as variaveis que v�i armazenar as teclas

unsigned char cont_init, cont_passos;
void rampa (void)
{
 TA0CCR1= CICLOS_METADE;           // Coloca no 50% com ajuste de offset.
 cont_init = 200;
 cont_passos = 50;
   //
  TACCTL0 = CCIE;
   __bis_SR_register(GIE);   	// Habilita interrup��o geral

  //
   while( cont_passos != 0)
   {
   while(1)
   {   if(cont_init == 0)
          break;
   }
   TA0CCR1+=10;			//Incrementa DT
   cont_init = 200;
   cont_passos--;
   }
   TACCTL0_bit.CCIE = 0;
   __bic_SR_register(GIE);   	// Habilita interrup��o geral

}


// Programa principal
void main(void)
{
  //Configura o hardware
  WDTCTL = WDTPW+WDTHOLD;     	// Desativa o WDT

  // Configura��o do m�dulo de clock do sistema
  // Configura��o DCO para frequ�ncia ~734KHz - Configura��o default
 
  BCSCTL1 &= ~XT2OFF;                              //Ligando o oscilador XT2CLK
  BCSCTL2 |= SELS;				//Define XT2 como fonte de clock do SMCLK

  // Configura��o do Timer_A
  // N�cleo do Timer
  // Define valor inicial do TAR
  TA0R=0;                     	//Reseta contador 
  TACTL = TASSEL1 |          	// Fonte do clock: SMCLK ( 4MHz )
	      ID1 +					// Divide SMCLK/4 = 1MHz
          MC_1;               	
  
  // Bloco Capture/Compare 0
  TA0CCR0=CICLOS;            	// Per�odo da modula��o

  // Bloco Capture/Compare 1
  TA0CCR1=CICLOS_METADE;            	// Limite da contagem a cada CICLO de clock (DT=50%)
  TA0CCTL1 = OUTMOD_7;         	// Modo de opera��o 7 Reset/set

  // Configura��o dos I/Os
  P1DIR |= BIT2; 				// Define P1.2 como sa�da
  P1SEL |= BIT2; 				// Habilita fun��o especial em P1.2 (TA1)- PWM

  //Variaveis
  configura_teclado();                          // Chama a fun��o que configura os teclados
  
  while(1)// loop infinito
	{
	  tecla=teclado();	// Chava a fun��o para ler os teclados
          
            if(tecla == '1')    // Verifica se a tecla � 1.
            {
              TA0CCR1-=10;			//Decrementa DT            
            }
            if(tecla == '2')    // Verifica se a tecla � 2.
            {
              TA0CCR1= CICLOS/2;                // Coloca no 50% de CT no valor teorico
            }
            if(tecla == '3')    // Verifica se a tecla � 3.
            {
              TA0CCR1+=10;			//Incrementa DT
            }
            if(tecla == '5')    // Verifica se a tecla � 5.
            {
              TA0CCR1= CICLOS_METADE;           // Coloca no 50% com ajuste de offset.
            }
            if(tecla == '7')    // Verifica se a tecla � 7.
              TA0CCR1=CICLOS_METADE+100;        // Coloca na rota��o horario com 10% da velocidade total
            //Sendo 1000 a velocidade total, 10% de 1000=100, logo 10% no sentido horario � o 50% (ele patrado) mais 100.
            
            if(tecla == '8')
            {
                rampa();
            }
                                       
	}

}

// Interrup��o do TimerA0
#pragma vector=TIMER0_A0_VECTOR
__interrupt void baseTempo()
{
 
    cont_init--;
  
}
