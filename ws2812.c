#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "ws2812.pio.h"
#include "hardware/pio.h"

#define NUM_LEDS 25
#define PIN 7
#define RED_LED 13
#define GREEN_LED 12
#define BLUE_LED 11
#define BUTTON_A 5
#define BUTTON_B 6

static volatile uint32_t last_event = 0;

double number_0[NUM_LEDS] = 
    {0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0};

double number_1[NUM_LEDS] = 
        
    {0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0};

double number_2[NUM_LEDS] = 
        
    {0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0};

double number_3[NUM_LEDS] = 
        
    {0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,};

double number_4[NUM_LEDS] = 
        
    {0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0};

double number_5[NUM_LEDS] = 
        
   {0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.0, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.0, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0};

double number_6[NUM_LEDS] = 
        
    {0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0};

double number_7[NUM_LEDS] = 
        
    {0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0};

double number_8[NUM_LEDS] = 
        
    {0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0};

double number_9[NUM_LEDS] = 
        
    {0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0};

double* numbers[] = {number_0, number_1, number_2, number_3, number_4, number_5, number_6, number_7, number_8, number_9};

// Variável para armazenar o desenho atual que pode mudar a qualquer momento
volatile uint8_t current_number = 0; 

// Rotina para definir a intensidade de cores do LED
uint32_t rgb_matrix(double r, double g, double b) {
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 06) | (B << 8);
}

void draw_number(double *number, uint32_t led_value, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUM_LEDS; i++) {
        led_value = rgb_matrix(number[i] * r, number[i] * g, number[i] * b);
        pio_sm_put_blocking(pio, sm, led_value);
    }
}


void button_pressed(uint gpio, uint32_t events){

    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if(current_time - last_event > 300000){

        last_event = current_time;

        if(gpio == BUTTON_A){
            if(current_number == 9){
                current_number -= 9;
            }else{
                current_number += 1;
            }
            printf("Número Atual: %d", current_number);

        }else if(gpio == BUTTON_B){
            if(current_number == 0){
                current_number += 9;
            }else{
                current_number -= 1;
            }
            printf("Número Atual: %d", current_number);
        }
    }

}

void blink_red(){ // Função para piscar o LED vermelho 5 vezes consecutivas por segundo
    for (int i = 0; i < 5; i++){
        gpio_put(RED_LED, 1);
        sleep_ms(100);
        gpio_put(RED_LED, 0);
        sleep_ms(100);
    } 
}

int main()
{
    stdio_init_all();
    PIO pio = pio0;

    bool clock_config = set_sys_clock_khz(128000, false);

    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_put(RED_LED, 0);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

// Adiciona o programa ao pio às maquinas de estado
    uint offset = pio_add_program(pio, &ws2812_program);
    // Reivindica uma máquina de estado livre para executar o programa PIO
    uint sm = pio_claim_unused_sm(pio, true); 
    // Configura a SM para executar o programa PIO
    ws2812_program_init(pio, sm, offset, PIN, 800000, false); 

    // Configuração da interrupção com callback para botão A
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_pressed); 

    // Configuração da interrupção com callback para botão B
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_pressed);

    printf("Sistema iniciado\n");

    while (true)
    {
        
        blink_red();
        draw_number(numbers[current_number], 0, pio, sm, 0.2, 0.2, 0.0);
        sleep_ms(100); // Delay para evitar sobrecarga
        
    }

}
