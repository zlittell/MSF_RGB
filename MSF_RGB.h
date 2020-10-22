#ifndef MSF_RGB_H_
#define MSF_RGB_H_

void LED_Init(uint8_t);
uint8_t LED_SingleUpdate(uint8_t, uint8_t, uint8_t);
void LED_UpdateAll(uint8_t, uint8_t*, uint8_t);
void LED_UpdateBrightness(uint8_t, uint8_t);
void LED_Enable(uint8_t);
void LED_Disable(uint8_t);

#endif /* MSF_RGB_H_ */