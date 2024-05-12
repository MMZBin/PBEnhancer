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

    PBEnhancer(const uint8_t pin, const uint8_t mode, const uint32_t longThreshold=500, const uint32_t doubleThreshold=100, const uint32_t debounceTime=20);

    uint8_t getPin() const;

    void registerCallback(const Event type, const CallbackFunc func);
    void removeCallback(const Event type);

    void update();  //状態を更新する

    bool hasOccurred(const Event type) const;

private:
    inline void onPress(const uint32_t now) {
        emit(Event::PRESSING);

        //立ち上がりエッジのときの処理
        if (!isPressBak_) { onRisingEdge(now); }

        //長押し判定の時間を過ぎたら
        if ((!isHandled_) && (now - pressTime_ > LONG_THRESHOLD)) {
            emit(Event::LONG);
            isHandled_ = true;
        }
    }

    inline void onRelease(const uint32_t now) {
        emit(Event::RELEASING);

        //立ち下がりエッジのときの処理
        if (isPressBak_) { onFallingEdge(now); }

        //時間を過ぎた&ダブルクリック待ち(再度押されなかったとき)
        if ((isDoubleClickWait_) && (now - releaseTime_ > DOUBLE_THRESHOLD)) {
            emit(Event::SINGLE);
            isDoubleClickWait_ = false;
        }

        isHandled_ = false;
    }

    inline void onRisingEdge(const uint32_t now) {
        emit(Event::RISING_EDGE);
        emit(Event::CHANGE_INPUT);

        pressTime_ = now; //押し始めた時間を記録
        lastTransTime_ = now;

        //未処理&ダブルクリック待ちのとき
        if ((isDoubleClickWait_) && (!isHandled_)) {
            emit(Event::DOUBLE);
            isHandled_ = true;
        }

        isDoubleClickWait_ = false;
    }

    inline void onFallingEdge(const uint32_t now) {
        emit(Event::FALLING_EDGE);
        emit(Event::CHANGE_INPUT);

        releaseTime_ = now; //離し始めた時間を記録
        lastTransTime_ = now;
        isDoubleClickWait_ = !isHandled_; //すでに処理されていれば待たない
    }

    inline void emit(const Event type) { hasOccurred_ |= (1 << static_cast<uint8_t>(type)); }

    inline void invoke() const {
        for (uint8_t i = 0; i < NUM_OF_EVENTS; i++) {
            if ((hasOccurred(static_cast<Event>(i))) && (callbacks_[i] != nullptr)) {
                callbacks_[i]();
            }
        }
    }


    const uint8_t PIN, PIN_MODE;
    const uint32_t LONG_THRESHOLD, DOUBLE_THRESHOLD, DEBOUNCE_TIME;
    static constexpr uint8_t NUM_OF_EVENTS = 8;

    uint32_t pressTime_, releaseTime_, lastTransTime_;

    bool isPressBak_, isHandled_, isDoubleClickWait_;

    byte hasOccurred_; //0番目のビットが短押し,1番目のビットが長押し...のように対応している

    CallbackFunc callbacks_[NUM_OF_EVENTS];
};

#endif
