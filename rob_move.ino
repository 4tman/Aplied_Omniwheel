#include <DynamixelWorkbench.h>



// #define CAM_ID       1  //  ID камеры  
#define DEVICE_NAME "3" //Dynamixel on SerialUSB3(USART3)  <-OpenCM 485EXP



#define BAUDRATE  57600
#define DXL_ID_1  1
#define DXL_ID_2  2
#define DXL_ID_3  3
#define DXL_ID_4  4

DynamixelWorkbench dxl_wb;
unsigned long previousMillis = 0; // stores last time cam was updated

void setup() 
{



  SerialUSB.begin(57600);


  // while(!SerialUSB); // Wait for Opening SerialUSB Monitor
  const char *log;
  bool result = false;

  // Массив ID сервоприводов
  uint8_t dxl_ids[4] = {DXL_ID_1, DXL_ID_2, DXL_ID_3, DXL_ID_4};
  uint16_t model_number = 0;

  result = dxl_wb.init("3", BAUDRATE, &log);
  if (result == false)
  {
    SerialUSB.println(log);
    SerialUSB.println("Failed to init");
  }
  else
  {
    SerialUSB.print("Succeeded to init : ");
    SerialUSB.println(BAUDRATE);  
  }

  // Инициализация всех 4 сервоприводов
  for (int i = 0; i < 4; i++) 
  {
    result = dxl_wb.ping(dxl_ids[i], &model_number, &log);
    if (result == false)
    {
      SerialUSB.print("Failed to ping DXLD ID: ");
      SerialUSB.println(dxl_ids[i]);
      SerialUSB.println(log);
    }
    else
    {
      SerialUSB.print("Succeeded to ping - ID: ");
      SerialUSB.print(dxl_ids[i]);
      SerialUSB.print(" Model: ");
      SerialUSB.println(model_number);
    }

    // Переключаем в режим колеса (непрерывное вращение)
    result = dxl_wb.wheelMode(dxl_ids[i], 0, &log);
    if (result == false)
    {
      SerialUSB.print("Failed to change wheel mode for ID: ");
      SerialUSB.println(dxl_ids[i]);
      SerialUSB.println(log);
    }
    else
    {
      SerialUSB.print("Succeed to change wheel mode for ID: ");
      SerialUSB.println(dxl_ids[i]);
    }
  }

  SerialUSB.println("All servos initialized in wheel mode");
  SerialUSB.println("Ready to control...");




}

void loop() {
  if (SerialUSB.available()) {
    char c = SerialUSB.read();

    SerialUSB.print("RX: ");
    SerialUSB.println(c);

    if (c == 'F') vpered();
    else if (c == 'B') nazad();
    else if (c == 'L') vlevo();
    else if (c == 'R') vpravo();
    else if (c == 'S') stop();
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
  SerialUSB.println("STOPPING all wheels");
  for (int i = 0; i < 4; i++) 
  {
    dxl_wb.goalVelocity(i+1, 0);
  }
  delay(1000);

}

void vpravo(){//вправо 
 
  
  // Пример: дифференциальное вращение (для поворота)
  SerialUSB.println("DIFFERENTIAL rotation - turning");
  dxl_wb.goalVelocity(1, 150);  // Левые колеса вперед
  dxl_wb.goalVelocity(2, 150);  // Левые колеса вперед
  dxl_wb.goalVelocity(3, -150); // Правые колеса назад
  dxl_wb.goalVelocity(4, -150); // Правые колеса назад


}
void vlevo(){   
 
  
  // Пример: дифференциальное вращение (для поворота)
  SerialUSB.println("DIFFERENTIAL rotation - turning");
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

   SerialUSB.println("Moving all wheels FORWARD"); //поворот направо
  for (int i = 0; i < 4; i++) 
  {
    dxl_wb.goalVelocity(i+1, 100); // Скорость +100 (вперед)
  }
  
}

void l_povorot(){//поворот налево

    // Пример: вращение всех сервоприводов назад
  SerialUSB.println("Moving all wheels BACKWARD");
  for (int i = 0; i < 4; i++) 
  {
    dxl_wb.goalVelocity(i+1, -100); // Скорость -100 (назад)
  }

}