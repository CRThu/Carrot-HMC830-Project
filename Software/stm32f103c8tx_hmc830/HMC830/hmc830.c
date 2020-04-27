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

// HMC Mode VCO SubSystem Write
void HMC830_HMC_VCO_Write(uint8_t vco_address,uint32_t vco_data)
{
    uint32_t data = 0x000000;
    data |= HMC830_REG05H_VCO_SUBSYSTEM_ID_MASK & (HMC830_REG05H_VCO_SUBSYSTEM_ID_WB << HMC830_REG05H_VCO_SUBSYSTEM_ID_OFFSET);
    data |= HMC830_REG05H_VCO_SUBSYSTEM_REG_ADDRESS_MASK & (vco_address << HMC830_REG05H_VCO_SUBSYSTEM_REG_ADDRESS_OFFSET);
    data |= HMC830_REG05H_VCO_SUBSYSTEM_DATA_MASK & (vco_data << HMC830_REG05H_VCO_SUBSYSTEM_DATA_OFFSET);
    
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, data);
}

uint32_t HMC830_HMC_Read_Chip_ID(void)
{
    return HMC830_REG00H_CHIP_ID_MASK & HMC830_HMC_Read(HMC830_REG00H_ID);
}

void HMC830_HMC_Write_REFDIV(uint16_t refdiv)
{
    uint32_t data = 0x000000;
    
    if(refdiv > HMC830_REG02H_RDIV_MAX)
        refdiv = HMC830_REG02H_RDIV_MAX;
    if(refdiv < HMC830_REG02H_RDIV_MIN)
        refdiv = HMC830_REG02H_RDIV_MIN;
    
    data = HMC830_REG02H_RDIV_MASK & refdiv;
    
    HMC830_HMC_Write(HMC830_REG02H_REFDIV, data);
}

void HMC830_HMC_Write_NDIV(double ndiv)
{   
    uint32_t data_intg = 0x000000;
    uint32_t data_frac = 0x000000;
    
    uint32_t ndiv_integer = (uint32_t)ndiv;
    uint32_t ndiv_fractional = (uint32_t)((double)(ndiv - (double)ndiv_integer) * 16777216 + 0.5);    // N * 2^-24
    
    if(ndiv == (double)ndiv_integer)
    {
        // integer
        if(ndiv_integer > HMC830_REG03H_INTG_INTEGER_MAX)
            ndiv_integer = HMC830_REG03H_INTG_INTEGER_MAX;
        if(ndiv_integer < HMC830_REG03H_INTG_INTEGER_MIN)
            ndiv_integer = HMC830_REG03H_INTG_INTEGER_MIN;
    }
    else
    {
        // fractional
        if(ndiv_integer > HMC830_REG03H_INTG_FACTIONAL_MAX)
            ndiv_integer = HMC830_REG03H_INTG_FACTIONAL_MAX;
        if(ndiv_integer < HMC830_REG03H_INTG_FACTIONAL_MIN)
            ndiv_integer = HMC830_REG03H_INTG_FACTIONAL_MIN;
        
        if(ndiv_fractional > HMC830_REG04H_FRAC_MAX)
            ndiv_fractional = HMC830_REG04H_FRAC_MAX;
        if(ndiv_fractional < HMC830_REG04H_FRAC_MIN)
            ndiv_fractional = HMC830_REG04H_FRAC_MIN;
    }
    
    data_intg = HMC830_REG03H_INTG_MASK & ndiv_integer;
    data_frac = HMC830_REG04H_FRAC_MASK & ndiv_fractional;
    
    HMC830_HMC_Write(HMC830_REG03H_INTEGER_PART, data_intg);
    HMC830_HMC_Write(HMC830_REG04H_FRACTIONAL_PART, data_frac);
}

