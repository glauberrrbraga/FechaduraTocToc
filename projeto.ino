//Definindo os pinos
#include <LiquidCrystal.h>
#include <Servo.h> 
Servo Servo1; 
const int piezoBuzzer = A0; //Piezo Buzzer conectado a porta A0
const int botao = 2; //Botão que serve para cadastrar batida
const int botao2 = 13;
const int ledR = 3;
const int ledG = 4; 
const int ledB = 5; 
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

const int sensibilidade = 50;
int posicao = 0; 
int estado = HIGH;
int estado2 = false;
int leitura;
int i, h;
int totaltimeDifferences = 0;
int timeDiff = 0;
const int batidas = 10; //Quantidade de batidas
int codigoSecreto[batidas];
int batidasLidas[batidas];
int taxaRejeicao = 15;
int taxaMedia = 15;

int batidaCompleta = 2400; //Tempo para esperar e ver quantas batidas aconteceram

void setup() {
  Servo1.attach(6);
  pinMode(botao, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  Serial.begin(9600);   
  Serial.println("Inicio");
  digitalWrite(ledB, HIGH);
  lcd.begin(16, 2);
  Servo1.write(3);
  
  
}

void loop() {
  lcd.clear();
  lcd.print("Fechadura TocToc"); 
  lcd.setCursor(0, 1);
  lcd.print("Versao 2.0");  
  
  leitura = analogRead(piezoBuzzer);
  //Se clicar no botao, ele grava o código secreto
  if (digitalRead(botao) == LOW){
    estado2 = false;
    estado = LOW;
    digitalWrite(ledB, LOW);
    digitalWrite(ledG, LOW);
    digitalWrite(ledR, HIGH); 
    gravar();
    estado2 = true;
    Serial.println("Gravou");
  }
  delay(200);
  if (digitalRead(botao2) == LOW && estado2){
      gravar2();
  }
   
}

void gravar(){
  delay(1000);
  lcd.clear();
  i = 0;
  lcd.print("Gravando chave");
  Serial.println("Gravando");  
  unsigned long tempoInicial = millis();
  
  unsigned long atual = 0;
  int j = 0;

   for (i = 0; i < batidas; i++){
    codigoSecreto[i] = 0;
   }
  do{   
    leitura = analogRead(piezoBuzzer);
    if (leitura >= sensibilidade){
      Serial.println("BATEU");
      //grava o tempo entre as batidas
      atual = millis();
      codigoSecreto[j] = atual - tempoInicial;
      j++;
      tempoInicial = atual;
      delay(150); 
      }
    }while ((j < batidas));
    for (i = 0; i < batidas; i++){
        codigoSecreto[i]= map(codigoSecreto[i], 0, batidaCompleta, 0, 100); 
        Serial.println(codigoSecreto[i]);

      }
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    lcd.clear();
    lcd.print("Chave gravada");
    lcd.setCursor(0, 1);
    lcd.print("com sucesso :)");
    delay(2000);
 
    }


void gravar2(){
  digitalWrite(ledG, LOW);
  digitalWrite(ledR, HIGH);
  digitalWrite(ledB, HIGH);
  //delay(1000);
  lcd.clear();
  lcd.print("Aguardando as 10");
  lcd.setCursor(0, 1);
  lcd.print("batidas");
  
  Serial.println("Gravando");  
  unsigned long tempoInicial = millis();
  unsigned long atual = 0;
  int j = 0;
  i = 0;

   for (i = 0; i < batidas; i++){
    batidasLidas[i] = 0;
   }
  do{       
    leitura = analogRead(piezoBuzzer);
    if (leitura >= sensibilidade){
      Serial.println("BATEU");
      //grava o tempo entre as batidas
      atual = millis();
      batidasLidas[j] = atual - tempoInicial;
      j++;
      tempoInicial = atual;
      delay(150); 
      }
    }while (j < batidas);
    
    for (i = 0; i < batidas; i++){
        batidasLidas[i]= map(batidasLidas[i], 0, batidaCompleta, 0, 100); 
        Serial.println(batidasLidas[i]);
    }

    digitalWrite(ledR, HIGH);
    digitalWrite(ledB, LOW);
    digitalWrite(ledG, HIGH);


    lcd.clear();
    lcd.print("Validando....");
    if(validar() == false){
      Serial.println("Não validou");
      digitalWrite(ledR, HIGH);
      digitalWrite(ledB, LOW);
      digitalWrite(ledG, LOW);
      lcd.clear();
      lcd.print("Chave incorreta.");
      delay(2000);
    }
    else{
      Serial.println("Validou :)");
       Servo1.write(85);
       digitalWrite(ledR, LOW);
       digitalWrite(ledB, LOW);
       digitalWrite(ledG, HIGH);
       lcd.clear();
       lcd.print("Aberto");
       delay(6000);
       Servo1.write(3);  
       lcd.clear();
       lcd.print("Fechado");
       delay(2000);
      
    }
    delay(3000);
}

boolean validar(){
  for(i = 0; i < batidas; i++){
    if(abs(codigoSecreto[i] - batidasLidas[i]) > taxaRejeicao){
      Serial.println(abs(codigoSecreto[i] - batidasLidas[i]));
      Serial.println("Deu ruim.");
      return false;
      //break;
    }
     Serial.println(abs(codigoSecreto[i] - batidasLidas[i]));

  }
  
  return true;
}
