#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WiFiMulti.h>

// Cập nhật thông tin
// Thông tin về wifi
#define ssid "Fatlab"
#define password "12345678@!"
// Thông tin về MQTT Broker
#define mqtt_server "fawvietnam.xyz" // Thay bằng thông tin của bạn
#define mqtt_topic_pub "demo"   //Giữ nguyên nếu bạn tạo topic tên là demo
#define mqtt_topic_sub "cmd"
#define LED D1 

const uint16_t mqtt_port = 1883; //Port của CloudMQTT

WiFiClient espClient;
PubSubClient client(espClient);

ESP8266WiFiMulti wifiMulti;

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 5000;

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(LED, OUTPUT);    // LED pin as output.
  digitalWrite(LED, LOW);
  Serial.begin(115200);
  // setup_wifi();

  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);

  Serial.begin(115200);
  Serial.println("\nESP8266 Multi WiFi example");

  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);

  // Register multi WiFi networks
  wifiMulti.addAP("Xiaomi", "bean2020");
  wifiMulti.addAP("TP-Link_1768", "abcd1234");
  // wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");
  // More is possible
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
  
}
// Hàm kết nối wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
// Hàm call back để nhận dữ liệu
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  if ((char)payload[0] == '1'){
    digitalWrite(LED, HIGH);
    Serial.println("ON");
    delay(100);
  }
  else{
     digitalWrite(LED, LOW);
     Serial.println("OFF");
     delay(100);
  }
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
//  String myString = String((char*)payload);
//  Serial.println(myString);
  Serial.println();
}
// Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect() {
  // Chờ tới khi kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Thực hiện kết nối với mqtt user và pass
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Khi kết nối sẽ publish thông báo
      client.publish(mqtt_topic_pub, "ESP_reconnected");
      // ... và nhận lại thông tin này
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Đợi 5s
      delay(5000);
    }
  }
}
void loop() {

    // Maintain WiFi connection
  if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
    Serial.print("WiFi connected: ");
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi not connected!");
  }
  
  // Kiểm tra kết nối
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
