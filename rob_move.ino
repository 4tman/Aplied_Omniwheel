#include <DynamixelWorkbench.h>




// #define CAM_ID       1  //  ID камеры  
#define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP




#define BAUDRATE  57600
#define DXL_ID_1  1
#define DXL_ID_2  2
#define DXL_ID_3  3
#define DXL_ID_4  4


DynamixelWorkbench dxl_wb;
unsigned long previousMillis = 0; // stores last time cam was updated


void setup() 
{




  Serial.begin(57600);



  // while(!Serial); // Wait for Opening Serial Monitor
  const char *log;
  bool result = false;


  // Массив ID сервоприводов
  uint8_t dxl_ids[4] = {DXL_ID_1, DXL_ID_2, DXL_ID_3, DXL_ID_4};
  uint16_t model_number = 0;


  result = dxl_wb.init("3", BAUDRATE, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to init");
  }
  else
  {
    Serial.print("Succeeded to init : ");
    Serial.println(BAUDRATE);  
  }


  // Инициализация всех 4 сервоприводов
  for (int i = 0; i < 4; i++) 
  {
    result = dxl_wb.ping(dxl_ids[i], &model_number, &log);
    if (result == false)
    {
      Serial.print("Failed to ping DXLD ID: ");
      Serial.println(dxl_ids[i]);
      Serial.println(log);
    }
    else
    {
      Serial.print("Succeeded to ping - ID: ");
      Serial.print(dxl_ids[i]);
      Serial.print(" Model: ");
      Serial.println(model_number);
    }


    // Переключаем в режим колеса (непрерывное вращение)
    result = dxl_wb.wheelMode(dxl_ids[i], 0, &log);
    if (result == false)
    {
      Serial.print("Failed to change wheel mode for ID: ");
      Serial.println(dxl_ids[i]);
      Serial.println(log);
    }
    else
    {
      Serial.print("Succeed to change wheel mode for ID: ");
      Serial.println(dxl_ids[i]);
    }
  }


  Serial.println("All servos initialized in wheel mode");
  Serial.println("Ready to control...");





}


void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'F') vpered();
    if (c == 'B') nazad();
    if (c == 'L') vlevo();
    if (c == 'R') vpravo();
    if (c == 'S') stop();
  }
}








void vpered(){//вперёд 
  dxl_wb.goalVelocity(1, -250);  
  dxl_wb.goalVelocity(2, 250);  
  dxl_wb.goalVelocity(3, 250); 
  dxl_wb.goalVelocity(4, -250);// Правые колеса назад
}


void stop(){ 
 
   // Остановка
  Serial.println("STOPPING all wheels");
  for (int i = 0; i < 4; i++) 
  {
    dxl_wb.goalVelocity(i+1, 0);
  }
  delay(1000);


}


void vpravo(){//вправо 
 
  
  // Пример: дифференциальное вращение (для поворота)
  Serial.println("DIFFERENTIAL rotation - turning");
  dxl_wb.goalVelocity(1, 150);  // Левые колеса вперед
  dxl_wb.goalVelocity(2, 150);  // Левые колеса вперед
  dxl_wb.goalVelocity(3, -150); // Правые колеса назад
  dxl_wb.goalVelocity(4, -150); // Правые колеса назад



}
void vlevo(){   
 
  
  // Пример: дифференциальное вращение (для поворота)
  Serial.println("DIFFERENTIAL rotation - turning");
  dxl_wb.goalVelocity(1, -150);  // Левые колеса вперед
  dxl_wb.goalVelocity(2, -150);  // Левые колеса вперед
  dxl_wb.goalVelocity(3, 150); // Правые колеса назад
  dxl_wb.goalVelocity(4, 150); // Правые колеса назад


}
void nazad(){


  dxl_wb.goalVelocity(1, 250);  
  dxl_wb.goalVelocity(2, -250);  
  dxl_wb.goalVelocity(3, -250); 
  dxl_wb.goalVelocity(4, 250); 


}



void r_povorot(){//поворот направо


   Serial.println("Moving all wheels FORWARD"); //поворот направо
  for (int i = 0; i < 4; i++) 
  {
    dxl_wb.goalVelocity(i+1, 100); // Скорость +100 (вперед)
  }
  
}


void l_povorot(){//поворот налево


    // Пример: вращение всех сервоприводов назад
  Serial.println("Moving all wheels BACKWARD");
  for (int i = 0; i < 4; i++) 
  {
    dxl_wb.goalVelocity(i+1, -100); // Скорость -100 (назад)
  }


}