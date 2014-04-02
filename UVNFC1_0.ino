



#define EEPROM_CLRT 0x50;


long int EEPROM_ADR = 0;
long int MEASCOUNT = 0;

byte TFLAG=0;
byte NDEFFLAG=1;
boolean COR_APP=flase;




/*writes one byte of data to stated address in memory*/
void EEPROM_Write(int address, byte data){
  byte add_lo = address;                //split new 16 bit address into high and low bytes
  byte add_hi = (address >> 8);
  Wire.beginTransmission(EEPROM_CRTL); //start transmission & send control byte
  Wire.write(add_hi);                   //send address high byte
  Wire.write(add_lo);                   //send address low byte
  Wire.write(data);                     //send data byte
  Wire.endTransmission();               //end transmission
  delay(5);
}


/*update the number of measurments stored in the header in the EEPROM*/
void Update_MeasCount(void){
    byte meascount[3];
    meascount[0]=MEASCOUNT;         //split the meas conter in to 3 bytes
    meascount[1]=MEASCOUNT>>8;
    meascount[2]MEASCOUNT>>16
    EEPROM_Write(9, meascount[0]);  //store the 3 bytes of the meas count in the EEPROM
    EEPROM_Write(8, meascount[1]);
    EEPROM_Write(7, meascount[2]);
}


/*take a measurment from the sensors and store it in the EEPROM and update the meas count in the data header in EEPROM*/
void Take_Store_Meas(Void){
  MEASCOUNT++;                    //incrament the number of measurments taken
  int uv = analogRead(A0);        //read and store the uv value
  EEPROM_Write(EEPROM_ADR, uv);
  EEPROM_ADR++;
  int amb = analogRead(A1);        //read and store the ambiant value
  EEPROM_Write(EEPROM_ADR, amb);
  EEPROM_ADR++;
  update_MeasCount()               //update the number of measurments taken in EEPROM
}


/*stop the device from takeing any more measurments*/
void Stop_Meas(void){
 //disable timer interupts 
}

void Copy_Header_To_Memory(void){
  
}


/*this function compares two arrays and returns whither or not they are the sane as an boolean*/
boolean array_cmp(int *a, int *b){
      int n;
      for (n=0;n<len_a;n++) if (a[n]!=b[n]) return false;    // test each element to be the same. if not, return false
      return true;                                           //ok, if we have not returned yet, they are equal
}


/* this function checks if the NDEF data is from the correct app and returns the NDEF data as a byte array*/
byte[] Get_NDEF_Data(Void){
    byte buffer[1024];
    byte R_mime[27];
    int x;
    
    uint16_t NDEFMessageLength = 0;                        //get the NDEF Message Length 
    nfc.Read_Continuous(0, buffer, (uint16_t)0x1C);
    NDEFMessageLength = buffer[0x1A] << 8 | buffer[0x1B];
    Tag4Length = NDEFMessageLength + 0x1C;                 // entire Tag length  
    nfc.Read_Continuous(0, buffer, Tag4Length);            //get the entire tag data
    
    for(x=0x1F, x<=0x39, x++){                             //extrace AAR from message
      R_mime[x-0x1F]=buffer[x]
    }
    if (array_cmp(R_mime, MIME_TYPE) == true){             //check if message is from correct app
            COR_APP=true;
      }else{
            CORR_APP= false;
      }
    return buffer[];
}
  
/*this function copys data from the nedf message to the header in EEPROM*/
void Copy_Header_To_Memory(Byte data[]){
  for(i=0; i<6; i++){            //copy the data from the phone to the EEPROM (Rory thinks the payload starts at byte 63 may be 62)
    EEPROM_Write(x, data[x+63];
  }
  for (i=6; i<9; i++){           //set the 3 bytes of the mesurment counter in EEPROM to zero
    EEPROM_Write(x, 0);
  }
}


void loop (void){
  
  if(TFLAG==1){
    TFLAG=0;
    take_store_meas();
    Update_NDEF();
  }
  if(NDEFFLAG==1){
    NDEFFLAG=0;
    if(READ){
      Stop_Meas();
    }
    if(WRITE){
      byte data[]=Get_NDEF_Data();
      if(CORR_APP==true){
        copy_header_to_memory(data[]);
        EEPROM_ADR=10;
        timer_no= (data[69]*10);
      }
    }
}
      
  
