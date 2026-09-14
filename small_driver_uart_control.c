#include "small_driver_uart_control.h"

// 逐飞库 UART 驱动中提供，原型为：void uart_dma_init(uart_index_enum uart_n);
// 工程里若未显式包含到该声明，Keil 会“猜”成 int uart_dma_init()，导致参数/符号不匹配并最终链接失败。
extern void uart_dma_init(uart_index_enum uart_n);

small_device_value_struct xdata motor_value_1;
small_device_value_struct xdata motor_value_2;

static void small_driver_parse_byte(small_device_value_struct *motor_value_temp, uint8 receive_data)
{
    uint8 i;

    if((receive_data == 0xA5) && (motor_value_temp->receive_data_buffer[0] != 0xA5))
    {
        motor_value_temp->receive_data_count = 0;
    }

    motor_value_temp->receive_data_buffer[motor_value_temp->receive_data_count++] = receive_data;

    if(motor_value_temp->receive_data_count >= 7)
    {
        if(motor_value_temp->receive_data_buffer[0] == 0xA5)
        {
            motor_value_temp->sum_check_data = 0;
            for(i = 0; i < 6; i++)
            {
                motor_value_temp->sum_check_data += motor_value_temp->receive_data_buffer[i];
            }

            if(motor_value_temp->sum_check_data == motor_value_temp->receive_data_buffer[6])
            {
                if(motor_value_temp->receive_data_buffer[1] == 0x02)
                {
                    motor_value_temp->receive_left_speed_data  = (int16)(((int16)motor_value_temp->receive_data_buffer[2] << 8) |
                                                                          (int16)motor_value_temp->receive_data_buffer[3]);
                    motor_value_temp->receive_right_speed_data = (int16)(((int16)motor_value_temp->receive_data_buffer[4] << 8) |
                                                                          (int16)motor_value_temp->receive_data_buffer[5]);
                }
            }
        }

        motor_value_temp->receive_data_count = 0;
        memset(motor_value_temp->receive_data_buffer, 0, 7);
    }
}

// 逐飞库 UART DMA 中断回调：每次进来带 1 个字节
static void small_driver_uart1_irq(uint8 dat)
{
    small_driver_parse_byte(&motor_value_1, dat);
}

