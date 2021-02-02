#include <stdint.h>

#define RCC_APB2ENR (*((uint32_t *) (0x40021000 + 0x18)))
#define TIM1EN (0x01 << 11)
#define IOPCEN (0x01 << 4)

#define AFIO_EXTICR1 (*((uint32_t *) (0x40010000 + 0x08)))
//#define AFIO_EXTICR2 (*((uint32_t *) (0x40010000 + 0x0C)))

#define AFIOEN (0x01 << 0)
#define IOPBEN (0x01 << 3)

#define MODE_OUT_2MHZ 0x02
#define CNF_GP_OUT_PUSH_PULL 0x00

#define MODE_IN 0x00
#define CNF_GP_IN_PULL 0x02

#define EXTI_PORT_C 0x02
#define EXTI_PORT_B 0x01


void init_RCC()
{
	RCC_APB2ENR |= TIM1EN | IOPCEN | AFIOEN | IOPBEN ;
}

#define NVIC_ISER0 (*((uint32_t *) (0xE000E100 + 0x00)))
#define SETENA 0x01

void init_NVIC()
{
	NVIC_ISER0 = SETENA << 25;
	NVIC_ISER0 = SETENA << 6;
	NVIC_ISER0 = SETENA << 7;


}

#define GPIOC_CRL (*((uint32_t *) (0x40011000 + 0x00)))
#define GPIOC_CRH (*((uint32_t *) (0x40011000 + 0x04)))
#define GPIOC_ODR (*((uint32_t *) (0x40011000 + 0x0C)))
#define GPIOC_IDR (*((uint32_t *) (0x40011000 + 0x08)))


#define GPIOB_CRL (*((uint32_t *) (0x40010C00 + 0x00)))
#define GPIOB_IDR (*((uint32_t *) (0x40010C00 + 0x08)))

#define MODE_OUT_2MHZ 0x02
#define CNF_GP_OUT_PUSH_PULL 0x00

#define MODE_IN 0x00
#define CNF_GP_IN_PULL 0x02

void init_GPIOC()
{
	uint32_t gpioc_crl_value = GPIOC_CRL;
	for (int i = 0; i < 8; i++)
	{
		gpioc_crl_value &= ~(0x0F << (4 * i));
		gpioc_crl_value |= MODE_OUT_2MHZ << (4 * i) | CNF_GP_OUT_PUSH_PULL << (2 + 4 * i);
	}
	GPIOC_CRL = gpioc_crl_value;
	
	uint32_t gpioc_crh_value = GPIOC_CRH;
	for (int i = 0; i < 6; i++)
	{
		gpioc_crh_value &= ~(0x0F << (4 * i));
		gpioc_crh_value |= MODE_OUT_2MHZ << (4 * i) | CNF_GP_OUT_PUSH_PULL << (2 + 4 * i);
	}
	GPIOC_CRH = gpioc_crh_value;
	
	GPIOC_ODR = 0x0;

}

void init_GPIOB()
{

	uint32_t gpiob_crl_value = GPIOB_CRL;

	gpiob_crl_value &= ~(0x0F << (4 * 0));
	gpiob_crl_value |= MODE_IN << (4 * 0) | CNF_GP_IN_PULL << (2 + 4 *0);

	GPIOB_CRL = gpiob_crl_value;

	gpiob_crl_value &= ~(0x0F << (4 * 1));
	gpiob_crl_value |= MODE_IN << (4 * 1) | CNF_GP_IN_PULL << (2 + 4 *1);

	GPIOB_CRL = gpiob_crl_value;

	GPIOB_IDR = 0x00;
}

#define TIM1_CR1 (*((uint32_t *) (0x40012C00 + 0x00)))
#define TIM1_SMCR (*((uint32_t *) (0x40012C00 + 0x08)))
#define TIM1_DIER (*((uint32_t *) (0x40012C00 + 0x0C)))
#define TIM1_SR (*((uint32_t *) (0x40012C00 + 0x10)))
#define TIM1_CNT (*((uint32_t *) (0x40012C00 + 0x24)))
#define TIM1_PSC (*((uint32_t *) (0x40012C00 + 0x28)))
#define TIM1_ARR (*((uint32_t *) (0x40012C00 + 0x2C)))
#define UIE 0x01
#define PSC_VALUE 7999
#define ARR_VALUE 8
#define CEN 0x01

void init_TIM1()
{
	TIM1_DIER |= UIE;
	TIM1_SR = 0;
	TIM1_PSC = PSC_VALUE;
	TIM1_ARR = ARR_VALUE;
	TIM1_CR1 |= CEN;
}


