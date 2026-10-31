#include <Arduino.h>

#ifdef LCD_ENABLE
    #include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_1_2nd_HW_I2C u8g2(U8G2_R0);

TaskHandle_t Lcd_TaskHandle;
static void Lcd_Task(void *parameter);
#endif

void Lcd_Init(void){
#ifdef LCD_ENABLE
	xTaskCreatePinnedToCore(
		Lcd_Task, /* Function to implement the task */
		"Lcd_Task", /* Name of the task */
		3072, /* Stack size in words */ // 20251015: increased to 3072 because saving of "allgemeine Einstellungen" let to restarts because of stack overflows
		NULL, /* Task input parameter */
		1, /* Priority of the task */
		&Led_TaskHandle, /* Task handle. */
		1 /* Core where the task should run */
	);
#endif
}

void Lcd_Exit(void){
#ifdef LCD_ENABLE

#endif
}

#ifdef LCD_ENABLE
// A static layout engine helper that splits text across two lines nicely by spaces
void drawSplitTrackTitle(const char* text, int x, int yStart, int maxLineWidth) {
  String line1 = "";
  String line2 = "";
  String currentWord = "";
  
  u8g2.setFont(u8g2_font_6x10_tf);
  
  for (int i = 0; text[i] != '\0'; i++) {
    char c = text[i];
    if (c == ' ') {
      // Check if adding the word fits on Line 1
      if (u8g2.getUTF8Width((line1 + currentWord).c_str()) < maxLineWidth && line2 == "") {
        line1 += currentWord + " ";
      } else {
        line2 += currentWord + " ";
      }
      currentWord = "";
    } else {
      currentWord += c;
    }
  }
  
  // Clean up any remaining trailing buffer words
  if (currentWord.length() > 0) {
    if (u8g2.getUTF8Width((line1 + currentWord).c_str()) < maxLineWidth && line2 == "") {
      line1 += currentWord;
    } else {
      line2 += currentWord;
    }
  }

  // Draw line 1
  u8g2.drawUTF8(x, yStart, line1.c_str());
  
  // Draw line 2 (offset downwards by standard 12px row gap line-height metrics)
  if (line2.length() > 0) {
    u8g2.drawUTF8(x, yStart + 12, line2.c_str());
  }
}
void drawNowPlayingScreen() {
    // 1. Static Layout Title Bar
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(0, 8, "NOW PLAYING");
    u8g2.drawHLine(0, 11, 128);

    // 2. Intelligent Two-Line Wrapping Split Block
    // Arguments: (Text buffer, X coordinate alignment, Y baseline start, Max row pixel width limits)
    drawSplitTrackTitle(currentTrackName, 4, 25, 120);

    // 3. Completely Static Bottom Control Bar Frame
    u8g2.drawHLine(0, 48, 128);
    
    u8g2.drawFrame(4, 54, 80, 6); // Track Progress bar frame configuration
    int barWidth = (76 * trackProgress) / 100;
    u8g2.drawBox(6, 56, barWidth, 2); 
    
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(90, 60, isPlaying ? "▶ PLAY" : "⏸ PAUS");
}

void Led_Task(void *parameter){
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x10_tf);

    for (;;){

            vTaskDelay(portTICK_PERIOD_MS * taskDelay);
    }
	delete[] leds;
	delete indicator;
	vTaskDelete(NULL);
}
#endif