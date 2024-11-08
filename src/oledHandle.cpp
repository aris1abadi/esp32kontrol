#include <Arduino.h>
#include <U8g2lib.h>
#include <MUIU8g2.h>
//#include <SimpleRotary.h>
#include <timeHandle.h>
// #include <Versatile_RotaryEncoder.h>
#include <kontrolHandle.h>
#include <mySensorsHandle.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <ezButton.h>


#define VRX_PIN  36 // ESP32 pin GPIO39 (ADC3) connected to VRX pin
#define VRY_PIN  39 // ESP32 pin GPIO36 (ADC0) connected to VRY pin
#define SW_PIN   4 // ESP32 pin GPIO17 connected to SW  pin

#define LEFT_THRESHOLD  1000
#define RIGHT_THRESHOLD 3000
#define UP_THRESHOLD    1000
#define DOWN_THRESHOLD  3000

#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

ezButton button(SW_PIN);

int valueX = 0 ; // to store the X-axis value
int valueY = 0 ; // to store the Y-axis value
int bValue = 0; // To store value of the button
int command = COMMAND_NO;

int lastCommand = COMMAND_NO;

// U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

// U8g2 User Interface
MUIU8G2 mui;


uint8_t formInfo = 1;
uint8_t setupStatus = false;


#define LEN_DATATASK 6

extern int dataTaskLen;
// global variables for menu redraw and input event handling
unsigned long delayJoystick = 0;
uint8_t is_redraw = 1;
uint8_t rotate_event = 0; // 0 = not turning, 1 = CW, 2 = CCW
uint8_t press_event = 0; // 0 = not pushed, 1 = pushed
uint8_t long_press_event = 0; // 0 = not pushed, 1 = pushed
bool isEnable[LEN_DATATASK] = {false,false,false,false,false,false};
const char *nama[LEN_DATATASK + 1] = {"Dummy","Auto Temperature","Auto Humidity","Auto Lengas","Auto Genangan","Auto Intermittent","Auto Campur"};
float sensorValue[LEN_DATATASK] = {32.4,80.4,75,1,27.5,90};

extern myTask dataTask[];

uint16_t aktuator1_idx = 0;
uint16_t aktuator2_idx = 4;
uint16_t sensor1Use_idx = 0;
uint16_t lengas_idx = 0;
uint16_t temp_idx = 0;
uint16_t hum_idx = 0;

uint8_t targetBawah = 0;
uint8_t targetAtas = 0;

uint8_t tesCount = 0;
long tesDelay = millis();

unsigned long lastClickTime = 0;
const unsigned long doubleClickThreshold = 300;  // Interval maksimal untuk mendeteksi double-click
bool singleClickFlag = false;

//list selector
const char *aktuatorSelect[] = { "Aktuator1", "Aktuator2", "Aktuator3", "Aktuator4", "Aktuator5", "Aktuator6", "Aktuator7", "Aktuator8", "Aktuator9", "Aktuator10", "Aktuator11", "Aktuator12", "Aktuator13", "Aktuator14", "Aktuator15", "Aktuator16", "Aktuator17", "Aktuator18", "Aktuator19", "Aktuator20","Aktuator21", "Aktuator22", "Aktuator23", "Aktuator24", };
const char *lengasSelect[] = {"Lengas1","Lengas2","Lengas3","Lengas4"};
const char *tempSelect[]= {"SensorTemp1","SensorTemp2","SensorTemp3","SensorTemp4"};
const char *humSelect[]= {"SensorHum1","SensorHum2","SensorHum3","SensorHum4"};

uint16_t aktuatorSelect_name_list_get_cnt(void *data) {
  return sizeof(aktuatorSelect)/sizeof(*aktuatorSelect);    /* number of animals */
}
const char *aktuatorSelect_name_list_get_str(void *data, uint16_t index) {
  return aktuatorSelect[index];
}

uint16_t lengasSelect_name_list_get_cnt(void *data) {
  return sizeof(lengasSelect)/sizeof(*lengasSelect);    /* number of animals */
}
const char *lengasSelect_name_list_get_str(void *data, uint16_t index) {
  return lengasSelect[index];
}