static void small_driver_uart2_irq(uint8 dat)
{
    small_driver_parse_byte(&motor_value_2, dat);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无刷驱动 串口接收回调函数（轮询版）
// 参数说明     device_num
// 返回参数     void
// 备注信息     若你已经调用 small_driver_uart_init()，会自动挂接到 uartX_irq_handler，通常无需再手动轮询。
//-------------------------------------------------------------------------------------------------------------------
void uart_control_callback(small_device_enum device_num)
{
    uint8 receive_data;
    uint8 receive_flag;
    small_device_value_struct *motor_value_temp;
    uart_index_enum uart_index;

    if(device_num == SMALL_DRIVER_1)
    {
        motor_value_temp = &motor_value_1;
        uart_index = SMALL_DRIVER_UART_1;
    }
    else
    {
        motor_value_temp = &motor_value_2;
        uart_index = SMALL_DRIVER_UART_2;
    }

    // 轮询取字节，取到就喂解析器
    do
    {
        receive_flag = uart_query_byte(uart_index, &receive_data);
        if(receive_flag)
        {
            small_driver_parse_byte(motor_value_temp, receive_data);
        }
    } while(receive_flag);
}

void small_driver_set_duty(small_device_enum device_num, int16 left_duty, int16 right_duty)
{
    uint8 i;
    small_device_value_struct *motor_value_temp;
    uart_index_enum uart_index;

    if(device_num == SMALL_DRIVER_1)
    {
        motor_value_temp = &motor_value_1;
        uart_index = SMALL_DRIVER_UART_1;
    }
    else
    {
        motor_value_temp = &motor_value_2;
        uart_index = SMALL_DRIVER_UART_2;
    }

    motor_value_temp->send_data_buffer[0] = 0xA5;
    motor_value_temp->send_data_buffer[1] = 0x01;
    motor_value_temp->send_data_buffer[2] = (uint8)((left_duty & 0xFF00) >> 8);
    motor_value_temp->send_data_buffer[3] = (uint8)(left_duty & 0x00FF);
    motor_value_temp->send_data_buffer[4] = (uint8)((right_duty & 0xFF00) >> 8);
    motor_value_temp->send_data_buffer[5] = (uint8)(right_duty & 0x00FF);

    motor_value_temp->send_data_buffer[6] = 0;
    for(i = 0; i < 6; i++)
    {
        motor_value_temp->send_data_buffer[6] += motor_value_temp->send_data_buffer[i];
    }

    uart_write_buffer(uart_index, motor_value_temp->send_data_buffer, 7);
}

void small_driver_get_speed(small_device_enum device_num)
{
    small_device_value_struct *motor_value_temp;
    uart_index_enum uart_index;

    if(device_num == SMALL_DRIVER_1)
    {
        motor_value_temp = &motor_value_1;
        uart_index = SMALL_DRIVER_UART_1;
    }
    else
    {
        motor_value_temp = &motor_value_2;
        uart_index = SMALL_DRIVER_UART_2;
    }

    motor_value_temp->send_data_buffer[0] = 0xA5;
    motor_value_temp->send_data_buffer[1] = 0x02;
    motor_value_temp->send_data_buffer[2] = 0x00;
    motor_value_temp->send_data_buffer[3] = 0x00;
    motor_value_temp->send_data_buffer[4] = 0x00;
    motor_value_temp->send_data_buffer[5] = 0x00;
    motor_value_temp->send_data_buffer[6] = 0xA7;

    uart_write_buffer(uart_index, motor_value_temp->send_data_buffer, 7);
}

static void small_driver_init_value(small_device_value_struct *motor_value_temp)
{
    memset(motor_value_temp->send_data_buffer, 0, 7);
    memset(motor_value_temp->receive_data_buffer, 0, 7);

    motor_value_temp->receive_data_count = 0;
    motor_value_temp->sum_check_data = 0;

    motor_value_temp->receive_left_speed_data = 0;
    motor_value_temp->receive_right_speed_data = 0;
}

void small_driver_uart_init(small_device_enum device_num)
{
    uart_index_enum uart_index;
    uint32 baudrate;
    uart_pin_enum tx_pin;
    uart_pin_enum rx_pin;

    if(device_num == SMALL_DRIVER_1)
    {
        uart_index = SMALL_DRIVER_UART_1;
        baudrate = SMALL_DRIVER_BAUDRATE_1;
        tx_pin = SMALL_DRIVER_TX_1;
        rx_pin = SMALL_DRIVER_RX_1;

        small_driver_init_value(&motor_value_1);

        // 挂接到逐飞库的 UART DMA 接收回调（每字节进入）
        if(uart_index == UART_1) uart1_irq_handler = small_driver_uart1_irq;
        else if(uart_index == UART_2) uart2_irq_handler = small_driver_uart1_irq;
        else if(uart_index == UART_3) uart3_irq_handler = small_driver_uart1_irq;
        else if(uart_index == UART_4) uart4_irq_handler = small_driver_uart1_irq;
    }
    else
    {
        uart_index = SMALL_DRIVER_UART_2;
        baudrate = SMALL_DRIVER_BAUDRATE_2;
        tx_pin = SMALL_DRIVER_TX_2;
        rx_pin = SMALL_DRIVER_RX_2;

        small_driver_init_value(&motor_value_2);

        if(uart_index == UART_1) uart1_irq_handler = small_driver_uart2_irq;
        else if(uart_index == UART_2) uart2_irq_handler = small_driver_uart2_irq;
        else if(uart_index == UART_3) uart3_irq_handler = small_driver_uart2_irq;
        else if(uart_index == UART_4) uart4_irq_handler = small_driver_uart2_irq;
    }

    uart_init(uart_index, baudrate, tx_pin, rx_pin);

    // 确保 DMA RX 已经启动，且开接收中断
    uart_dma_init(uart_index);
    uart_rx_start_buff(uart_index);
    uart_rx_interrupt(uart_index, 1);

    // 启动后先发 0 占空比，再请求速度（驱动默认 10ms 周期回传）
    small_driver_set_duty(device_num, 0, 0);
    small_driver_get_speed(device_num);
}
