#include <PBEnhancer.h>

                                  //'PBEnhancer::Event'を'Event'と略せるように登録しています。不要な場合は省略できます。
using Event = PBEnhancer::Event; //To define for abbreviation of identifiers. (It can be specified in the form like PBEnhancer::Event::SINGLE without writing this.)

                                              //PBEnhancerオブジェクトを生成します。第三引数以降は省略可能です。
PBEnhancer pb(2, INPUT_PULLUP, 200, 100, 20); //Instantiate a PBEnhancer object. Arguments beyond the third one are optional.

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
