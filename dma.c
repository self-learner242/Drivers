#include "dma.h"
#include "rcc.h"
#include "systick.h"

DMA_Status_t DMA_Init(DMA_Handle_t* hdma){
    // Error Checking
    if (hdma == NULL || hdma->Instance == NULL) return DMA_ERROR;
    if (hdma->Channel > 7) return DMA_ERROR;
    
    // Enable Clock for DMA
    DMA_TypeDef_t* DMAx;
    if((uint32_t)hdma->Instance >= DMA2_BASE){
        DMAx = DMA2;
        RCC_EnableAHB1Clock(RCC_AHB1ENR_DMA2EN);
    }
    else{
        DMAx = DMA1;
        RCC_EnableAHB1Clock(RCC_AHB1ENR_DMA1EN);
    }
    // 0. Disable and Choose Stream, Channel
    // 1. Clear all interrupt flag
    // 2. Set destination buffer
    // 3. Set source buffer
    // 4. Set Length
    // 5. Memory Increment
    // 6. Transfer direction
    // 7. Mode
}