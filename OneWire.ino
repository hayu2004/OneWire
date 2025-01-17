/*
Chương trình này được viết dựa trên giao thức 1-Wire dùng cho việc kết nối DS18B20 và Arduino.
Nguyễn Cảnh Việt, Khoa Vật lý, Đại học Khoa học Tự nhiên.
*/

//set tín hiệu I/O cho pin
void  SetMode(unsigned char mode){
  if (mode==0){
    pinMode(DS18B20_PIN, OUTPUT);    //Chân của master sẽ GỬI tín hiệu tới chân DATA của DS18B20
    digitalWrite(DS18B20_PIN,LOW);
  }
  else
    pinMode(DS18B20_PIN, INPUT);    //Chân của master sẽ NHẬN tín hiệu từ chân DATA của DS18B20
}
//---------------------------------------------------------
unsigned char DS18B20_Reset()       //khởi tạo cảm biến 
{
  unsigned char result;
  SetMode(0);
  
  delayMicroseconds(480);
  SetMode(1);//release bus
  delayMicroseconds(70);  //550-480
  result = digitalRead(DS18B20_PIN);// Read Presence pulse
  delayMicroseconds(410); //960-550

  return result;
}
//---------------------------------------------------------
//viết bit 1
void DS18B20_Write1()
{
  SetMode(0);
  delayMicroseconds(6);
  SetMode(1);//release bus
  delayMicroseconds(64);//54+10
}
//---------------------------------------------------------
//viết bit 0
void DS18B20_Write0()// viết bit 0
{
  SetMode(0);  
  delayMicroseconds(60);
  SetMode(1);//release bus
  delayMicroseconds(10);  
}
//---------------------------------------------------------
//viết bit
void DS18B20_Write_Bit(unsigned char b)//viết bit
{
  if(b == 1)
  {
    DS18B20_Write1();
  }
  else
  {
    DS18B20_Write0();
  }
}
//---------------------------------------------------------
//đọc bit
unsigned char DS18B20_Read_Bit()
{
  unsigned char result;
  SetMode(0);  
  delayMicroseconds(6);
  SetMode(1);
  delayMicroseconds(4);
  result = digitalRead(DS18B20_PIN);
  delayMicroseconds(60);
  return result;
}
//---------------------------------------------------------
//Master ĐỌC 1 byte từ DS18B20. Vì khi DS18B20 gửi 1 byte cho master, từng bit được gửi đi và bit LSB luôn được gửi đi trước. 
//---------------------------------------------------------
unsigned int DS18B20_Read_Byte(void) {
   unsigned char result=0;
   unsigned char i;
   for(i=0;i<8;i++){                        //cho vòng lặp chạy 8 lần để đọc đủ 1 byte, mỗi vòng lặp đọc 1 bit.
    result>>=1;                             //dịch result sang phải 1
    result=result | DS18B20_Read_Bit()<< 7; //lưu bit đọc được vào bit cuối cùng của result
    }
   return result;
}
//---------------------------------------------------------
//Master GHI 1 byte ra DS18B20. Vì khi master ghi 1 byte tới DS18B20, từng bit được gửi đi và bit LSB luôn được gửi đi trước.
//---------------------------------------------------------
void DS18B20_Write_Byte(unsigned char b)
{
  unsigned char i ;

  for(i=0;i<8;i++)               //cho vòng lặp chạy 8 lần để đọc đủ 1 byte do DS18B20 gửi đến
  {
    DS18B20_Write_Bit(b & 0x01);//lấy ra bit thấp nhất của bite b và gửi đi 
    b >>= 1;                    //dịch bit tiếp theo xuống thành bit thấp nhất 
  }
}


void setup(){
  Serial.begin(9600);
  DS18B20_Reset();
  delay(1000);
  // Write Scratch pad
  DS18B20_Write_Byte(0xCC);               //skip ROM
  delay(1000);
  //Ví dụ về cài đặt độ phân giải cho ADC tích hợp trong DS18B20
  
  DS18B20_Reset();
  DS18B20_Write_Byte(0xCC);               //skip ROM
  DS18B20_Write_Byte(0x4E);               //Write cratchpad
  
  DS18B20_Write_Byte(0x00);               //Write first byte(value 0) to byte 2
  DS18B20_Write_Byte(0x00);               //Write second byte(value 0) to byte 3
  DS18B20_Write_Byte(0x5F);               //Write third byte(value 5F) to  CONFIG register(byte 4)
  // 1F-> 9 bit resolution; 3F-> 10 bit resolution; 5F -> 11 bit resolution;7F->12 bit resolution; 
 
}

void loop(){
// Các bạn tự viết chương trình thêm vào mục này
  
}
