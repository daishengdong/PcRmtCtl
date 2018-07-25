/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Descriptors for Joystick Mouse Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "../include/usb_lib.h"
#include "../include/usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const u8 Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize40*/
    0x83,                       /*idVendor (0x0483)*/
    0x04,
    0x10,                       /*idProduct = 0x5710*/
    0x57,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                                  manufacturer */
    2,                          /*Index of string descriptor describing
                                                 product*/
    3,                          /*Index of string descriptor describing the
                                                 device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* Joystick_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const u8 Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
  {
    0x09, /* bLength: Configuation Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    JOYSTICK_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /*bNumInterfaces: 1 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
                                     the configuration*/
    0xE0,         /*bmAttributes: bus powered */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x00,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x02,//0x01,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x00,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    //JOYSTICK_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    //0x00,
    sizeof(Joystick_ReportDescriptor)&0xff,
		(sizeof(Joystick_ReportDescriptor)>>8)&0xff,
		
		
		/******************** Descriptor of  endpoint ********************/
    /* 27 */	
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x10,//0x04,          /*wMaxPacketSize: 4 Byte max */
    0x00,
    0x20,          /*bInterval: Polling Interval (32 ms)*/
    /* 34 */
		
		
		0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x01,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x10,//0x04,          /*wMaxPacketSize: 4 Byte max */
    0x00,
    0x20,          /*bInterval: Polling Interval (32 ms)*/
		/* 41 */
  }
  ; /* MOUSE_ConfigDescriptor */
const u8 Joystick_ReportDescriptor[] =
  {
    0x05,0x01,          /*Usage Page(Generic Desktop)*/  
    0x09,0x06,          /*Usage(Keyboard)*/
    0xA1,0x01,          /*Collection(Application)*/  
		0x85,0x01,
			
    0x05,0x07,          /*Usage_page(Keyboard)*/   
    /* 8 */
    0x19,0xe0,          /*Usage Minimum(1)*/
    0x29,0xe7,          /*Usage Maximum(3)*/    
    0x15,0x00,          /*Logical Minimum(0)*/
    0x25,0x01,          /*Logical Maximum(1)*/
    /* 16 */
    0x95,0x08,          /*Report Count(8)*/
    0x75,0x01,          /*Report Size(1)*/
    0x81,0x02,          /*Input(Variable)*/ 
    0x95,0x01,          /*Report Count(1)*/
    
    /* 24 */
    0x75,0x08,          /*Report Size(5)*/
    0x81,0x03,          /*Input(Constant,Array)*/
    0x95,0x06,          /*Report Count(6)*/
    0x75,0x08,          /*Report Size(8)*/
    
		//
    /* 32 */
    0x15,0x00,          /*Logical Minimum(0)*/
    0x25,0xFF,          /*Logical Minimum(255))*/
    0x05,0x07,          /*Usage_page(Keyboard)*/
    0x19,0x00,          /*Usage Minimum(0)*/
    /* 40 */
    0x29,0x65,          /*Usage Maximum(0x65)*/   
    0x81,0x00,          /*Input(Constant,Array)*/
		
		//下面为输出描述
    0x25,0x01,          /*Logical Minimum(1)*/
    0x95,0x05,          /*Report Count(5)*/
    
    /* 48 */
    0x75,0x01,          /*Report Size(1)*/ 
    0x05,0x08,          /*Usage_page(Keyboard)*/
    0x19,0x01,					/*Usage Minimum(0)*/
    0x29,0x05,					/*Usage Maximum(0x5)*/ 
    
    /* 56 */
    0x91,0x02,					/*OUTPUT()*/
    0x95,0x01,					/*Report Count(6)*/
    0x75,0x03,					/*Report Size(1)*/
    0x91,0x03,					/*OUTPUT()*/
		/*64*/
    0xc0,
		
		/*************鼠标部分描述符**************/
		0x05,0x01,          /*Usage Page(Generic Desktop)*/  
    0x09,0x02,          /*Usage(Mouse)*/    
    0xA1,0x01,          /*Collection(Logical)*/  
		0x85,0x02,
    0x09,0x01,          /*Usage(Pointer)*/    
    /* 8 */
    0xA1,0x00,          /*Collection(Linked)*/    
    0x05,0x09,          /*Usage Page(Buttons)*/    
    0x19,0x01,          /*Usage Minimum(1)*/    
    0x29,0x03,          /*Usage Maximum(3)*/    
    /* 16 */
    0x15,0x00,          /*Logical Minimum(0)*/    
    0x25,0x01,          /*Logical Maximum(1)*/   
    0x95,0x03,          /*Report Count(3)*/   
    0x75,0x01,          /*Report Size(1)*/
    /* 24 */
    0x81,0x02,          /*Input(Variable)*/    
    0x95,0x01,          /*Report Count(3)*/   
    0x75,0x05,          /*Report Size(5)*/   
    0x81,0x03,          /*Input(Constant,Array)*/
    
    /* 32 */
    0x05,0x01,          /*Usage Page(Generic Desktop)*/   
    0x09,0x30,          /*Usage(X axis)*/    
    0x09,0x31,          /*Usage(Y axis)*/    
    0x09,0x38,          /*Usage(Wheel)*/
    
    /* 40 */
    0x15,0x81,          /*Logical Minimum(-127)*/
    0x25,0x7F,          /*Logical Maximum(127)*/   
    0x75,0x08,          /*Report Size(8)*/  
    0x95,0x03,          /*Report Count(3)*/    
    /* 48 */
    0x81,0x06,          /*Input(Variable, Relative)*/
    
    0xC0,0xc0          /*End Collection*/	
  }
  ; /* Joystick_ReportDescriptor */

/* USB String Descriptors (optional) */
const u8 Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] =
  {
    JOYSTICK_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const u8 Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] =
  {
    JOYSTICK_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const u8 Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] =
  {
    JOYSTICK_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'U', 0,
    'S', 0, 'B', 0, 's', 0, 't', 0, 'i', 0, 'c', 0, 'k', 0
  };
u8 Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] =
  {
    JOYSTICK_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
  };

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

