#include <SoftwareSerial.h>
SoftwareSerial monitorSerial(0, 1); // RX, TX

const byte CH_PD = 5;
const byte RST = 6;
#define rede "Trojan"
#define senha "pianodecauda"

void setup() {
  monitorSerial.begin(38400);
  monitorSerial.println("--------------");
  pinMode(CH_PD, OUTPUT);
  digitalWrite(CH_PD, HIGH);
  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(100);
  digitalWrite(RST, HIGH);
  delay(1000);
  Serial.begin(9600);
  Serial.setTimeout(5000);

  monitorSerial.println("ESP8266 Demo - aguardando 1 segundo");
  delay(1000);
  monitorSerial.println("Enviando RST");
  Serial.println("AT+RST");
  recebeResposta();
  recebeResposta(); // o reset tem 2 respostas - OK (antes) e ready (depois)
  monitorSerial.println("Selecionando modo de operacao misto (AP e estacao)");
  Serial.println("AT+CWMODE=3");
  recebeResposta();
  monitorSerial.println("Listando redes visiveis");
  Serial.println("AT+CWLAP");
  recebeResposta();
  monitorSerial.println("Conectando-se a uma rede");
  String comando = "AT+CWJAP=\"";
  comando += rede;
  comando += "\",\"";
  comando += senha;
  comando += "\"";
  Serial.println(comando);
  recebeResposta();
  monitorSerial.println("Listando endereco IP (pode dar ping no segundo deles)");
  Serial.println("AT+CIFSR");
  recebeResposta();
}

void loop() {
}

void recebeResposta() {
  int limite = 7000;
  unsigned long chegada = millis();
  boolean continuar = true;
  String S = "";
  unsigned long ultimochar = 0;
  while (continuar) {
    if (Serial.available()) {
      char c = Serial.read();
      ultimochar = millis();
      S = S + c;
      monitorSerial.print(c);
      if (c == 10) { // LF, fim da linha recebida
        byte p = S.indexOf(13);
        String S1 = S.substring(0, p);
        if (S1 == "OK") continuar = false;
        if (S1 == "ready") continuar = false;
        if (S1 == "no change") continuar = false;
        if (S1 == "ERROR") continuar = false;
        S = "";
      }
    }
    if (millis() - chegada > limite) continuar = false;
  }
}
