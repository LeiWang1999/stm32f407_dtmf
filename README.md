# 简易示波器 & 信号发生器 for 正点原子探索者 STM32F407 开发板

## 最初版本

STM32F4 简易波形图显示

————小马哥————2018.3.14 --2018.3.16

内容：在显示屏上通过按键产生正弦波、三角波、矩形波、锯齿波，并且能通过按键来暂停波形，显示静态波形，并且显示波形电压的峰峰值。

一、波形图显示

1. 生成正弦波、三角波、矩形波、锯齿波数据

2. 将这些数据显示到 TFT 2.4 寸液晶屏上

思路：用数组 1,2,3,4 分别保存正弦波、三角波、矩形波、锯齿波数据，写按键函数分别打印不同的数组内容，然后再将数组内容打印到 TFT 液晶屏上。

二、峰峰值显示

从数组中采集最大和最小值，然后做差求得峰峰值，显示到 TFT 液晶屏上。

优化：使用 DMA 来减轻 CPU 的负担，提高数据传输速度。

*STM32 使用 DMA+DAC+TIMER 输出正弦波*

那么对于使用 DMA+DAC+TIMER 产生正弦波的原理或过程，我有这样一个简单的理解：先将一个可以生成正弦波的数据表保存在静态内存中，然后在 DAC 以及这块内存中间使用 DMA 建立一个通道，经过以上步骤之后，DAC 模块就可以通过 DMA 通道拿取静态内存中可以生成正弦波的数据，拿取数据，然后经过数模准换，在引脚进行输出就可以得到正弦波了。那么当然，这个速度是非常快的，如果没有一定的延时，那么得到的估计就是一个变化很快的模拟量。所以这个时候就需要使用定时器 TIMER 了。DAC 在初始化的时候，可以设置成使用定时器触发，这就意味着，当定时器溢满的时候，就会触发 DAC 工作。这样一来，就可以通过改变定时器的定时时间来改变正弦波的周期了。电压大小的显示用 DAC 来处理。（PA4 和 PA5 短接，DAC 与 ADC 结合。）

## 修改

在网友代码的基础上增加了一个功能：按下黄色按键（KEY_UP,KEY_0,KEY_1,KEY_2），动态正弦波会暂停，按下红色按键（RESERT）动态波形重新显示。

操作：只需要在 HARDWARE 文件下添加 key.c 和 key.h 文件，再做修改即可。

## 再修改

把 DAC 头文件和源文件删除了，居然还有波形

为什么 DAC GPIO 配置 PIN4 或 PIN5：

PA4：DAC 把正弦数字数据转化为模拟数据，把模拟数据给 PA5

PA5：模拟量转化为数字量，生成波形和 Vpp 大小

## 第三次修改

基本完成四种波形的 run 和 stop，~~但无法完成在暂停键按下去之后，按其他的按键可以直接跳转到那个按键对应的波形~~。