uint16_t tempSelect_name_list_get_cnt(void *data) {
  return sizeof(tempSelect)/sizeof(*tempSelect);    /* number of animals */
}
const char *tempSelect_name_list_get_str(void *data, uint16_t index) {
  return tempSelect[index];
}

uint16_t humSelect_name_list_get_cnt(void *data) {
  return sizeof(humSelect)/sizeof(*humSelect);    /* number of animals */
}
const char *humSelect_name_list_get_str(void *data, uint16_t index) {
  return humSelect[index];
}







/*
  global variables which form the communication gateway between the user interface and the rest of the code
*/

uint8_t num_value = 0;
uint8_t bar_value = 0;
uint16_t animal_idx = 0;




/*
  list of animal names
*/
const char *animals[] = { "Bird", "Bison", "Cat", "Crow", "Dog", "Elephant", "Fish", "Gnu", "Horse", "Koala", "Lion", "Mouse", "Owl", "Rabbit", "Spider", "Turtle", "Zebra" };

uint16_t animal_name_list_get_cnt(void *data) {
  return sizeof(animals)/sizeof(*animals);    /* number of animals */
}

const char *animal_name_list_get_str(void *data, uint16_t index) {
  return animals[index];
}


uint8_t mui_hrule(mui_t *ui, uint8_t msg) {
  if ( msg == MUIF_MSG_DRAW ) {
      u8g2.drawHLine(0, mui_get_y(ui), u8g2.getDisplayWidth());
  }
  return 0;
}

uint8_t show_my_data(mui_t *ui, uint8_t msg) {
  if ( msg == MUIF_MSG_DRAW ) {
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);
    u8g2.setCursor(x+5, y);
    u8g2.print("Num:");
    u8g2.setCursor(x+50, y);
    u8g2.print(num_value);

    u8g2.setCursor(x+5, y+12);
    u8g2.print("Bar:");
    u8g2.setCursor(x+50, y+12);
    u8g2.print(bar_value);

    u8g2.setCursor(x+5, y+24);
    u8g2.print("Animal:");
    u8g2.setCursor(x+50, y+24);
    u8g2.print(animal_idx);  
    u8g2.print("=");  
    u8g2.print(animals[animal_idx]);  
  }
  return 0;
}

uint8_t show_target_abmix(mui_t *ui, uint8_t msg) {
  if ( msg == MUIF_MSG_DRAW ) {
    String valTxt = dataTask[(formInfo - 1)].mixAnama;
            valTxt += "(";
            valTxt += dataTask[(formInfo - 1)].targetMixA;
            valTxt += "):";
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);
    u8g2.setCursor(x+5, y);
    u8g2.print(valTxt);
    u8g2.setCursor(x+80, y);
    u8g2.print(num_value);

    //--=-----------
    valTxt = dataTask[(formInfo - 1)].mixBnama;
            valTxt += "(";
            valTxt += dataTask[(formInfo - 1)].targetMixB;
            valTxt += "):";

    u8g2.setCursor(x+5, y+12);
    u8g2.print(valTxt);
    u8g2.setCursor(x+80, y+12);    
    u8g2.print(bar_value); 

    valTxt = dataTask[(formInfo - 1)].mixCnama;
            valTxt += "(";
            valTxt += dataTask[(formInfo - 1)].targetMixC;
            valTxt += "):";

    u8g2.setCursor(x+5, y+24);
    u8g2.print(valTxt);
    u8g2.setCursor(x+80, y+24);
    u8g2.print(animal_idx);  
     
  }
  return 0;
}




uint8_t headerTxtEnable(mui_t *ui, uint8_t msg){
  if ( msg == MUIF_MSG_DRAW ) {
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);
    u8g2.setCursor(x+5, y);
    //u8g2.setFontMode(0);
    //u8g2.setDrawColor(0);
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawBox(0,0,128,11);
    //u8g2.print(nama[(formInfo - 1)]);
    u8g2.setDrawColor(2);
    String hdtxt = "Auto ";
    hdtxt += dataTask[formInfo - 1].nama; 
    //u8g2.drawStr(x+5, y,nama[(formInfo - 1)]);
    u8g2.drawStr(x+5, y,hdtxt.c_str());
    //u8g2.setDrawColor(1);
  }

  return 0;
}

