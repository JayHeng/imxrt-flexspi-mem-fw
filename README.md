# imxrt-flexspi-mem-unittest
Collect NOR Flash/HyperRAM/PSRAM unit test projects for i.MXRT FlexSPI | 收集i.MXRT全平台FlexSPI NOR Flash/HyperRAM/PSRAM读写单元测试例程 

> * IAR EWARM v9.10.2 
> * Keil MDK v5.31  

备注：

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

