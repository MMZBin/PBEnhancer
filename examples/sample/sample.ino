/*
  使用できるイベントは
  ・単押し            (SINGLE)
  ・長押し            (LONG)
  ・ダブルクリック     (DOUBLE)
  ・立ち上がりエッジ   (RISING_EDGE)
  ・立ち下がりエッジ   (FALLING_EDGE)
  ・状態変化          (CHANGE_INPUT)
  ・押している間ずっと (PRESSING)
  ・話している間ずっと (RELEASING)
  の8つです。
*/
/*
  The available events are:
  -Single press           (SINGLE)
  -Long press             (LONG)
  -Double click           (DOUBLE)
  -Rising edge            (RISING_EDGE)
  -Falling edge           (FALLING_EDGE)
  -Input change           (CHANGE_INPUT)
  -Pressing continuously  (PRESSING)
  -Releasing continuously (RELEASING)
*/

/*
  "PBEnhancer::Event::[イベント名]"の形でイベントを指定します。 例:"PBEnhancer::Event::SINGLE"
  最初にusing Event = PBEnhancer::Event;と書くと、"Event::SINGLE"のように"PBEnhancer::"の部分を省略できます。
*/
/*
  Specify the events in the format "PBEnhancer::Event::[event name]". For example: "PBEnhancer::Event::SINGLE".
  By writing "using Event = PBEnhancer::Event;" first, you can then abbreviate "PBEnhancer::" to just "Event::".
*/

#include <PBEnhancer.h>

                                  //'PBEnhancer::Event'を'Event'と略せるように登録しています。不要な場合は省略できます。
using Event = PBEnhancer::Event; //To define for abbreviation of identifiers. (It can be specified in the form like PBEnhancer::Event::SINGLE without writing this.)

                                              //PBEnhancerオブジェクトを生成します。第三引数以降は省略可能です。
PBEnhancer pb(2, INPUT_PULLUP, 200, 100, 20); //Instantiate a PBEnhancer object. Arguments beyond the third one are optional.

//([ピン番号], [入力モード(INPUTかINPUT_PULLUP)], 長押し時間, ダブルクリックの間隔, デバウンス(チャタリング防止)時間)の順番で指定してください。
//Please specify in the following order: (pin number, input mode (INPUT or INPUT_PULLUP), long press duration, double-click interval, debounce time).

/*
  //このように書くこともできます。
  //You can also write it like this.
  PBEnhancer pb(2, INPUT_PULLUP);
*/

const uint8_t LED = 13;

//LEDのON/OFFを切り替える関数です。今回の例では長押しで呼び出されます。
//The function to toggle LED on and off.
void toggleLed() {
  static bool state = false;

  digitalWrite(LED, state);
  state = !state;
}

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);

                                               //イベントを指定してコールバック関数を登録します。
  pb.registerCallback(Event::LONG, toggleLed); //Registering a callback function specifying the type of event.

}

void loop() {
  //コールバック関数を使用しなくても、hasOccurred()メソッドを使うことでイベントの発生を知ることができます。
  //You can also confirm using the hasOccurred() method without registering a callback function.
  if (pb.hasOccurred(Event::DOUBLE)) {
    Serial.println("Double clicked!");
  }

                //このメソッドが呼び出されるたびに判定が行われるため、できるだけ早い周期で呼び出し続けることをおすすめします。
  pb.update(); //It is recommended to call this method at the earliest possible cycle to update events.
}
