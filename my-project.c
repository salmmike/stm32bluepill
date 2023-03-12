#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <stdlib.h>

static void clock_setup(void)
{
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_USART3);
}

static void usart_setup(void)
{
	/* Setup GPIO pin GPIO_USART3_TX/GPIO9 on GPIO port B for transmit. */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		      	  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART3_TX);

	/* Setup UART parameters. */
	usart_set_baudrate(USART3, 112500);
	usart_set_databits(USART3, 8);
	usart_set_stopbits(USART3, USART_STOPBITS_1);
	usart_set_mode(USART3, USART_MODE_TX);
	usart_set_parity(USART3, USART_PARITY_NONE);
	usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART3);
}

static void mpsusart_send_message(const char* msg, size_t len)
{
	for (size_t i = 0; i < len; ++i) {
		usart_send_blocking(USART3, msg[i]);
	}
	usart_send_blocking(USART3, '\r');
	usart_send_blocking(USART3, '\n');
}

static void gpio_setup()
{
	gpio_set_mode(GPIOC,
				  GPIO_MODE_OUTPUT_2_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL,
				  GPIO13);
}

static void usleep(size_t time) {
	for (size_t i = 0; i < time; ++i) {
		__asm__("nop");
	}
}


int main(void) {
	clock_setup();
	usart_setup();
	gpio_setup();

	while (1) {
		usleep(10000000);
		gpio_toggle(GPIOC, GPIO13);
		mpsusart_send_message("Hello!", 7);
	}
}
