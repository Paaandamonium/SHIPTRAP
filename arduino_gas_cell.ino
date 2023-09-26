#include <SPI.h>
#include <Ethernet.h>

int bypassPin = 13;  // SET TO READ PIN 13 FOR THE INPUT SIGNAL IF THE PURIFIER HAS GONE INTO BYPASS MODE (BYPASS = ON/HIGH/1/TRUE)
int val = 0;      // variable to store the read value
byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x75, 0x79}; // HAS BEEN SET FOR ETHERNET SHIELD

EthernetClient client;

int    HTTP_PORT   = 8086;
String HTTP_METHOD = "POST";
char HOST_NAME[] = "192.168.0.10";
String PATH_NAME   = "/trigger";
String queryString = "?value1=26&value2=70";;
String toprint;
int dataSize;

void setup() {
  pinMode(bypassPin, INPUT);  // sets the digital pin 13 as output

  Serial.begin(9600);

  // initialize the Ethernet shield using DHCP:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to obtain an IP address using DHCP");
    while(true);
 
 
  }
 if(client.connect(HOST_NAME, HTTP_PORT)) {
    // if connected:
    Serial.println("Connected to server");
    // make a HTTP request:
    // send HTTP header
/*
    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  } else {// if not connected:
    Serial.println("connection failed");
  }
*/
}
}

void writeToDb(int val) {
  toprint = "";
  toprint +="status,device=purifier_shiptrap,pin=LSS status=";
  toprint.concat(val);
  
  Ethernet.maintain();
  int conState = client.connect(HOST_NAME, HTTP_PORT);
  Serial.print(conState);
  delay(70);
  Serial.println(F("Reply from InfluxDB"));
    while(client.available()) {
      Serial.print((char)client.read());
      
    }
  
  delay(50);
  if(conState <= 0) {
    Serial.print(F("Could not connect to InfluxDB server, Error #:"));
    Serial.println(conState);
    client.stop();
    return;
  }

  toprint +='\n';    
  dataSize = toprint.length();
  Serial.print(toprint);
  // connect to web server on port 80:
  client.println(F("POST /api/v2/write?org=superheavy&bucket=test_bucket&precision=ns HTTP/1.1")); // DB LOCATION: PORT 8086: INFLUX DB: TEST BUCKET
  client.println(F("Host: 192.168.0.10:8086"));
  // SHIP +  test token
  client.println(F("Authorization: Token qm7Il83VqLscF70XZTQi_oT_EhP2AWhkIqtJDt6psX1_15m6DrMP9MvOFuSY3PEjRrbfCdRU9Iq-It34QDxBqA=="));
 
  client.println(F("Content-Type:text/plain; charset=utf-8")); // charset=utf-8
  //client1.println("Accept: application/json");
  client.print(F("Content-Length: "));
  client.println(dataSize);
  client.println();
    // send HTTP body
   client.print(toprint);

  //  delay(50);
  //     while(client.connected()) {
  //     if(client.available()){
  //       // read an incoming byte from the server and print it to serial monitor:
  //       char c = client.read();
  //       Serial.print(c);
  //     }
  //   }
    client.stop();
}


void loop() {
  val = digitalRead(bypassPin);   // read the input pin
  writeToDb(val);
  delay(30000); // READS THE SIGNAL ONCE EVERY 30 SECONDS
}
