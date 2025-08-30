// 1. MEMASUKKAN LIBRARY
// Ini seperti meminjam buku "Cara Mengatur Servo" dari perpustakaan
#include <Servo.h>

// 2. MENENTUKAN PIN (Seperti memberi nama pada colokan)
const int pinMotor = 9;      // Colokan untuk motor
const int pinServo = 10;     // Colokan untuk servo
const int pinTrig = 11;      // Colokan "pengirim" sensor ultrasonik
const int pinEcho = 12;      // Colokan "penerima" sensor ultrasonik

// 3. MEMBUAT OBJEK DAN VARIABEL
Servo myServo; // Membuat "remote control" untuk servo kita
long durasi;   // Variabel untuk menyimpan waktu pantul suara
long jarakCm;  // Variabel untuk menyimpan hasil hitungan jarak

void setup() {
  // Pengaturan awal yang hanya dijalankan sekali
  Serial.begin(9600); // Menyalakan monitor untuk lihat tulisan dari Arduino
  
  // Mengatur colokan sebagai OUTPUT (mengirim sinyal) atau INPUT (menerima sinyal)
  pinMode(pinMotor, OUTPUT);
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  
  myServo.attach(pinServo); // Menghubungkan "remote" servo ke colokannya
  myServo.write(90);        // Perintahkan servo ke posisi tengah (90 derajat)
}

void loop() {
  // Bagian ini akan diulang-ulang terus menerus
  
  // --- LANGKAH 1: MENGUKUR JARAK ---
  // Kirim sinyal suara ultrasonik
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  
  // Dengar dan ukur berapa lama sinyal itu kembali
  durasi = pulseIn(pinEcho, HIGH);
  
  // Hitung durasi menjadi jarak dalam centimeter
  jarakCm = durasi * 0.034 / 2;
  
  // --- LANGKAH 2: TAMPILKAN JARAK DI MONITOR ---
  Serial.print("Jarak: ");
  Serial.print(jarakCm);
  Serial.print(" cm");

  // --- LANGKAH 3: BUAT KEPUTUSAN BERDASARKAN JARAK ---
  // Jika jaraknya antara 5 cm dan 50 cm...
  if (jarakCm >= 5 && jarakCm <= 50) {
    // Maka, lakukan aksi:
    
    // Atur kecepatan motor (semakin dekat, semakin cepat)
    int kecepatanMotor = map(jarakCm, 5, 50, 255, 0);
    analogWrite(pinMotor, kecepatanMotor);
    
    // Atur sudut servo (semakin dekat, sudutnya semakin kecil)
    int sudutServo = map(jarakCm, 5, 50, 0, 180);
    myServo.write(sudutServo);
    
    // Tampilkan aksi di monitor
    Serial.print(" | Aksi: Kecepatan -> ");
    Serial.print(kecepatanMotor);
    Serial.print(", Sudut -> ");
    Serial.println(sudutServo);

  } else {
    // Jika tidak (terlalu dekat atau terlalu jauh)...
    // Maka, diam saja:
    
    analogWrite(pinMotor, 0); // Matikan motor
    myServo.write(90);        // Kembalikan servo ke tengah
    
    // Tampilkan status diam
    Serial.println(" | Aksi: Diam");
  }

  // --- LANGKAH 4: KASIH JEDA SEBENTAR ---
  delay(100); // Tunggu sebentar sebelum mengulang lagi dari awal
}
