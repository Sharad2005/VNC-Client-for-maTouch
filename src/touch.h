#include <Wire.h>
#include "TAMC_GT911.h"

#define I2C_SDA_PIN 17
#define I2C_SCL_PIN 18
#define TOUCH_INT -1
#define TOUCH_RST 38

int16_t touch_last_x = 0, touch_last_y = 0;

// Touch state machine
typedef enum {
  TOUCH_STATE_IDLE,       // No touch detected
  TOUCH_STATE_DETECTED,   // Touch detected but not yet processed
  TOUCH_STATE_PROCESSED,  // Touch has been processed
  TOUCH_STATE_RELEASED    // Touch has been released
} touch_state_t;

touch_state_t touch_state = TOUCH_STATE_IDLE;

// Debounce time
unsigned long last_touch_time = 0;
const unsigned long TOUCH_DEBOUNCE_MS = 150; // 150ms debounce time

TAMC_GT911 ts = TAMC_GT911(I2C_SDA_PIN, I2C_SCL_PIN, TOUCH_INT, TOUCH_RST, 1024, 600);

void touch_init(int max_x, int max_y)
{
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  ts.begin();

#ifdef SCREEN_HD
  ts.setRotation(ROTATION_INVERTED);
#endif

#ifdef SCREEN_NORMAL
  ts.setRotation(ROTATION_NORMAL);
#endif
}

bool touch_has_signal()
{
  return true;
}

// Process touch state machine
void touch_update()
{
  ts.read();
  unsigned long current_time = millis();
  
  // State machine for touch processing
  switch(touch_state) {
    case TOUCH_STATE_IDLE:
      if (ts.isTouched && (current_time - last_touch_time > TOUCH_DEBOUNCE_MS)) {
        // New touch detected
        touch_state = TOUCH_STATE_DETECTED;
        
        // Update coordinates
#ifdef SCREEN_HD
        touch_last_x = map(ts.points[0].x, 0, 1024, 0, SCREEN_W);
        touch_last_y = map(ts.points[0].y, 0, 750, 0, SCREEN_H);
#endif

#ifdef SCREEN_NORMAL
        touch_last_x = map(ts.points[0].x, 1024, 200, 0, SCREEN_W);
        touch_last_y = map(ts.points[0].y, 600, 120, 0, SCREEN_H);
#endif
        last_touch_time = current_time;
      }
      break;
    
    case TOUCH_STATE_DETECTED:
      // Keep in this state until processed externally
      break;
    
    case TOUCH_STATE_PROCESSED:
      if (!ts.isTouched) {
        // Touch released after processing
        touch_state = TOUCH_STATE_RELEASED;
        last_touch_time = current_time;
      }
      break;
    
    case TOUCH_STATE_RELEASED:
      if (current_time - last_touch_time > TOUCH_DEBOUNCE_MS) {
        // Return to idle state after debounce
        touch_state = TOUCH_STATE_IDLE;
      }
      break;
  }
}

bool touch_touched()
{
  // Return true only when a new touch is detected and not yet processed
  return (touch_state == TOUCH_STATE_DETECTED);
}

void touch_mark_processed()
{
  // Mark the current touch as processed
  if (touch_state == TOUCH_STATE_DETECTED) {
    touch_state = TOUCH_STATE_PROCESSED;
  }
}

bool touch_released()
{
  // Return true only when a touch has just been released
  return (touch_state == TOUCH_STATE_RELEASED);
}