uint8_t headerTxt(mui_t *ui, uint8_t msg){
  if ( msg == MUIF_MSG_DRAW ) {
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);
    u8g2.setCursor(x+5, y);
    u8g2.setFontMode(0);
    u8g2.setDrawColor(1);
    String hdtxt = "Auto ";
    hdtxt += dataTask[formInfo - 1].nama; 
    //u8g2.drawStr(x+5, y,nama[(formInfo - 1)]);
    u8g2.drawStr(x+5, y,hdtxt.c_str());
    
  }

  return 0;
}

uint8_t valueTxt(mui_t *ui, uint8_t msg){
  if ( msg == MUIF_MSG_DRAW ) {
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);   
    u8g2.setCursor(x, y);
    float val = getSensorValue(dataTask[(formInfo - 1)].sensorUse1Type, dataTask[(formInfo - 1)].sensorUse1); 
    u8g2.print(val,1);
    
  }

  return 0;
}

uint8_t countTxt(mui_t *ui, uint8_t msg){
  if ( msg == MUIF_MSG_DRAW ) {
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);   
    u8g2.setCursor(x, y); 

    String tm = "";
  if (getJam() < 10)
  {
    tm += "0";
  }
  tm += String(getJam());
  tm += ":";
  if (getMenit() < 10)
  {
    tm += "0";
  }
  tm += getMenit();
    u8g2.print(tm.c_str());
    
  }

  return 0;
}



uint8_t headerSetup(mui_t *ui, uint8_t msg){
  if ( msg == MUIF_MSG_DRAW ) {
    String hd = "Setup ";
    hd += dataTask[formInfo - 1].nama;
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);
    u8g2.setCursor(x+5, y);
    u8g2.setFontMode(0);
    u8g2.setDrawColor(1);
    u8g2.drawStr(x+5, y,hd.c_str());
    
  }

  return 0;
}

uint8_t simpan_data(mui_t *ui, uint8_t msg){
  if ( msg == MUIF_MSG_DRAW ) {
    //
    Serial.println("simpan update data");
    //mui.gotoForm(1,0);
  }
  return 0;
}




