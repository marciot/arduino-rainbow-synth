/****************************************************************************
 *   RainbowPiano by (c) 2018 Marcio Teixeira                               *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "src/ui_toolbox.h"

#include "FastLED.h"
#define NUM_LEDS       16
#define LED_PIN        5
#define LED_BRIGHTNESS 64
#define LCD_BRIGHTNESS 255

// Margin around the buttons
#define MARGIN_T  10
#define MARGIN_B  10

constexpr int16_t  font_small    = 28;
constexpr int16_t  font_medium   = 29;
constexpr int16_t  font_large    = 31;

/****************************** SCREEN DECLARATIONS *****************************/

class PianoScreen : public InterfaceScreen {
  private:
    static effect_t instrument;
    static uint8_t  volume;
    static uint8_t  highlighted_note;
    static uint8_t  highlighted_instrument;
    static CRGB     leds[NUM_LEDS];

    static void buttonStyleCallback(uint8_t tag, uint8_t &style, uint16_t &options, bool post);
    static uint32_t getNoteColor(uint8_t tag);
  public:
    static void onEntry();
    static void onExit();
    static void onRedraw(draw_mode_t what);
    static void onTouchStart(uint8_t tag);
    static void onIdle();
};

class SongsScreen : public InterfaceScreen {
  public:
    static void onRedraw(draw_mode_t what);
    static void onTouchEnd(uint8_t tag);
};

SCREEN_TABLE {
  DECL_SCREEN(PianoScreen),
  DECL_SCREEN(SongsScreen)
};
SCREEN_TABLE_POST

/***************************** PIANO SCREEN *****************************/

static effect_t PianoScreen::instrument;
static uint8_t  PianoScreen::volume;
static uint8_t  PianoScreen::highlighted_instrument;
static uint8_t  PianoScreen::highlighted_note;
static CRGB     PianoScreen::leds[NUM_LEDS];

constexpr uint16_t dial_min = 4095;
constexpr uint16_t dial_max = 0xFFFF - dial_min;

enum {
  black  = 0x000000,
  red    = 0xFF0000,
  orange = 0xFF7F00,
  yellow = 0xFFFF00,
  green  = 0x00FF00,
  blue   = 0x0000FF,
  indigo = 0x4B0082,
  violet = 0x9400D3,
  white  = 0xFFFFFF
};

void PianoScreen::onEntry() {
  instrument = PIANO;
  volume     = 255;
  highlighted_instrument = 241;
  
  InterfaceScreen::onEntry();
  sound.set_volume(volume);
  UIData::enable_touch_sounds(false);

  CLCD::turn_on_backlight();
  CLCD::set_brightness(LCD_BRIGHTNESS);

  CommandProcessor cmd;
  cmd.set_button_style_callback(buttonStyleCallback);

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( LED_BRIGHTNESS );
}

void PianoScreen::onExit() {
  InterfaceScreen::onExit();
  CommandProcessor cmd;
  cmd.set_button_style_callback(NULL);
}

void PianoScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(0x222222))
     .cmd(CLEAR(true,true,true));

  #define MARGIN_L  3
  #define MARGIN_R  3
  #define MARGIN_T  3
  #define MARGIN_B  3
  
  #define GRID_ROWS 8
  #define GRID_COLS 12
  cmd.font(font_small)
     .fgcolor(0x111111)
     .tag(241).button( BTN_POS(1,1), BTN_SIZE(2,1), F("Piano"))
     .tag(242).button( BTN_POS(1,2), BTN_SIZE(2,1), F("Organ"))
     .tag(243).button( BTN_POS(1,3), BTN_SIZE(2,1), F("Harp"))

     .tag(247).button( BTN_POS(3,1), BTN_SIZE(2,1), F("Tuba"))
     .tag(251).button( BTN_POS(3,2), BTN_SIZE(2,1), F("Bell"))     
     .tag(246).button( BTN_POS(3,3), BTN_SIZE(2,1), F("Sine"))

     .tag(250).button( BTN_POS(5,1), BTN_SIZE(2,1), F("Chimes"))
     .tag(248).button( BTN_POS(5,2), BTN_SIZE(2,1), F("Trumpet"))
     .tag(249).button( BTN_POS(5,3), BTN_SIZE(2,1), F("Music Box"))

     .tag(244).button( BTN_POS(7,1), BTN_SIZE(3,1), F("Xylophone"))
     .tag(245).button( BTN_POS(7,2), BTN_SIZE(3,1), F("Glockenspeil"))
     .tag(252).button( BTN_POS(7,3), BTN_SIZE(2,1), F("Drum Kit"))
     .tag(239).button( BTN_POS(9,3), BTN_SIZE(1,1), F("..."))
     .tag(240).dial  ( BTN_POS(10,1), BTN_SIZE(3,3), dial_min + (dial_max - dial_min) / 255 * volume);

  #define NUM_OCTAVES 2
  #undef  GRID_COLS
  #define GRID_COLS (NUM_OCTAVES*14)

  for(int octave = 0; octave < NUM_OCTAVES; octave++) {
    cmd.fgcolor(white)
       .tag(octave*12 + 1) .button( BTN_POS(octave*14 + 1,4),  BTN_SIZE(2,5), F(""), OPT_FLAT)
       .tag(octave*12 + 3) .button( BTN_POS(octave*14 + 3,4),  BTN_SIZE(2,5), F(""), OPT_FLAT)
       .tag(octave*12 + 5) .button( BTN_POS(octave*14 + 5,4),  BTN_SIZE(2,5), F(""), OPT_FLAT)
       .tag(octave*12 + 6) .button( BTN_POS(octave*14 + 7,4),  BTN_SIZE(2,5), F(""), OPT_FLAT)
       .tag(octave*12 + 8) .button( BTN_POS(octave*14 + 9,4),  BTN_SIZE(2,5), F(""), OPT_FLAT)
       .tag(octave*12 + 10).button( BTN_POS(octave*14 + 11,4), BTN_SIZE(2,5), F(""), OPT_FLAT)
       .tag(octave*12 + 12).button( BTN_POS(octave*14 + 13,4), BTN_SIZE(2,5), F(""), OPT_FLAT);
  
    cmd.fgcolor(black)
       .tag(octave*12 + 2) .button( BTN_POS(octave*14 + 2,4),  BTN_SIZE(2,3), F(""), OPT_FLAT)
       .tag(octave*12 + 4) .button( BTN_POS(octave*14 + 4,4),  BTN_SIZE(2,3), F(""), OPT_FLAT)
       .tag(octave*12 + 7) .button( BTN_POS(octave*14 + 8,4),  BTN_SIZE(2,3), F(""), OPT_FLAT)
       .tag(octave*12 + 9) .button( BTN_POS(octave*14 + 10,4), BTN_SIZE(2,3), F(""), OPT_FLAT)
       .tag(octave*12 + 11).button( BTN_POS(octave*14 + 12,4), BTN_SIZE(2,3), F(""), OPT_FLAT);
  }
  
  #undef GRID_COLS
}

uint32_t PianoScreen::getNoteColor(uint8_t tag) {
  switch(tag % 12) {
    case 1:  return red;                 // C
    case 2:  return (red + orange)/2;    // C#
    case 3:  return orange;              // D
    case 4:  return (orange + yellow)/2; // D#
    case 5:  return yellow;              // E
    case 6:  return green;               // F
    case 7:  return (green + blue)/2;    // F#
    case 8:  return blue;                // G
    case 9:  return (blue + indigo)/2;   // G#
    case 10: return indigo;              // A
    case 11: return (indigo + violet)/2; // A#
    case 0:  return violet;              // B
  }
}

void PianoScreen::buttonStyleCallback(uint8_t tag, uint8_t &style, uint16_t &options, bool post) {
  CommandProcessor cmd;

  // Highlight the selected instrument
  if(tag > 240) {
    if(tag == highlighted_instrument) {
      cmd.fgcolor(0x888888);
    } else {
      cmd.fgcolor(0x000000);
    }
  } else {
    // Hightlight the note that is playing
    if(tag == highlighted_note) {
      cmd.fgcolor(getNoteColor(tag));
    } else {
      switch(tag % 12) {
        case 0:
        case 1:
        case 3:
        case 5:
        case 6:
        case 8:
        case 10:
          cmd.fgcolor(white);
          break;
        default:
          cmd.fgcolor(black);
          break;
      }
    }
  }
}

void PianoScreen::onTouchStart(uint8_t tag) {
  CommandProcessor cmd;
  switch(tag) {
    case 239: GOTO_SCREEN(SongsScreen); break;
    case 241: highlighted_instrument = tag; instrument = PIANO;        break;
    case 242: highlighted_instrument = tag; instrument = ORGAN;        break;
    case 243: highlighted_instrument = tag; instrument = HARP;         break;
    case 244: highlighted_instrument = tag; instrument = XYLOPHONE;    break;
    case 245: highlighted_instrument = tag; instrument = GLOCKENSPIEL; break;
    case 246: highlighted_instrument = tag; instrument = SINE_WAVE;    break;
    case 247: highlighted_instrument = tag; instrument = TUBA;         break;
    case 248: highlighted_instrument = tag; instrument = TRUMPET;      break;
    case 249: highlighted_instrument = tag; instrument = MUSIC_BOX;    break;
    case 250: highlighted_instrument = tag; instrument = CHIMES;       break;
    case 251: highlighted_instrument = tag; instrument = BELL;         break;
    case 252: highlighted_instrument = tag; instrument = HIHAT;        break;
    #define GRID_COLS 6
    case 240: cmd.track_circular (BTN_POS(5,1), BTN_SIZE(2,3), 240); break;
    default:
      const uint32_t color = getNoteColor(tag);
      for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = color;
      }
      FastLED.show();
      if(instrument == HIHAT) {
        switch(tag % 9) {
          case 0: sound.play(CLICK,    NOTE_C3); break;
          case 1: sound.play(SWITCH,   NOTE_C3); break;
          case 2: sound.play(COWBELL,  NOTE_C3); break;
          case 3: sound.play(NOTCH,    NOTE_C3); break;
          case 4: sound.play(HIHAT,    NOTE_C3); break;
          case 5: sound.play(KICKDRUM, NOTE_C3); break;
          case 6: sound.play(POP,      NOTE_C3); break;
          case 7: sound.play(CLACK,    NOTE_C3); break;
          case 8: sound.play(CHACK,    NOTE_C3); break;
        }
      } else {
        sound.play(instrument, NOTE_C3 + tag - 1);
      }
      highlighted_note = tag;
  }
  onRefresh();
  #undef GRID_ROWS
  #undef GRID_COLS
}

