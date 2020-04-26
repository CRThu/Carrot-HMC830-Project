/**
  ******************************************************************************
  * File Name          : HMC830.c
  * Version            : v1.0
  * Date               : 2020.04.25
  * Description        : This file provides code for the configuration
  *                      of HMC830.
  *
  * Library            : HMC830_LIB
  * Version            : v1.0
  * Dependency         : ---
  * Description        : HMC830 Communication Library.
  *
  *                      COPYRIGHT(c) 2020 yummycarrot
  *                      https://www.github.com/crthu
  *
  ******************************************************************************
  */

#include "hmc830.h"

// Delay
void HMC830_Delay(void)
{
    __nop();
}

// Initial (Mode Select)
void HMC830_Init(uint8_t MODE)
{
    HMC830_SCK(0);
    HMC830_SEN(0);
    HMC830_SDI(0);
    
    HAL_Delay(5);
    if(MODE == HMC830_HMC_MODE)
    {
        HMC830_SEN(1);
        HMC830_Delay();
        HMC830_SCK(1);
    }
    // TODO HMC830_OPEN_MODE
    
    HMC830_Delay();
}

// HMC Mode Write
void HMC830_HMC_Write(uint8_t address,uint32_t data)
{
    // Begin
    HMC830_SCK(0);
    HMC830_SDI(0);
    HMC830_SEN(0);
    HMC830_Delay();
    
    HMC830_SEN(1);
    // CLK1(Pos):/WR
    HMC830_SDI(0);  // /WR
    HMC830_Delay();
    HMC830_SCK(1);  // CLK1 Posedge
    HMC830_Delay();
    
    // CLK2-CLK7(Pos):A5-A0(with CLK1 Negedge)
    for(int i=0;i<6;i++)
    {
        HMC830_SCK(0);
        HMC830_SDI(address & (1<<(5-i)));   // a5-a0
        HMC830_Delay();
        HMC830_SCK(1);
        HMC830_Delay();
    }
    
    // CLK8-CLK31(Pos):D23-D0(with CLK7 Negedge)
    for(int i=0;i<24;i++)
    {
        HMC830_SCK(0);
        HMC830_SDI(data & (1<<(23-i)));   // d23-d0
        HMC830_Delay();
        HMC830_SCK(1);
        HMC830_Delay();
    }
    
    // CLK32(Pos)(with CLK31 Negedge)
    HMC830_SCK(0);
    for(uint8_t i=0;i<5;i++)
        HMC830_Delay();
    HMC830_SCK(1);
    HMC830_Delay();
    HMC830_SCK(0);
    HMC830_Delay();
    
    HMC830_SEN(0);
    HMC830_Delay();
}

// HMC Mode Read
uint32_t HMC830_HMC_Read(uint8_t address)
{
    uint32_t data = 0x00000000;
    
    // Begin
    HMC830_SCK(0);
    HMC830_SDI(0);
    HMC830_SEN(0);
    HMC830_Delay();
    
    HMC830_SEN(1);
    // CLK1(Pos):RD
    HMC830_SDI(1);
    HMC830_Delay();
    HMC830_SCK(1);  // CLK1 Posedge
    HMC830_Delay();
    
    // CLK2-CLK7(Pos):A5-A0(with CLK1 Negedge)
    for(int i=0;i<6;i++)
    {
        HMC830_SCK(0);
        HMC830_SDI(address & (1<<(5-i)));   // a5-a0
        HMC830_Delay();
        HMC830_SCK(1);
        HMC830_Delay();
    }
    
    // CLK8(Pos)(with CLK7 Negedge)
    HMC830_SCK(0);
    for(uint8_t i=0;i<5;i++)
        HMC830_Delay();
    HMC830_SCK(1);
    HMC830_Delay();
    
    // CLK8-CLK31(Neg):D23-D0(with CLK32 Posedge)
    for(int i=0;i<24;i++)
    {
        HMC830_SCK(0);
        data |= (HMC830_SDO << (23-i));   // d23-d0
        HMC830_Delay();
        HMC830_SCK(1);
        HMC830_Delay();
    }
    
    // CLK32(Neg)
    HMC830_SCK(0);
    HMC830_Delay();
    
    HMC830_SEN(0);
    HMC830_Delay();
    
    return data;
}
