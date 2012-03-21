//*****************************************************************************
//
//! @page xspi_testcase loopback and interrupt test
//!
//! File: @ref spitest03.c
//!
//! <h2>Description</h2>
//! This module implements the test sequence for the xsysctl sub component.<br><br>
//! - \p Board: HT32F125x Development Board <br><br>
//! - \p Last-Time(about): 0.5s <br><br>
//! - \p Phenomenon: Success or failure information will be printed on the UART. <br><br>
//! .
//!
//! <h2>Preconditions</h2>
//! The module requires the following options:<br><br>
//! - \p Option-define:
//! <br>(1)None.<br><br>
//! - \p Option-hardware:
//! <br>(1)Connect an USB cable to the development board.<br><br>
//! - \p Option-OtherModule:
//! <br>Connect an COM cable to the development board.<br>
//! .
//! In case some of the required options are not enabled then some or all tests
//! may be skipped or result FAILED.<br>
//!
//! <h2>Test Cases</h2>
//! The module contain those sub tests:<br><br>
//! - \subpage test_xspi_register
//! .
//! \file spitest01.c
//! \brief xspi test source file
//! \brief xspi test header file <br>
//
//*****************************************************************************
#include "xhw_memmap.h"
#include "test.h"
#include "xspi.h"
#include "xhw_spi.h"
#include "xsysctl.h"

//*****************************************************************************
//
//!\page test_xspi_register test_xsysctl_register
//!
//!<h2>Description</h2>
//!Test xspi register. <br>
//!
//
//*****************************************************************************
unsigned long ulSourceData[] = {1, 2, 3, 4,
                                5, 6, 7, 8};

unsigned long ulDestData[16];

unsigned long i = 0;

unsigned long SPI0Callback(void *pvCBData,  unsigned long ulEvent,
                                       unsigned long ulMsgParam,
                                       void *pvMsgData)
{  
  //  SPISSClear(SPI0_BASE);
  //  SPISSSet(SPI0_BASE);
  //  xSPIDataPutNonBlocking(SPI0_BASE, ulSourceData[i]);
  //  xSPIDataGetNonBlocking(xSPI0_BASE, &ulDestData[i]);
  //  xSPIDataPutNonBlocking(xSPI0_BASE, 0xff);
  //  i++;
	//
	// Write a date to exti the interrupt
	//

	if(i >=8)
	{
	    xSPIIntDisable(SPI0_BASE, xSPI_INT_EOT);
	}
    return 0;
}
//*****************************************************************************
//
//! \brief Get the Test description of xspi004 register test.
//!
//! \return the desccription of the xspi001 test.
//
//*****************************************************************************
static char* xSpi004GetTest(void)
{
    return "xspi, 002, xspi loopback and interrupt test";
}

//*****************************************************************************
//
//! \brief something should do before the test execute of xspi001 test.
//!
//! \return None.
//
//*****************************************************************************
static void xSpi004Setup(void)
{    
    //
    // Set SysClk 8MHz using Extern 8M oscillator
    //
    xSysCtlClockSet(8000000, xSYSCTL_OSC_MAIN | xSYSCTL_XTAL_8MHZ);

    //
    // Enable Peripheral SPI0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SPI0);  
	SysCtlPeripheralEnable(SYSCTL_PERIPH_AFIO); 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); 
}


//*****************************************************************************
//
//! \brief something should do after the test execute of xspi001 test.
//!
//! \return None.
//
//*****************************************************************************
static void xSpi004TearDown(void)
{   
    SysCtlPeripheralDisable(SYSCTL_PERIPH_SPI0);
}

//*****************************************************************************
//
//! \brief xspi 004 test of SPI0 initialization.
//!
//! \return None.
//
//*****************************************************************************
static void SPI0MasterInit(void)
{
    //
    // Configure Some GPIO pins as SPI Mode
    //
    xSPinTypeSPI(SPI0CLK, PB13);
	xSPinTypeSPI(SPI0MOSI, PB15);
	xSPinTypeSPI(SPI0MISO, PB14);
	xSPinTypeSPI(SPI0CS, PA7);

    //
    // Master Mode polarity 0,Rx latched at rising edge Tx changed at rising edge
    // 200000Hz 8Bits Data windth  SPI MSB First
    //
    xSPIConfigSet(SPI0_BASE, 10000, SPI_FORMAT_MODE_5 | SPI_DATA_WIDTH16 | 
                                               SPI_MSB_FIRST | SPI_MODE_MASTER);
    //
    // Software Control Transfer Select SS0
    //
    xSPISSSet(SPI0_BASE, xSPI_SS_SOFTWARE, 0);

//	SPISSClear(SPI0_BASE);
    
//    xIntEnable(INT_SPI0);

	xSPIIntCallbackInit(SPI0_BASE, SPI0Callback);
    xSPIEnable(SPI0_BASE);
//    xIntPrioritySet(INT_SPI0, 1);
//	xSPIIntEnable(SPI0_BASE, xSPI_INT_EOT);
//	SPIFIFOLevelSet(SPI0_BASE, SPI_FSR_RX, 5);
	
}

//*****************************************************************************
//
//! \brief xspi 004 test of SPI Loopback between SPI0MISO and SPI0MOSI.
//!
//! \return None.
//
//*****************************************************************************
static void xspi_SpiLoopbackTest_test(void)
{

    for(i = 0; i < 8; i++)
    {
        xSPIDataPut(SPI0_BASE, ulSourceData[i]);
		//xSPIDataPutNonBlocking(SPI0_BASE, ulSourceData[i]);
		xSPIDataGet(xSPI0_BASE, &ulDestData[i]);
		//xSPIDataGetNonBlocking(xSPI0_BASE, &ulDestData[i]);
        //TestAssertQBreak("a", "Transfer Over Interrupy fialed!", 0xFFFFFFFF);
		//ulDestData[i] = SPISingleDataReadWrite(SPI0_BASE, ulSourceData[i]);
	//	xHWREG(SPI0_BASE + SPI_DR) = ulSourceData[i];
	//	ulDestData[i] = xHWREG(SPI0_BASE + SPI_DR);
    } 

    for(i = 0; i < 8; i++)
    {
        TestAssert((ulSourceData[i] == ulDestData[i]), "xspi API error!");
    }
}

//*****************************************************************************
//
//! \brief xspi 004 test execute main body.
//!
//! \return None.
//
//*****************************************************************************
static void xSpi004Execute(void)
{
    SPI0MasterInit();
    xspi_SpiLoopbackTest_test();
}


//
// xspi register test case struct.
//
const tTestCase sTestXSpi004Register = {
    xSpi004GetTest,
    xSpi004Setup,
    xSpi004TearDown,
    xSpi004Execute
};

//
// Xspi test suits.
//
const tTestCase * const psPatternXspi01[] =
{
    &sTestXSpi004Register,
    0
};
