# PcRmtCtl
基于STM32的PC遥控器（红外+USB HID模拟键盘鼠标）

## 硬件采购
- [stm32f103 购买链接](https://item.taobao.com/item.htm?spm=a1z10.3-c-s.w4002-14787471870.16.3ec36865sI2jYe&id=559521739942)
- [红外遥控器+接收头 购买链接](https://item.taobao.com/item.htm?spm=a1z10.3-c-s.w4002-16569462289.9.534a6a4bxEFUju&id=548823910606)  
*硬件成本约人民币12元左右*

## 硬件连线
- ISP下载TTL线连接方式  
![ISP下载TTL线连接方式](https://img.alicdn.com/imgextra/i4/44390641/TB2osHTlS3PL1JjSZFxXXcBBVXa_!!44390641.jpg)
- 程序烧录跳帽  
![程序烧录跳帽](https://img.alicdn.com/imgextra/i3/361121710/TB29p17aSiJ.eBjSspoXXcpMFXa_!!361121710.jpg)
- 接收头针脚  
![接收头针脚](https://img.alicdn.com/imgextra/i3/2658592015/TB2T9lvnjihSKJjy0FiXXcuiFXa_!!2658592015.jpg)
- 板子布局图  
![板子布局图](https://img.alicdn.com/imgextra/i3/44390641/TB2c4K4anSPY1JjSZPcXXXIwpXa_!!44390641.png)

>红外接收头与f103的连接，代码中是设定stm32f103的PB9针脚作为红外信号的输入针脚(remote.c)，可自行更改。因此这里需将红外接收头的输出引脚连接到板子PB9脚上，物理上是上图中右下角倒数第三根针(可以参考上图或在淘宝链接下自行下载板子原理图查看)

## USB模拟键鼠数据表
- [USB键码表](http://www.win.tue.nl/~aeb/linux/kbd/scancodes-10.html#ss10.6)
- [USB鼠标、键盘数据格式](https://blog.csdn.net/qq_35661436/article/details/53729229)

- 遥控器  
![遥控器](https://img.alicdn.com/imgextra/i4/2658592015/TB20DruohlmpuFjSZPfXXc9iXXa_!!2658592015.jpg)

>程序原理：板子接收到遥控器上按“上下左右、OK”五个键时，分别发送键盘消息“上下左右 空格”，可以实现音量上调、下调，视频的快退、快进、暂停/播放。具体的逻辑参考main.c，可自行更改。

**这里本来想实现遥控器按2、8、4、6、5分别控制鼠标上、下、左、右移动及双击动作的，不过[USB鼠标、键盘数据格式](https://blog.csdn.net/qq_35661436/article/details/53729229)中的描述让我非常迷惑，举例来说，鼠标消息的首字节，按文中描述第三bit必然为1，那么首字节必然>=8，实际上代码中要求该字节必须为1；同理，键盘消息的首字节按照不同情况的组合应该是256范围内的一个“随机”值，实际上从代码中看该字节却必须为2，从文中测试抓取的键盘消息数据来看，首字节也是2。因此实际上我对具体组装键盘、鼠标消息字节的算法其实非常迷惑，如果有知情的，非常欢迎随时指点。**
