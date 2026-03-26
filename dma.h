#ifndef DMA_H
#define DMA_H

#include <stdint.h>

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t NDTR;
    volatile uint32_t PAR;
    volatile uint32_t M0AR;
    volatile uint32_t M1AR;
    volatile uint32_t FCR;
} DMA_Stream_TypeDef_t;

typedef struct {
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
    DMA_Stream_TypeDef_t Stream[8];
} DMA_TypeDef_t;

typedef enum {
    DMA_OK = 0,
    DMA_ERROR,
    DMA_BUSY,
    DMA_TIMEOUT
} DMA_Status_t;

/* BASE ADDR */
#define DMA1_BASE               0x40026000
#define DMA2_BASE               0x40026400

/* INSTANCE */
#define DMA1                    ((DMA_TypeDef_t*)DMA1_BASE)
#define DMA2                    ((DMA_TypeDef_t*)DMA2_BASE)

/* DMA1 STRESAM 0 - 7 */
#define DMA1_STREAM0_BASE       (DMA1_BASE + 0x010U)
#define DMA1_STREAM1_BASE       (DMA1_BASE + 0x028U)
#define DMA1_STREAM2_BASE       (DMA1_BASE + 0x040U)
#define DMA1_STREAM3_BASE       (DMA1_BASE + 0x058U)
#define DMA1_STREAM4_BASE       (DMA1_BASE + 0x070U)
#define DMA1_STREAM5_BASE       (DMA1_BASE + 0x088U)
#define DMA1_STREAM6_BASE       (DMA1_BASE + 0x0A0U)
#define DMA1_STREAM7_BASE       (DMA1_BASE + 0x0B8U)

/* DMA2 STRESAM 0 - 7 */
#define DMA2_STREAM0_BASE       (DMA2_BASE + 0x010U)
#define DMA2_STREAM1_BASE       (DMA2_BASE + 0x028U)
#define DMA2_STREAM2_BASE       (DMA2_BASE + 0x040U)
#define DMA2_STREAM3_BASE       (DMA2_BASE + 0x058U)
#define DMA2_STREAM4_BASE       (DMA2_BASE + 0x070U)
#define DMA2_STREAM5_BASE       (DMA2_BASE + 0x088U)
#define DMA2_STREAM6_BASE       (DMA2_BASE + 0x0A0U)
#define DMA2_STREAM7_BASE       (DMA2_BASE + 0x0B8U)

/* INSTANCE */
// DMA1_STREAMx
#define DMA1_STREAM0            ((DMA_Stream_TypeDef_t*)DMA1_STREAM0_BASE)
#define DMA1_STREAM1            ((DMA_Stream_TypeDef_t*)DMA1_STREAM1_BASE)
#define DMA1_STREAM2            ((DMA_Stream_TypeDef_t*)DMA1_STREAM2_BASE)
#define DMA1_STREAM3            ((DMA_Stream_TypeDef_t*)DMA1_STREAM3_BASE)
#define DMA1_STREAM4            ((DMA_Stream_TypeDef_t*)DMA1_STREAM4_BASE)
#define DMA1_STREAM5            ((DMA_Stream_TypeDef_t*)DMA1_STREAM5_BASE)
#define DMA1_STREAM6            ((DMA_Stream_TypeDef_t*)DMA1_STREAM6_BASE)
#define DMA1_STREAM7            ((DMA_Stream_TypeDef_t*)DMA1_STREAM7_BASE)

// DMA2
#define DMA2_STREAM0            ((DMA_Stream_TypeDef_t*)DMA2_STREAM0_BASE)
#define DMA2_STREAM1            ((DMA_Stream_TypeDef_t*)DMA2_STREAM1_BASE)
#define DMA2_STREAM2            ((DMA_Stream_TypeDef_t*)DMA2_STREAM2_BASE)
#define DMA2_STREAM3            ((DMA_Stream_TypeDef_t*)DMA2_STREAM3_BASE)
#define DMA2_STREAM4            ((DMA_Stream_TypeDef_t*)DMA2_STREAM4_BASE)
#define DMA2_STREAM5            ((DMA_Stream_TypeDef_t*)DMA2_STREAM5_BASE)
#define DMA2_STREAM6            ((DMA_Stream_TypeDef_t*)DMA2_STREAM6_BASE)
#define DMA2_STREAM7            ((DMA_Stream_TypeDef_t*)DMA2_STREAM7_BASE)

/* CHANNEL */
typedef enum{
    DMA_CHANNEL_0 = 0,
    DMA_CHANNEL_1,
    DMA_CHANNEL_2,
    DMA_CHANNEL_3,
    DMA_CHANNEL_4,
    DMA_CHANNEL_5,
    DMA_CHANNEL_6,
    DMA_CHANNEL_7
} DMA_Channel_t;

/* Direction */
typedef enum{
    DMA_DIR_PERIPH_TO_MEM   = 0,
    DMA_DIR_MEM_TO_PERIPH,
} DMA_Direction_t;

/* PRIORITY */
typedef enum{
    DMA_PRIORITY_LOW        = 0,
    DMA_PRIORITY_MED,
    DMA_PRIORITY_HIGH,
    DMA_PRIORITY_VERY_HIGH
} DMA_Priority_t;

/* MODE */
typedef enum{ 
    DMA_MODE_NORMAL = 0, 
    DMA_MODE_CIRCULAR 
} DMA_Mode_t;

typedef struct{
    DMA_Stream_TypeDef_t*   Instance;   // DMA1_Stream5, DMA1_Stream0
    DMA_Channel_t           Channel;    // Channel 0 - 7
    DMA_Direction_t         Direction;  // Mem to Peri or Peri to Mem
    DMA_Priority_t          Priority;   // Low, Med, Hi, VH
    DMA_Mode_t              Mode;       // Normal or Circular
} DMA_Handle_t;

/* BIT MASKS */

/* SxCR bits */
#define DMA_CR_EN           (1U << 0)
#define DMA_CR_CIRC         (1U << 8)
#define DMA_CR_DIR_Pos      6U
#define DMA_CR_DIR_Msk      (0x3U << DMA_CR_DIR_Pos)
#define DMA_CR_PL_Pos       16U
#define DMA_CR_PL_Msk       (0x3U << DMA_CR_PL_Pos)
#define DMA_CR_CHSEL_Pos    25U
#define DMA_CR_CHSEL_Msk    (0x7U << DMA_CR_CHSEL_Pos)

/*APIs*/
DMA_Status_t DMA_Init(DMA_Handle_t* hdma);

#endif /* DMA_H */