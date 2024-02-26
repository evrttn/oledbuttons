 #include <U8glib.h>
 #include <EEPROM.h>

volatile bool menuMode = false;

uint8_t k[5];
int p = 0;

char *variaveis[] = {"Kp", "Kc", "Kd", "PWM", "Led"};

U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE);

char dataBuf[10];

void clear_screen() {  
  u8g.firstPage(); 
  do {
    for(int i = 0; i < 128; i++ )
    {
      for(int j = 0; j < 64; j++ ){
        u8g.setPrintPos(i, j);
        u8g.print(""); 
      }
      
    }
  } while ( u8g.nextPage() ); 
}

void lerVariaveis(){
  k[0] = EEPROM.read(0);
  k[1] = EEPROM.read(1);
  k[2] = EEPROM.read(2);
  k[3] = EEPROM.read(3);
  k[4] = EEPROM.read(4);
}

void salvarVariaveis(){
  EEPROM.write(0, k[0]);
  EEPROM.write(1, k[1]);
  EEPROM.write(2, k[2]);
  EEPROM.write(3, k[3]);
  EEPROM.write(4, k[4]);
}

void setup() {
  PORTB= PORTB | (1 << PB0) | (1 << PB1) | (1 << PB2);
  
  PORTD = PORTD | (1 << PD3);

  attachInterrupt(digitalPinToInterrupt(3), showMenu, RISING);

  lerVariaveis(); 
  clear_screen();   
}

void showMenu(){
  menuMode = !menuMode;
}

void handleMenu(){
  bool refresh = true;  
  while(menuMode){
    if((PINB & (1 << PB0)) == 0){
      k[p]--;
      refresh = true;
      delay(300);
    }
    else if((PINB & (1 << PB1)) == 0){
      k[p]++;
      refresh = true;
      delay(300);
    }
    else if((PINB & (1 << PB2)) == 0){
      p++;
      p = p % 5;
      refresh = true;
      delay(300);
    }
  
    if(refresh){
      u8g.firstPage();  // Put information on OLED
      do {
        u8g.setFont(u8g_font_8x13B);
        u8g.drawStr(10, 27, variaveis[p]);  // 
    
        sprintf(dataBuf, "%d", k[p]);  
        u8g.setFont(u8g_font_fub20);// u8g_font_fub30
        u8g.drawStr(10, 57, dataBuf);  // display time
      } while ( u8g.nextPage() );
      refresh = false;
    }
  }  
}


void loop() {
  // put your main code here, to run repeatedly:
  if(menuMode){
    handleMenu();    
    salvarVariaveis();    
    clear_screen();
  }
}
