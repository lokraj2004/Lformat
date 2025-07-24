/* Hi, I am N.Lokeshraj. I have written this header file. I have created a Mobile App named Coupling_Together which is used as Android Serial Monitor 
for Microcontroller or any serial devices and sends the received sensor values to firebase cloud. From the cloud, the values are distributed
to subscribers. For passing the values from microcontroller to the App using USB-OTG, this Header file ("Lformat.h") is required. This 
Lformat.h contains the format to pass the values from microcontroller to App. It consists of only one Function "Lformat". It can be easily 
integrated to any projects. */

#include <Lformat.h>   //Included Lformat library


void setup() {
  Serial.begin(9600);  
  delay(1000);         
}

void loop() {    // Format: name;value;unit+id
  String sensor = "Temperature:";
  String n = "NULL";             
  double temperature = 28.5;  
  String unit = "C";
  int id = 1;             // direct method of providing values


  String batch = String("Temperature:") + ";" + String(temperature) + ";" + unit + ";" + String(id); // format in which CT app understands
  Serial.println(batch);   // Output: Temperature:;28.5;C;1


  //You can also use Lformat function to achieve same format
  Lformat("Temperature:",temperature,"C",1);     // Output: Temperature:;28.5;C;1

  //Lformat contains Serial.println in default, if you wish to manipulate the formatted data. It also returns the formatted string data
  String batch1 =Lformat("Temperature:",temperature,"C",2 );  
  Serial.println(batch1);   // Output: Temperature:;28.5;C;2

  // Lformat Arguments "Lformat(String,double,String,int)" must be passed to execute the function, otherwise error occurs
  // Lformat can accept Null values for first three arguments, but in fourth argument (int) Null is not accepted.

  Lformat("NULL",temperature,"C",3);         // OUTPUT: NULL;28.5;C;3
  Lformat(sensor,temperature,"NULL",4);      //OUTPUT: Temperature:;28.5;NULL;4
 
  
  //Lformat fourth Argument 'id' should be in range of 1-10, otherwise error occurs

  delay(1000);
}
