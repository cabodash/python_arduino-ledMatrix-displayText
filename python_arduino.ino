#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#if USE_UI_CONTROL
#include <MD_UISwitch.h>
#endif

#define DEBUG 0

#if DEBUG
#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTX(x) Serial.println(x, HEX)
#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTX(x)
#endif

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

MD_Parola parola = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

#if USE_UI_CONTROL
const uint8_t SPEED_IN = A5;

const uint8_t DIRECTION_SET = 8;
const uint8_t INVERT_SET = 9;

const uint8_t SPEED_DEADBAND = 5;
#endif

uint8_t scrollSpeed = 30; // Velocidad de desplazamiento no será necesaria para el efecto fade
textEffect_t scrollEffect = PA_FADE; // Cambiado a efecto fade
textPosition_t scrollAlign = PA_CENTER; // Alineación al centro para el efecto fade
uint16_t scrollPause = 2000; // Pausa entre mensajes

#define BUF_SIZE 75
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "Hola, soy cabodash   " };
bool newMessageAvailable = true;

#if USE_UI_CONTROL

MD_UISwitch_Digital uiDirection(DIRECTION_SET);
MD_UISwitch_Digital uiInvert(INVERT_SET);

void doUI(void)
{
  // La velocidad ya no es relevante para el efecto fade, por lo que se elimina el ajuste de velocidad
  if (uiDirection.read() == MD_UISwitch::KEY_PRESS)
  {
    PRINTS("\nCambio en la dirección no aplica para efecto fade");
    // No se cambia el efecto de desplazamiento ya que estamos usando fade
  }

  if (uiInvert.read() == MD_UISwitch::KEY_PRESS)
  {
    PRINTS("\nCambiando modo invertido");
    parola.setInvert(!parola.getInvert());
  }
}
#endif

void readSerial(void)
{
  static char *cp = newMessage;

  while (Serial.available())
  {
    *cp = (char)Serial.read();
    if ((*cp == '\n') || (cp - newMessage >= BUF_SIZE-2))
    {
      *cp = '\0';
      cp = newMessage;
      newMessageAvailable = true;
    }
    else
      cp++;
  }
}

void setup() {
  Serial.begin(57600);
  Serial.print("\n[Parola Scrolling Display]\nEscribe un mensaje para el display\nTermina el mensaje con una nueva línea");

#if USE_UI_CONTROL
  uiDirection.begin();
  uiInvert.begin();
  pinMode(SPEED_IN, INPUT);

  doUI();
#endif

  parola.begin();
  parola.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
}

void loop() {
#if USE_UI_CONTROL
  doUI();
#endif

  if (parola.displayAnimate())
  {
    if (newMessageAvailable)
    {
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    parola.displayReset();
  }
  readSerial();
}
