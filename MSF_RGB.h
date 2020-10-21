#ifndef MSF_RGB_H_
#define MSF_RGB_H_

void LED_init(uint8_t);
void LED_updateRed(uint8_t, uint8_t);
void LED_updateGreen(uint8_t, uint8_t);
void LED_updateBlue(uint8_t, uint8_t);
void LED_updateRGB(uint8_t, uint8_t, uint8_t, uint8_t);
void LED_updateBrightness(uint8_t, uint8_t);

#endif /* MSF_RGB_H_ */