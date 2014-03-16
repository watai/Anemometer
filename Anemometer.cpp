/**************************************
 * Anemometer.cpp
 * created by Taiki Watai, 21 Nov 2013. 
 **************************************/
#include <avr/interrupt.h>
#include <Arduino.h>
#include "Anemometer.h"

volatile static unsigned short wmanemo, wmanemo_count;
static unsigned short buf_wmanemo[12];

// 割込み　風速
void wmeters_int0 () {
	wmanemo_count ++;
}

// 割込み　風速バッファ
ISR(TIMER1_COMPA_vect) {
	if (! OCR1A) { OCR1A = (F_CPU / 256 / 4) - 1; }

	// 0.25秒毎に3秒分(12サンプル)をバッファ=最大瞬間風速
	unsigned short a = 0;
	for (int i = 11; i > 0; i --) {
		buf_wmanemo[i] = buf_wmanemo[i - 1];
		a += buf_wmanemo[i];
	}

	buf_wmanemo[0] = wmanemo_count;
	a += buf_wmanemo[0];
	wmanemo_count = 0;
	if (a > wmanemo) wmanemo = a; // 最高値更新
}

void Anemometer::setup() {
	wmanemo = 0;
	wmanemo_count = 0;

	for (int i = 0; i < 12; i ++) {
		buf_wmanemo[i] = 0;
	}

	pinMode(ANEMO, INPUT);
	attachInterrupt(0, wmeters_int0, FALLING);

	// タイマー1初期化
        // set compare match register for 4Hz, 250ms update rate
	TCNT1 = 0; // タイマーカウンタ
	OCR1A = (F_CPU / 256 / 4) - 1; // F_CPU(8MHz:8000000L)/256 比較A 4Hz, 割り込みクロック数指定
	TCCR1A = 0; // CTC動作
	TCCR1B = _BV(WGM12)|_BV(CS12); // ck/256
	TIMSK1 = _BV(OCIE1A); // 割り込み
}

// 風速（m/s）取得
void Anemometer::get_anemo (unsigned int *anemo) { 
	cli();
	// 風速 (0.667) m/s/Hz　秒速(m/s) 
        *anemo = (unsigned int)wmanemo;
	//*anemo = (float)wmanemo * 0.667 / 3.0;
	wmanemo = 0;
	sei();
}
