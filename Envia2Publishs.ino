
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "Felipe"
#define wifi_password "39588474"
#define mqtt_server "test.mosquitto.org"

#define mqtt_user "not used in this example"
#define mqtt_password "not used in this example"
#define topicStatusum "mriotstatus1"
#define topicStatusdois "mriotstatus2"
#define topicComandoum "mriotcomando1"
#define topicComandodois "mriotcomando2"

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value;
int valuedois;

// INICIO RECEBE AS MENSAGENS DE COMANDO DO BROKER

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("COMANDO RECEBIDO [");
  Serial.print(topic);
  Serial.print("], ");

  // convertendo o comando na porra de uma String
  String StringComando = "";
  StringComando = String((char*)payload);
  String comandoStr = "";

  for (int i = 0; i < length; i++) {
    comandoStr += (char)payload[i];
  }
  Serial.println("PUBLICANDO STATUS NO BROKER");
  // publicando no servidor broker
}

void setup_wifi() {
  delay(1000);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("CONECTANDO EM ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP do IOT: ");
  Serial.println(WiFi.localIP());
}

// FINAL RECEBE AS MENSAGENS DE COMANDO DO BROKER

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Conectando no MQTT broker ...");
    // Attempt to connect
    if (client.connect("TestMQTT")) { //* NOME DO CLIENTE
      Serial.println("conectado ao broker mosquitto");
      Serial.println("LENDO O COMANDO1...");
      client.subscribe(topicComandoum, 0);
      Serial.println("LENDO O COMANDO2...");
        client.subscribe(topicComandodois, 0);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" tentando de novo em  5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }//final enquanto mqtt estiver offline
}//final reconect

void setup() {
  Serial.begin(115200);
  setup_wifi();
  // put your setup code here, to run once:
  client.setServer(mqtt_server, 1883);
  // colocando o listener de menssagem do servidor
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // INICIO MANDANDO O STATUS A CADA SEGUNDO
  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    // publicando no servidor broker
    value = analogRead(A0);
    client.publish(topicStatusum, String(value).c_str(), true);
    Serial.println(value);
    valuedois = (value+1);
    client.publish(topicStatusdois, String(valuedois).c_str(), true);
    Serial.println(valuedois);
  }

  // FINAL MANDANDO O STATUS A CADA SEGUNDO
}// final loop
