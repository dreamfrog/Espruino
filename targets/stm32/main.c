#include "platform_config.h"
#ifdef USB
#if defined(STM32F1) || defined(STM32F3)
 #include "usb_utils.h"
 #include "usb_lib.h"
 #include "usb_desc.h"
 #include "usb_pwr.h"
#endif
#ifdef STM32F4
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#endif
#endif
#include "jsinteractive.h"
#include "jshardware.h"
#include "jshardware_pininfo.h"


#define BSRR_VAL 0xFFFF

int main(void){
#ifdef STM32F103RB_MAPLE
  // get in quick and relocate vector table!
  SCB->VTOR = 0x08005000;
#endif
  jshInit();
#ifdef USB
#if defined(STM32F1) || defined(STM32F3)
  USB_Init_Hardware();
  USB_Init();
#endif
#ifdef STM32F4
  USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS
            USB_OTG_HS_CORE_ID,
#else
            USB_OTG_FS_CORE_ID,
#endif
            &USR_desc,
            &USBD_CDC_cb,
            &USR_cb);
#endif
#endif

  volatile int w,h;
//#ifndef ECU
#ifdef USB
  for (w=0;w<1000000;w++)
 #ifdef STM32F4 // IT's FAST!
    for (h=0;h<10;h++); // wait for things to settle (for USB)
 #else
    for (h=0;h<2;h++); // wait for things to settle (for USB)
 #endif
#else
  for (w=0;w<100000;w++)
    for (h=0;h<2;h++); // wait for things to settle (for Serial comms)
#endif
//#endif

  bool buttonState = false;
  buttonState = jshPinInput(BTN1_PININDEX) == BTN1_ONSTATE;
  jsiInit(!buttonState); // pressing USER button skips autoload


  int counter = 0;
  while (1) {
    jsiLoop();

/*#ifdef LED1_PORT
    counter++;
    GPIO_WriteBit(LED1_PORT,LED1_PIN, (counter>>13) & 1);
#endif*/
  }
  //jsiKill();
  //jshKill();
}