muif_t muif_list[] = {
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),        /* regular font */
  MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tr),        /* bold font */
  MUIF_U8G2_FONT_STYLE(2, u8g2_font_bubble_tr),

  MUIF_RO("HR", mui_hrule),
  MUIF_U8G2_LABEL(),
  MUIF_RO("GP",mui_u8g2_goto_data),  
  MUIF_BUTTON("GC", mui_u8g2_btn_goto_wm_fi),
  MUIF_RO("GE",headerTxtEnable),
  MUIF_RO("GH",headerTxt),
  MUIF_RO("GV",valueTxt),
  MUIF_RO("HS",headerSetup),
  MUIF_RO("TS",countTxt),

  MUIF_U8G2_U16_LIST("A1", &aktuator1_idx, NULL, aktuatorSelect_name_list_get_str, aktuatorSelect_name_list_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),
  MUIF_U8G2_U16_LIST("A2", &aktuator2_idx, NULL, aktuatorSelect_name_list_get_str, aktuatorSelect_name_list_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),

  MUIF_U8G2_U16_LIST("L1", &lengas_idx, NULL, lengasSelect_name_list_get_str, lengasSelect_name_list_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),
  MUIF_U8G2_U16_LIST("T1", &temp_idx, NULL, tempSelect_name_list_get_str, tempSelect_name_list_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),
  MUIF_U8G2_U16_LIST("H1", &hum_idx, NULL, humSelect_name_list_get_str, humSelect_name_list_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),




  MUIF_U8G2_U8_MIN_MAX("TA", &targetAtas, 0, 99, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("TB", &targetBawah, 0, 99, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX_STEP("NB", &bar_value, 0, 16, 1, MUI_MMS_2X_BAR, mui_u8g2_u8_bar_wm_mud_pf),
  MUIF_U8G2_U16_LIST("NA", &animal_idx, NULL, animal_name_list_get_str, animal_name_list_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),

  /* register custom function to show the data */
  MUIF_RO("SH", show_my_data),  
  MUIF_RO("DM", show_target_abmix),  

  /* a button for the menu... */
  MUIF_BUTTON("SV", simpan_data),  
  MUIF_EXECUTE_ON_SELECT_BUTTON("GO", mui_u8g2_btn_goto_wm_fi)  
};


fds_t fds_data[] = 

MUI_FORM(1)
MUI_STYLE(1)
MUI_LABEL(5, 8, "karjoAgro")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_STYLE(2)
MUI_XY("TS",16, 40)

MUI_FORM(2)
MUI_STYLE(1)
//MUI_XYAT("GO", 114, 60, 102, " Ok ") 
//MUI_LABEL(5, 8, "Temperature")
MUI_XY("GH",5,8)
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_STYLE(2)
MUI_XY("GV",30, 40)
MUI_STYLE(0)

MUI_FORM(102)
MUI_STYLE(1)
//MUI_LABEL(5, 8, "Auto Temperature") 
MUI_XY("GE",5,8)
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_STYLE(2)
MUI_XY("GV",30, 40)
MUI_STYLE(0)

MUI_FORM(20)//abmix display
MUI_STYLE(1)
MUI_XY("GH",5,8)
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_XY("DM", 5, 23)

MUI_FORM(120)//abmix  setup display
MUI_STYLE(1)
MUI_XY("GE",5,8)
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_XY("DM", 5, 23)

MUI_FORM(10)
MUI_STYLE(1)
MUI_XY("HS",2,8)
MUI_XY("HR", 0,11)
MUI_STYLE(0)
MUI_LABEL(5,23, "Akt1_Use:")
MUI_LABEL(5,36, "Akt2_Use:")
MUI_LABEL(5,49, "Sensor1 :")
MUI_XY("A1", 64, 23)
MUI_XY("A2", 64, 36)
MUI_XY("L1", 64, 49)
MUI_XYAT("GO", 114, 60, 12, " >> ") 

MUI_FORM(12)
MUI_STYLE(1)
MUI_XY("HS",2,8)
MUI_XY("HR", 0,11)
MUI_STYLE(0)
MUI_LABEL(5,23, "T Bawah:")
MUI_LABEL(5,36, "T Atas :")
MUI_LABEL(5,49, "Type   :")
MUI_XY("TB",64, 23)
MUI_XY("TA", 64, 36)
MUI_XYAT("GO",80,60,15,"Simpan")
MUI_XYAT("GO", 114, 60, 1, "Exit") 

MUI_FORM(15)
MUI_STYLE(1)
MUI_LABEL(5,36, "Tersimpan")
MUI_XYAT("GO", 114, 60, 1, "Exit") 

;

void cek_joystick(){
  button.loop(); // MUST call the loop() function first

  // Read the button value
  bValue = button.getState();

  if (button.isPressed()) {
    Serial.println("The button is pressed");
    // TODO do something here   
  
    
  }

  if (button.isReleased()) {
    Serial.println("The button is released");
    // TODO do something here
    unsigned long currentTime = millis();
    
    // Cek jika waktu antar klik kurang dari threshold (klik ganda)
    if (currentTime - lastClickTime <= doubleClickThreshold) {
      Serial.println("Double Click Detected");
      if(!setupStatus){
      mui.gotoForm(10,0);
      setupStatus = true;
      is_redraw = 1;
    }
    //mui.sendSelectWithExecuteOnSelectFieldSearch();
    is_redraw = 1;
      singleClickFlag = false;  // Reset flag klik tunggal
    } else {
      singleClickFlag = true;  // Set flag klik tunggal jika belum klik ganda
    }

    lastClickTime = currentTime;  // Update waktu klik terakhir
  }

  if (singleClickFlag && millis() - lastClickTime > doubleClickThreshold) {
    Serial.println("Single Click Detected");
    
    if(setupStatus){
      mui.sendSelect(); 
    }else{
      if(formInfo != 0){
        if(dataTask[(formInfo - 1)].enable == 0){
          //enable fungsi task
          dataTask[(formInfo - 1)].enable = 1;
          mui.gotoForm(102,0);
        }else{
          dataTask[(formInfo - 1)].enable = 0;
          mui.gotoForm(2,0);
        }
      }
    }   
    if(mui.getCurrentFormId() == 1){
    setupStatus = false;
    }
    
    is_redraw = 1;
    singleClickFlag = false;  // Reset flag klik tunggal setelah dieksekusi
  }
  // read X and Y analog values
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  // converts the analog value to commands
  // reset commands
  command = COMMAND_NO;

  // check left/right commands
  if (valueX < LEFT_THRESHOLD)
    command = command | COMMAND_LEFT;
  else if (valueX > RIGHT_THRESHOLD)
    command = command | COMMAND_RIGHT;

  // check up/down commands
  if (valueY < UP_THRESHOLD)
    command = command | COMMAND_UP;
  else if (valueY > DOWN_THRESHOLD)
    command = command | COMMAND_DOWN;

  // NOTE: AT A TIME, THERE MAY BE NO COMMAND, ONE COMMAND OR TWO COMMANDS

  if(lastCommand != command){
    lastCommand = command;
  
  // print command to serial and process command
  if (command & COMMAND_LEFT) {
    Serial.println("COMMAND LEFT");
    // TODO: add your task here
    if(!setupStatus){      
      if(++formInfo > dataTaskLen ){
        formInfo = 0;
      }
      if(dataTask[(formInfo - 1)].enable == 1){
        if(formInfo == 0){
          mui.gotoForm(1,0);
        }else{
          if(dataTask[(formInfo - 1)].nama == "ABMix"){
            mui.gotoForm(120,0);
          }else{
            mui.gotoForm(102,0);
          }
        }
      }else{
        if(formInfo == 0){
          mui.gotoForm(1,0);
        }else{
          if(dataTask[(formInfo - 1)].nama == "ABMix"){
            mui.gotoForm(20,0);
          }else{
            mui.gotoForm(2,0);
          }
        }
        
      }
      is_redraw = 1;
    }
    
    
  
  }

  if (command & COMMAND_RIGHT) {
    Serial.println("COMMAND RIGHT");
    // TODO: add your task here
    if(!setupStatus){      
      if(--formInfo < 0 ){
        formInfo = dataTaskLen;
      }
      if(dataTask[(formInfo - 1)].enable == 1){
        if(formInfo == 0){
          mui.gotoForm(1,0);
        }else{
          if(dataTask[(formInfo - 1)].nama == "ABMix"){
            mui.gotoForm(120,0);
          }else{
            mui.gotoForm(102,0);
          }
        }
      }else{
        if(formInfo == 0){
          mui.gotoForm(1,0);
        }else{
          if(dataTask[(formInfo - 1)].nama == "ABMix"){
            mui.gotoForm(20,0);
          }else{
            mui.gotoForm(2,0);
          }
        }
        
      }
      is_redraw = 1;
    }
    
    
    
    

  }

  if (command & COMMAND_UP) {
    Serial.println("COMMAND UP");
    // TODO: add your task here
    if(setupStatus){
      mui.prevField();
      is_redraw = 1;
      
    }
    
  }

  if (command & COMMAND_DOWN) {
    Serial.println("COMMAND DOWN");
    // TODO: add your task here
    if(setupStatus){
      mui.nextField();
      is_redraw = 1;
    }    
  }
  }
}

void updateDisplay(){
  is_redraw = 1;
}

void oled_init()
{

  // Load to the encoder all nedded handle functions here (up to 9 functions)
  // versatile_encoder.setHandleRotate(handleRotate);
  // versatile_encoder.setHandlePressRelease(handlePressRelease);
  // versatile_encoder.setHandleLongPressRelease(handleLongPressRelease);

  analogSetAttenuation(ADC_11db);
  button.setDebounceTime(50); // set debounce time to 50 milliseconds

  u8g2.begin();
  mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);

  Serial.println("karjoAgro start");
  
}

uint8_t m = 24;
static unsigned long updateDelay = millis();

void oled_loop()
{
   if ( mui.isFormActive() ) {

    /* update the display content, if the redraw flag is set */
    if ( is_redraw ) {
      u8g2.firstPage();
      do {
        
  mui.draw();
       
     
      } while( u8g2.nextPage() );
      is_redraw = 0;                    /* clear the redraw flag */
    }    
      
  } else {
      /* the menu should never become inactive, but if so, then restart the menu system */
      mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
  }

   cek_joystick();
   //update display tiap 1 detik
    if((millis() - updateDelay) > 1000){
      updateDelay = millis();
      is_redraw = 1;
    }


  }