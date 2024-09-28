# imxrt-flexspi-mem-fw

## 1. 工程源文件组织

```text
// 程序配置选项
\boards\mimxrt\mtu_fw\xxxDevice\port_cfg_mtu.h

// 程序主逻辑，MCU 端从串口接收上位机命令包，处理命令，打印命令处理结果
\boards\mimxrt\mtu_fw\src\mtu.c

// 不同命令包数据格式定义
    命令1. pin_unittest_packet_t
    命令2. config_system_packet_t
    命令4. rw_test_packet_t
\boards\mimxrt\mtu_fw\src\mtu.h

// 软件实现的 CRC16 校验命令包完整性
\boards\mimxrt\mtu_fw\src\mtu_crc16.c/h

// 用于命令包接收实现的串口驱动
   1. 中断方式接收上位机数据存进 512 字节 RingBuf
   2. 查询方式发送数据给上位机（print打印或者hex数据发送）
\boards\mimxrt\mtu_fw\src\mtu_lpuart.c
\boards\mimxrt\mtu_fw\src\mtu_usart.c
\boards\mimxrt\mtu_fw\src\mtu_uart.h

// 命令4之 R/W Test 实现
\boards\mimxrt\mtu_fw\src\mtu_flexspi_nor_test.c/h
\boards\mimxrt\mtu_fw\src\mtu_flexspi_nor_ops.c

// 命令1之 Pin Unittest 实现
   1. 根据命令包数据配置指定 GPIO，方波翻转测试连通性，可 ADC 采集回来画波形
   2. 这里涉及到 MCU 端给上位机发送数据模式切换（一般信息打印/ADC采集值）
\boards\mimxrt\mtu_fw\src\mtu_pin.h
\boards\mimxrt\mtu_fw\src\mtu_timer.c/h
\boards\mimxrt\mtu_fw\xxxDevice\port_adc_conv.c
\boards\mimxrt\mtu_fw\xxxDevice\port_flexspi_pin.c
```

### 附录1、板卡测试
> * i.MXRT三位数系列 FlexSPI 编号从 0 开始
> * i.MXRT四位数系列 FlexSPI 编号从 1 开始

<table><tbody>
    <tr>
        <th>RT</th>
        <th>Memory</th>
        <th>Port</th>
        <th>Project Build</th>
        <th>Status</th>
    </tr>
    <tr>
        <td rowspan="2">i.MXRT500</td>
        <td>NXP EVK_Rev.C1<br>
            200MHz CMOS Octal SPI Flash<br>
            U38 - MX25UM51345GXDI00</td>
        <td>PIO1[28:18]<br>
            FLEXSPI0_PortA</td>
        <td>/</td>
        <td>Done</td>
    </tr>
    <tr>
        <td>NXP EVK_Rev.C1<br>
            200MHz DDR OPI Xccela PSRAM<br>
            U108 - APS6408L-OBM-BA</td>
        <td>PIO5[18:15], PIO4[18:11]<br>
            FLEXSPI1_PortA</td>
        <td>/</td>
        <td>Done</td>
    </tr>
    <tr>
        <td rowspan="2">i.MXRT600</td>
        <td>NXP EVK_Rev.E<br>
            200MHz CMOS Octal SPI Flash<br>
            U19 - MX25UM51345GXDI00</td>
        <td>PIO1[14:11], PIO2[23:22,19:17]<br>
            FLEXSPI0_PortB</td>
        <td>/</td>
        <td>Done</td>
    </tr>
    <tr>
        <td>NXP EVK_Rev.E<br>
            200MHz DDR OPI Xccela PSRAM<br>
            U108 - APS6408L-OBM-BA</td>
        <td>PIO1[28:18]<br>
            FLEXSPI0_PortA</td>
        <td>/</td>
        <td>Done</td>
    </tr>
    <tr>
        <td rowspan="1">i.MXRT1050</td>
        <td>NXP EVKB_Rev.A1<br>
            166MHz Hyper Flash<br>
            U19- S26KS512SDPBHI020</td>
        <td>GPIO_SD_B1[11:00]<br>
            FLEXSPI1_PortA&B</td>
        <td>/</td>
        <td>Done</td>
    </tr>
    <tr>
        <td rowspan="1">i.MXRT1170</td>
        <td>NXP EVK_Rev.A-B<br>
            133MHz Quad SPI Flash<br>
            U23 - IS25WP128</td>
        <td>GPIO_SD_B2[11:06]<br>
            FLEXSPI1_PortA</td>
        <td>/</td>
        <td>Done</td>
    </tr>
</table>