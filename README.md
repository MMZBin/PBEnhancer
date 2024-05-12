This library uses English that has been machine-translated from Japanese.

# PBEnhancer
このライブラリはタクトスイッチの制御を簡単にするためのものです。
コールバック関数と評価式による判定の両方に対応しています。

This library is designed to simplify the control of tactile switches.
It supports both callback functions and evaluation-based criteria for decision-making.

## 機能 Functions

#### Event列挙型
  - イベントのタイプを指定するために使用します。 Used to specify the type of event.
      - SINGLE
      - LONG
      - DOUBLE
      - RISING_EDGE
      - FALLING_EDGE
      - CHANGE_INPUT
      - PRESSING
      - RELEASING

#### PBEnhancer(uint8_t pin, uint8_t mode, uint32_t longThreshold, uint32_t doubleThreshold, uint32_t debounceTime)
  - PBEnhancerオブジェクトを生成します。 Generates a PBEnhancer object.
    - pin: イベントの対象となるピン番号を指定します。 Specifies the pin number targeted for events.
    - mode: ピンの使用モードを設定します。 Sets the pin's operating mode.
      - INPUT
      - INPUT_PULLUP
    - longThreshold (省略可) (optional)
      - 長押しだと判定する時間をミリ秒単位で設定します。デフォルトでは500です。 Sets the time threshold in milliseconds to determine a long press. Default is 500.
    - doubleThreshold (省略可) (optional)
      - ダブルクリックだと判定する時間をミリ秒単位で設定します。デフォルトでは100です。 Sets the time threshold in milliseconds to determine a double click. Default is 100.
    - debounceTime (省略可) (optional)
      - デバウンス時間(不安定な入力の変化を無視する時間)をミリ秒単位で設定します。デフォルトでは20です。 Set the debounce time (the time to ignore changes in unstable inputs) in milliseconds. The default is 20.

#### uint8_t getPin()
  - 登録されているピンを返します。 Returns the registered pin.

#### void registerCallback(Event type, CallbackFunc func)
  - 特定のイベントが発生した際に呼び出されるコールバック関数を登録します。 Registers a callback function to be called when a specific event occurs.
    - type: 登録するイベントのタイプを指定します。 Specifies the type of event to register.
    - func: コールバック関数を指定します。 Specifies the callback function.

#### void removeCallback(Event type)
  - 特定のイベントに登録されたコールバック関数を削除します。 Removes the callback function registered for a specific event.
    - type: 削除するイベントを指定します。 Specifies the event to remove.
 
#### void update()
  - イベントの状態を更新します。 Updates the state of events.
  - このメソッドは定期的(一般に数十ミリ秒以内)に繰り返し呼び出される必要があります。 This method must be called periodically (usually within a few tens of milliseconds).
  - このメソッドが呼び出される箇所が一箇所である必要はありません。 It does not need to be called from a single location.

### bool hasOccurred(Event type)
  - 指定したイベントが発生したかどうかを真偽値で返します。 Returns a boolean value indicating whether the specified event has occurred.
  - イベントが発生したと判定されてから次にupdate()メソッドが呼び出されるまでの間trueになります。 It returns true from the moment the event is determined to have occurred until the next update() method is called.
    - type: 発生したか調べるイベントのタイプを指定します。 Specifies the type of event to check for occurrence.

## 使用例 Usage Example

```
#include <PBEnhancer.h>

                                  //'PBEnhancer::Event'を'Event::'と略せるように登録しています。不要な場合は省略できます。
using Event = PBEnhancer::Event; //To define for abbreviation of identifiers. (It can be specified in the form like PBEnhancer::Event::SINGLE without writing this.)

                                          //PBEnhancerオブジェクトを生成します。3つ目(長押し判定時間)と4つ目(ダブルクリック判定時間)は任意です。
PBEnhancer pb(2, INPUT_PULLUP, 200, 100); //Create a PBEnhancer object. The third and fourth arguments are optional.

const uint8_t LED = 13;

//LEDのON/OFFを切り替える関数です。今回の例では長押しで呼び出されます。
//The function to toggle LED on and off.
void changeLed() {
  static bool state = false;

  digitalWrite(LED, state);
  state = !state;
}

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);

                                                //イベントを指定してコールバック関数を登録します。
  pb.registerCallback(Event::LONG, changeLed); //Registering a callback function specifying the type of event.

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
```
