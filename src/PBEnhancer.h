/*
    MIT License

    Copyright (c) 2024 MOMIZICH

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef MMZ_PB_ENHANCER
#define MMZ_PB_ENHANCER

class PBEnhancer {
public:
    using CallbackFunc = void (*)();

    //イベントの種類
    enum class Event : uint8_t {
        SINGLE,       //短押し
        LONG,         //長押し
        DOUBLE,       //ダブルクリック
        RISING_EDGE,  //立ち上がりエッジ
        FALLING_EDGE, //立ち下がりエッジ
        CHANGE_INPUT, //状態が変わったとき
        PRESSING,     //押している間毎回
        RELEASING     //離している間毎回
    };

    PBEnhancer(const uint8_t pin, const uint8_t mode, const uint32_t longThreshold=500, const uint32_t doubleThreshold=100);

    uint8_t getPin() const;

    void registerCallback(const Event type, const CallbackFunc func);
    void removeCallback(const Event type);

    void update();  //状態を更新する

    bool hasOccurred(const Event type) const;

private:
    void onPress(const uint32_t now);
    void onRelease(const uint32_t now);
    void onRisingEdge(const uint32_t now);
    void onFallingEdge(const uint32_t now);

    inline void emit(const Event type) { hasOccurred_ |= (1u << static_cast<uint8_t>(type)); }
    void invoke() const; // コールバック関数を呼び出す


    const uint8_t PIN, PIN_MODE, LONG_THRESHOLD, DOUBLE_THRESHOLD;
    static constexpr uint8_t NUM_OF_EVENTS = 8;

    uint32_t pressTime_, releaseTime_;

    bool isPressBak_, isLongPressWait_, isDoubleClickWait_;

    byte hasOccurred_;

    CallbackFunc callbacks_[NUM_OF_EVENTS];
};

#endif
