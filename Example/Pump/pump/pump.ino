#include <ESP8266WiFi.h>
#include <PubSubClient.h>
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

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(LED, OUTPUT);    // LED pin as output.
  digitalWrite(LED, LOW);
  Serial.begin(115200);
  setup_wifi();
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
  // Kiểm tra kết nối
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
