#ifndef SMALL_DRIVER_UART_CONTROL_H_
#define SMALL_DRIVER_UART_CONTROL_H_

#include "zf_common_headfile.h"

// 说明：
// - 该模块将逐飞无刷双驱的 7 字节协议移植到 AI8051U(Keil C251) 工程。
// - 当前配置使用 UART_1 作为无刷驱动通信口（P3.1/P3.0）。
//   注意：如果你的工程还在用 debug_init()/printf 且 DEBUG_UART_INDEX 也是 UART_1，会和无刷共用串口产生冲突。
//   建议将调试串口换到 UART_2/3/4，或在调试阶段不要初始化无刷串口。
// - 如果你的硬件接线不同，只需要改下面这些宏即可。

// -------------------- 设备1（默认启用） --------------------
#define SMALL_DRIVER_UART_1            (UART_1)
#define SMALL_DRIVER_BAUDRATE_1        (460800)
#define SMALL_DRIVER_TX_1              (UART1_TX_P31)
#define SMALL_DRIVER_RX_1              (UART1_RX_P30)

// -------------------- 设备2（可选启用） --------------------
// 如不需要第二块驱动板，可以不用调用 small_driver_uart_init(SMALL_DRIVER_2)
#define SMALL_DRIVER_UART_2            (UART_2)
#define SMALL_DRIVER_BAUDRATE_2        (460800)
#define SMALL_DRIVER_TX_2              (UART2_TX_P13)
#define SMALL_DRIVER_RX_2              (UART2_RX_P12)

typedef struct
{
    uint8 send_data_buffer[7];
    uint8 receive_data_buffer[7];
    uint8 receive_data_count;
    uint8 sum_check_data;

    int16 receive_left_speed_data;
    int16 receive_right_speed_data;

} small_device_value_struct;

typedef enum
{
    SMALL_DRIVER_1 = 0,
    SMALL_DRIVER_2 = 1,
} small_device_enum;

extern small_device_value_struct xdata motor_value_1;
extern small_device_value_struct xdata motor_value_2;

// 轮询版：从 UART DMA 接收缓冲中取字节并解析
void uart_control_callback(small_device_enum device_num);

void small_driver_set_duty(small_device_enum device_num, int16 left_duty, int16 right_duty);
void small_driver_get_speed(small_device_enum device_num);
void small_driver_uart_init(small_device_enum device_num);

#endif
