#include "align.hpp"
#include "stm32g4/stm32g4xx.h"

Pin::Pin(GPIO_TypeDef* port, uint8_t pinIdx, PinMode pm, PinConfig cfg){
    pinIndex = pinIdx;
    pinReg = port;
    pinMode = pm;
    pinConfig = cfg;
    isSet = false;

    // Set pin to correct mode in MODER register
    MODIFY_REG(pinReg->MODER, (uint32_t) 0b11 << (pinIdx * 2), (uint32_t) pinMode << (pinIdx * 2));
    
    // Default to medium speed as of now
    MODIFY_REG(pinReg->OSPEEDR, (uint32_t) 0b11 << (pinIdx * 2), (uint32_t) 0b01 << (pinIdx * 2));

    if(pm == PinMode::Output){
        MODIFY_REG(pinReg->PUPDR, (uint32_t) 0b11 << (pinIdx * 2), (uint32_t) pinConfig << (pinIdx * 2));
    }else{
        MODIFY_REG(pinReg->PUPDR, (uint32_t) 0b11 << (pinIdx * 2), (uint32_t) 0b00 << (pinIdx * 2));
    }

}

void Pin::toggle(){

    if(isSet){
        pinReg->BSRR |= 1 << (16 + pinIndex);
    }else{
        pinReg->BSRR |= 1 << pinIndex;
    }
    isSet = !isSet;

}

// Aims for 170 MHz
RCC_Controller::RCC_Controller(RCC_TypeDef* rccRegister, RCC_Config rccConfig){
    rccReg = rccRegister;
    rccCfg = rccConfig;

    // If high speed external enabled
    if(rccCfg.hse){
        // Enable external clock source

        if(rccCfg.hse_bypass){
            rccReg->CR |= RCC_CR_HSEBYP;
        }
        rccReg->CR |= RCC_CR_HSEON;
        while(!((rccReg->CR & RCC_CR_HSERDY_Msk) == RCC_CR_HSERDY_Msk)); // Wait until HSE is ready.

        // As for now, it is assumed there is a 12MHz crystal in use
        hsFreq = 12000000;

    }else{ // Else fallback on internal
        // Use internal clocksource
        hsFreq = 16000000;

    }
}

void RCC_Controller::enable_gpio(){
    rccReg->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    rccReg->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    rccReg->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    rccReg->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    rccReg->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
    rccReg->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
    rccReg->AHB2ENR |= RCC_AHB2ENR_GPIOGEN;
}

void RCC_Controller::delay_ms(uint32_t ms){

}