/* 
 * Niz vrednosti koje je potrebno postaviti
 * na anode sedmosegmentnog displeja sa zajednickom katodom
 * kako bi se prikazala odgovarajuca decimalna cifra.
 *
 *                              a
 *                           *******
 *                         *         *
 *                       f *         * b
 *                         *    g    *
 *                           *******
 *                         *         *
 *                       e *         * c
 *                         *    d    *
 *                           *******
 */
uint8_t nickname[] = { 0x39, 0x79, 0x5B, 0x77, 0x33, 0x3F };
uint8_t index_number[] = { 0x7D, 0x3F, 0x5B, 0x40, 0x06, 0x7F };


uint8_t digits[] = { 0, 0, 0, 0 , 0, 0};
uint8_t current_digit = 0;
int iterator = 0 ;
int iterator2 = 0 ;
int direction = 1 ;
int button1 = 1;


uint8_t interrupt_count = 0;

void process_tim1_update_interrupt()
{

	if (++interrupt_count == 80)
		{



			if(direction == 1 ){
				interrupt_count = 0;

				digits[0] = iterator % 6;
				digits[1] = (1+ iterator)% 6 ;
				digits[2] = (2+ iterator)% 6 ;
				digits[3] = (3+ iterator)% 6;
				digits[4] = (4+ iterator)% 6 ;
				digits[5] = (5+ iterator )% 6 ;

			  	iterator += 1 ;

			  	if (iterator == 6 ) {
			  		iterator = 0 ;
			  	}
			}
			else{

				interrupt_count = 0;

				digits[0] = ((0 - iterator2) + 6) % 6;
				digits[1] = (( 1 - iterator2 )+ 6) % 6 ;
				digits[2] = ((2- iterator2 )+ 6 ) % 6;
				digits[3] = ((3- iterator2)+ 6) % 6;
				digits[4] = ((4- iterator2)+ 6) % 6 ;
				digits[5] =( (5- iterator2 )+ 6) % 6 ;

				iterator2 =  iterator2 + 1  ;
				if(iterator2 == (6)) {
					  		iterator2 = 0 ;
				}
			}

		}


	    if( GPIOB_IDR == 0x01 ) {
						 direction = (direction ^ 1 ) ;

						 //za reset svakog displeja na pocetnu vrednost
//						 	GPIOC_ODR |= 0x3F << 8;
//						 	GPIOC_ODR &= ~0xFF;
//
//
//						 	nickname[0] =  0x7D ;
//						 	nickname[1] =  0x3F ;
//						 	nickname[2] =  0x31 ;
//						 	nickname[3] =  0x06 ;
//						 	nickname[4] =  0x38 ;
//						 	nickname[5] =  0x77 ;
//
//						 	index_number[0] = 0x7D  ;
//						 	index_number[1] = 0x3F  ;
//						 	index_number[2] = 0x5B  ;
//						 	index_number[3] = 0x40  ;
//						 	index_number[4] = 0x06  ;
//						 	index_number[5] = 0x7F  ;


						 GPIOB_IDR &= ~0xFF;
					}
		if( GPIOB_IDR == 0x02 ) {
						 button1 = (button1 ^ 1 ) ;

//						 	GPIOC_ODR |= 0x3F << 8;
//						 	GPIOC_ODR &= ~0xFF;
//
//
//						 	nickname[0] =  0x7D ;
//						 	nickname[1] =  0x3F ;
//						 	nickname[2] =  0x31 ;
//						 	nickname[3] =  0x06 ;
//						 	nickname[4] =  0x38 ;
//						 	nickname[5] =  0x77 ;
//
//						 	index_number[0] = 0x7D  ;
//						 	index_number[1] = 0x3F  ;
//						 	index_number[2] = 0x5B  ;
//						 	index_number[3] = 0x40  ;
//						 	index_number[4] = 0x06  ;
//						 	index_number[5] = 0x7F  ;

						 GPIOB_IDR &= ~0xFF;

					}


	    GPIOC_ODR |= 0x3F << 8;
		GPIOC_ODR &= ~0xFF;


		if(button1 == 1 ){
			GPIOC_ODR |= nickname[digits[current_digit]];
		}else {
			GPIOC_ODR |= index_number[digits[current_digit]];
		}

		GPIOC_ODR &= ~(0x1 << (8 + current_digit));

		current_digit = (current_digit + 1) % 6;


}

void tim1_update_handler()
{
	if ((TIM1_SR & 0x01) != 0)
	{
		TIM1_SR = 0;
		process_tim1_update_interrupt();
	}
}


int main()
{

	init_RCC();
	init_NVIC();
	init_GPIOC();
	init_GPIOB();
	init_TIM1();

	

	while (1)
	{

	}
	
}
