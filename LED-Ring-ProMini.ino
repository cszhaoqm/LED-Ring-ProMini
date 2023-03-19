#include <FastLED.h>

#define LED_NUMS            24
#define LED_STAT            30
#define COLR_PIN             2
#define BRIG_PIN             3
#define MODE_PIN             4
#define DATA_PIN             5

#define LED_COLR_WHT         0
#define LED_COLR_RED         1
#define LED_COLR_GRN         2
#define LED_COLR_BLU         3
#define LED_COLR_RED_BLU     4
#define LED_COLR_END         5

#define LED_MODE_ON          0
#define LED_MODE_FLASH       1
#define LED_MODE_SOS         2
#define LED_MODE_OFF         3
#define LED_MODE_END         4

byte    glb_brightness   =  20;
byte    glb_clor_status  =   0;
byte    glb_mode_status  =   0;
byte    glb_time_index   =   0;
int     glb_loop_delay   = 500;

// Define the array of leds
CRGB leds[LED_NUMS];
byte led_status[LED_STAT];

void setup()
{
  Serial.begin(115200);
  pinMode(BRIG_PIN, INPUT);
  pinMode(COLR_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);
  digitalWrite(BRIG_PIN, HIGH);
  digitalWrite(COLR_PIN, HIGH);
  digitalWrite(MODE_PIN, HIGH);

  for (int i = 0; i < LED_STAT; i++)
    led_status[i] = 1;

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, LED_NUMS);
}

void loop()
{
  int i = 0;
  if (digitalRead(BRIG_PIN) == LOW)
  {
    delay(150);
    if (digitalRead(BRIG_PIN) == LOW)
    {
      glb_brightness = glb_brightness + 5;
      if (glb_brightness >= 255)
      {
        glb_brightness = 5;
      }
    }
  }

  if (digitalRead(COLR_PIN) == LOW)
  {
    delay(150);
    if (digitalRead(COLR_PIN) == LOW)
    {
      glb_clor_status++;
    }
  }

  if (digitalRead(MODE_PIN) == LOW)
  {
    delay(150);
    if (digitalRead(MODE_PIN) == LOW)
    {
      glb_mode_status++;
      if(glb_mode_status == LED_MODE_END)
        glb_mode_status = LED_MODE_ON;

      glb_time_index =   0;  
      switch (glb_mode_status)
      {
        case LED_MODE_ON:
          for (i = 0; i < LED_STAT; i++)
          {
            led_status[i] = 1;
          }
          break;
        case LED_MODE_FLASH:
          for (i = 0; i < LED_STAT/3; i++)
          {
            led_status[3 * i]   = 1;
            led_status[3 * i + 1] = 0;
            led_status[3 * i + 2] = 0;
          }
          break;
        case LED_MODE_SOS:
          i = 0;
          while (i < 6)
          {
            led_status[i++] = 1;
            led_status[i++] = 0;
          }
          while (i < 18)
          {
            led_status[i++] = 1;
            led_status[i++] = 1;
            led_status[i++] = 1;
            led_status[i++] = 0;
          }
          while (i < 24)
          {
            led_status[i++] = 1;
            led_status[i++] = 0;
          }
          while (i < LED_STAT)
          {
            led_status[i++] = 0;
          }
          break;
        case LED_MODE_OFF:
          for (i = 0; i < LED_STAT; i++)
          {
            led_status[i] = 0;
          }
          break;
        case LED_MODE_END:
          glb_mode_status = LED_MODE_ON;
          break;
        default:
          break;
      }
    }
  }

  Serial.print("Brightness, Color, Mode: ");
  Serial.print(glb_brightness);      
  Serial.print(", ");
  Serial.print(glb_clor_status);       
  Serial.print(", ");
  Serial.println(glb_mode_status);    

  CRGB led_color = CRGB::White;
  switch (glb_clor_status)
  {
    case LED_COLR_WHT:
      led_color = CRGB::White;
      break;
    case LED_COLR_RED:
      led_color = CRGB::Red;
      break;
    case LED_COLR_GRN:
      led_color = CRGB::Green;
      break;
    case LED_COLR_BLU:
      led_color = CRGB::Blue;
      break;
    case LED_COLR_END:
      glb_clor_status = LED_COLR_WHT;
      break;
    default:
      break;
  }

  if (glb_clor_status == LED_COLR_RED_BLU)
  {
      glb_loop_delay = 150;
      for (i = 0; i < LED_NUMS / 2; i++)
      {
      leds[i] = CRGB::Red;
      }
      for (; i < LED_NUMS; i++)
      {
      leds[i] = CRGB::Blue;
      }
  }
  else
  {
      glb_loop_delay = 500;
      for (i = 0; i < LED_NUMS; i++)
      {
      leds[i] = led_color;
      }
  }

  if (led_status[glb_time_index] == 0)
  {
      Serial.println("Turn all OFF");
      for (i = 0; i < LED_NUMS; i++)
      {
      leds[i] = CRGB::Black;
      }
  }
  else
      Serial.println("Turn all ON");
  
  FastLED.setBrightness(glb_brightness);
  FastLED.show();
  delay(glb_loop_delay);

  glb_time_index++;
  if(glb_time_index >= LED_STAT)
      glb_time_index = 0;
}
