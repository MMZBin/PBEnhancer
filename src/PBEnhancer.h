#ifndef MMZ_PB_ENHANCER
#define MMZ_PB_ENHANCER

class PBEnhancer {
public:
    using CallbackFunc = void (*)();

    //イベントの種類
    enum class Event {
        SINGLE,       //短押し
        LONG,         //長押し
        DOUBLE,       //ダブルクリック
        RISING_EDGE,  //立ち上がりエッジ
        FALLING_EDGE, //立ち下がりエッジ
        CHANGE_INPUT, //状態が変わったとき
        PRESSING,     //押している間毎回
        RELEASING     //離している間毎回
    };

    PBEnhancer(uint8_t pin, uint8_t mode, uint32_t longThreshold=500, uint32_t doubleThreshold=100);

    uint8_t getPin() const;

    void registerCallback(Event type, CallbackFunc func);

    void removeCallback(Event type);

    void update();  //状態を更新する

    bool isOccurred(Event type) const;

private:
    void onPress(uint32_t now);

    void onRelease(uint32_t now);

    void onRisingEdge(uint32_t now);

    void onFallingEdge(uint32_t now);

    void invoke(Event type); // コールバック関数を呼び出す

    uint8_t getIndex(Event type) const;

    uint8_t pin_;              //ピン番号
    uint8_t mode_;             //接続モード(INPUT/INPUT_PULLUPを想定)
    uint32_t longThreshold_;   //長押し判定をする時間
    uint32_t doubleThreshold_; //ダブルクリック判定をする時間0

    uint32_t pressTime_;       //押している時間
    uint32_t releaseTime_;     //離している時間

    bool isPressBak_;          //isPressの前回のポーリングのときの値(エッジ検出用)
    bool isHandled_;           //判定されたかどうか
    bool isDoubleClickWait_;   //ダブルクリック待ちかどうか

    byte isOccurred_;

    CallbackFunc callbacks_[8];
};

#endif