void HMC830_HMC_Write_VCO_General_Setting(uint8_t kdiv, uint8_t GAIN)
{
    uint32_t data = 0x000;
    
    if(kdiv > HMC830_VCO_REG02H_KDIV_MAX)
        kdiv = HMC830_VCO_REG02H_KDIV_MAX;
    if(kdiv < HMC830_VCO_REG02H_KDIV_MIN)
        kdiv = HMC830_VCO_REG02H_KDIV_MIN;
    
    if((kdiv != 1) && (kdiv % 2))
        kdiv--;
    
    if(GAIN > HMC830_VCO_REG02H_GAIN_3)
        GAIN = HMC830_VCO_REG02H_GAIN_3;
    if(GAIN < HMC830_VCO_REG02H_GAIN_0)
        GAIN = HMC830_VCO_REG02H_GAIN_0;
    
    data |= HMC830_VCO_REG02H_RF_DIVIDE_RATIO_MASK & (kdiv << HMC830_VCO_REG02H_RF_DIVIDE_RATIO_OFFSET);
    data |= HMC830_VCO_REG02H_RF_OUTPUT_BUF_GAIN_CTL_MASK & (GAIN << HMC830_VCO_REG02H_RF_OUTPUT_BUF_GAIN_CTL_OFFSET);
    data |= HMC830_VCO_REG02H_DIV_OUTPUT_STAGE_GAIN_CTL_MASK & ((kdiv < 4 ? 1 : 0) << HMC830_VCO_REG02H_DIV_OUTPUT_STAGE_GAIN_CTL_OFFSET);
    
    // Write Recommand Value
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, HMC830_VCO_REG05H_RECOMMAND);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, HMC830_VCO_REG04H_RECOMMAND);
    
    // VCO_REG02H
    HMC830_HMC_VCO_Write(HMC830_VCO_REG02H_BIASES, data);
    
    // VCO AutoCal
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x0);
}
void HMC830_HMC_Write_PFD_General_Setting(uint8_t PFD_MODE)
{
    // fPFD = 50MHz
    if(PFD_MODE == HMC830_INTEGER_MODE)
    {
        HMC830_HMC_Write(HMC830_REG06H_SD_CFG, 0x2003CA);
        HMC830_HMC_Write(HMC830_REG07H_LOCK_DETECT, 0xCCD);
        HMC830_HMC_Write(HMC830_REG0AH_VCO_AUTOCAL_CFG, 0x2046);
        HMC830_HMC_Write(HMC830_REG0BH_PD, 0x7C061);
        HMC830_HMC_Write(HMC830_REG0FH_GPO_SPI_RDIV, 0x81);
    }
    if(PFD_MODE == HMC830_FRACTIONAL_MODE)
    {
        HMC830_HMC_Write(HMC830_REG06H_SD_CFG, 0x200B4A);
        HMC830_HMC_Write(HMC830_REG07H_LOCK_DETECT, 0xCCD);
        HMC830_HMC_Write(HMC830_REG0AH_VCO_AUTOCAL_CFG, 0x2046);
        HMC830_HMC_Write(HMC830_REG0BH_PD, 0x7C021);
        HMC830_HMC_Write(HMC830_REG0FH_GPO_SPI_RDIV, 0x81);
    }
}

void HMC830_HMC_Write_Charge_Pump_Current(float Icp, uint8_t PFD_MODE, uint16_t fVCO, uint16_t fPFD)
{
    uint32_t data = 0x000000;
    
    float Icp_offset = (2.5 + 4.0 * (1000.0/(float)fVCO)) * (float)fPFD * Icp;  // uA
    
    if(Icp > HMC830_REG09H_CP_GAIN_MAX)
        Icp = HMC830_REG09H_CP_GAIN_MAX;
    if(Icp < HMC830_REG09H_CP_GAIN_MIN)
        Icp = HMC830_REG09H_CP_GAIN_MIN;
    
    if(Icp_offset > HMC830_REG09H_OFFSET_MAGNITUDE_MAX)
        Icp_offset = HMC830_REG09H_OFFSET_MAGNITUDE_MAX;
    if(Icp_offset < HMC830_REG09H_OFFSET_MAGNITUDE_MIN)
        Icp_offset = HMC830_REG09H_OFFSET_MAGNITUDE_MIN;
    
    data |= HMC830_REG09H_CP_DN_GAIN_MASK & ((uint8_t)(Icp * 1000.0 / 20.0 + 0.5) << HMC830_REG09H_CP_DN_GAIN_OFFSET);
    data |= HMC830_REG09H_CP_UP_GAIN_MASK & ((uint8_t)(Icp * 1000.0 / 20.0 + 0.5) << HMC830_REG09H_CP_UP_GAIN_OFFSET);
    data |= HMC830_REG09H_OFFSET_MAGNITUDE_MASK & ((uint8_t)(Icp_offset / 5.0 + 0.5) << HMC830_REG09H_OFFSET_MAGNITUDE_OFFSET);
    data |= HMC830_REG09H_OFFSET_UP_ENABLE_MASK & (0 << HMC830_REG09H_OFFSET_UP_ENABLE_OFFSET);
    data |= HMC830_REG09H_OFFSET_DN_ENABLE_MASK & ((PFD_MODE == HMC830_FRACTIONAL_MODE ? 1 : 0) << HMC830_REG09H_OFFSET_DN_ENABLE_OFFSET);
    data |= HMC830_REG09H_HIKCP_MASK & (0 << HMC830_REG09H_HIKCP_OFFSET);
    
    HMC830_HMC_Write(HMC830_REG09H_CHARGE_PUMP, data);
}

void HMC830_HMC_Write_Output_Mode(uint8_t OUTPUT_MODE)
{
    uint32_t data = HMC830_VCO_REG03H_CONFIG_DEFAULT;
    
    data = (data & ~HMC830_VCO_REG03H_RF_BUFFER_SE_ENABLE_MASK)
           |(HMC830_VCO_REG03H_RF_BUFFER_SE_ENABLE_MASK & (OUTPUT_MODE << HMC830_VCO_REG03H_RF_BUFFER_SE_ENABLE_OFFSET));
    
    HMC830_HMC_VCO_Write(HMC830_VCO_REG03H_CONFIG, data);
    
    // VCO AutoCal
    HMC830_HMC_Write(HMC830_REG0AH_VCO_AUTOCAL_CFG, 0x3046);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x0);
    HMC830_HMC_Write(HMC830_REG0AH_VCO_AUTOCAL_CFG, 0x2046);
}

uint8_t HMC830_HMC_Read_Lock_Detect(void)
{
    uint32_t data = HMC830_HMC_Read(HMC830_REG12H_GPO2);
    return (HMC830_REG12H_LOCK_DETECT_MASK & data) >> HMC830_REG12H_LOCK_DETECT_OFFSET;
}