void PianoScreen::onIdle() {
  uint16_t value;
  // Once the note finishes playing, unhighlight the key
  if(!sound.is_sound_playing() && highlighted_note) {
    highlighted_note = 0;
    onRefresh();
  }
  // Handle the rotation of the dial.
  switch(CLCD::get_tracker(value)) {
    case 240:
      volume = max(min(1,(float(value) - dial_min) / (dial_max - dial_min)),0) * 255;
      sound.set_volume(volume);
      onRefresh();
      break;
    default: return;
  }
}

/***************************** SONGS SCREEN *****************************/

void SongsScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(0x222222))
     .cmd(CLEAR(true,true,true));
     
  #define GRID_ROWS 5
  #define GRID_COLS 5
  
  cmd.font(font_large)
     .fgcolor(0x111111)
     .text(BTN_POS(1,1), BTN_SIZE(5,1), F("Effects and Songs"))
     .font(font_small)
     .tag(2).button( BTN_POS(1,2), BTN_SIZE(1,1), F("Chimes"))
     .tag(3).button( BTN_POS(1,3), BTN_SIZE(1,1), F("Sad Trombone"))
     .tag(4).button( BTN_POS(1,4), BTN_SIZE(1,1), F("Twinkle"))
     .tag(5).button( BTN_POS(2,2), BTN_SIZE(1,1), F("Fanfare"))
     .tag(6).button( BTN_POS(2,3), BTN_SIZE(1,1), F("USB In"))
     .tag(7).button( BTN_POS(2,4), BTN_SIZE(1,1), F("USB Out"))
     .tag(8).button( BTN_POS(3,2), BTN_SIZE(1,1), F("Bach Toccata"))
     .tag(9).button( BTN_POS(3,3), BTN_SIZE(1,1), F("Bach Joy"))
     .tag(10).button(BTN_POS(3,4), BTN_SIZE(1,1), F("Big Band"))
     .tag(11).button(BTN_POS(4,2), BTN_SIZE(1,1), F("Beeping"))
     .tag(12).button(BTN_POS(4,3), BTN_SIZE(1,1), F("Alarm"))
     .tag(13).button(BTN_POS(4,4), BTN_SIZE(1,1), F("Warble"))
     .tag(14).button(BTN_POS(5,2), BTN_SIZE(1,1), F("Carousel"))
     .tag(15).button(BTN_POS(5,3), BTN_SIZE(1,1), F("Beats"))
  #define MARGIN_T  15
     .tag(1).button( BTN_POS(1,5), BTN_SIZE(5,1), F("Back"));
     
  #undef GRID_ROWS
  #undef GRID_COLS
}

void SongsScreen::onTouchEnd(uint8_t tag) {
  CommandProcessor cmd;
  /* See "src/ui_sounds.h" for sound sequences */
  
  constexpr play_mode_t mode = PLAY_ASYNCHRONOUS;
  switch(tag) {
    case  1: GOTO_SCREEN(PianoScreen);           break;
    case  2: sound.play(chimes, mode);           break;
    case  3: sound.play(sad_trombone, mode);     break;
    case  4: sound.play(twinkle, mode);          break;
    case  5: sound.play(fanfare, mode);          break;
    case  6: sound.play(media_inserted, mode);   break;
    case  7: sound.play(media_removed, mode);    break;
    case  8: sound.play(js_bach_toccata, mode);  break;
    case  9: sound.play(js_bach_joy, mode);      break;
    case 10: sound.play(big_band, mode);         break;
    case 11: sound.play(beeping, mode);          break;
    case 12: sound.play(alarm, mode);            break;
    case 13: sound.play(warble, mode);           break;
    case 14: sound.play(carousel, mode);         break;
    case 15: sound.play(beats, mode);            break;
  }
}

/***************************** MAIN PROGRAM *****************************/

void setup() {
  onStartup();
}

void loop() {
  onIdle();
}
