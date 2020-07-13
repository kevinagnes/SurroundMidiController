USBMIDI_Interface midi;

CD74HC4067 mux = {2, {A4, A5, A6, A7}}; // Multiplexer 4067 {SIGpin, {S0,S1,S2,S3}}

PanJoystick joystickSurround = {{A1, A0}, mux.pin(3), CHANNEL_10}; // Surround Panner Joystick { {X,Y} , Activate Button No, Channel}
// ArrowJoystick vPOTstick = {{A1, A0}, mux.pin(6), mux.pin(3), mux.pin(10), CHANNEL_6}; 

Bank<1> SurroundBank(2);
Bankable::CCValue<1> SurroundJoy[2] = { 
  {{SurroundBank,BankType::CHANGE_CHANNEL},{MCU::V_POT_1, CHANNEL_10}},
  {{SurroundBank,BankType::CHANGE_CHANNEL},{MCU::V_POT_2, CHANNEL_10}}, 
};

Button Fn4(mux.pin(1)),
       encoderfn1(mux.pin(8)),
       encoderfn2(mux.pin(6)),
       joybutton(mux.pin(10));

Bank<4> bank0 = {1},
        bank1 = {1}, 
        bank2 = {1}, 
        bank3 = {1}, 
        bank4 = {1}, 
        bank5 = {1}, 
        bank6 = {1}, 
        bank7 = {1}, 
        bank8 = {1}, 
        bank9 = {1},
        bank[10] = {bank0,bank1,bank2,bank3,bank4,bank5,bank6,bank7,bank8,bank9};  

Bank<1> sixButBank2(6);
Bank<1> vPot(8);


Bankable::CCButton muxb[6] = { // buttonMidi[6] = {119,118,117,116,115,114}; 
  {{bank[0], BankType::CHANGE_CHANNEL}, mux.pin(0), {buttonMidi[0], CHANNEL_1}},
  {{bank[1], BankType::CHANGE_CHANNEL}, mux.pin(2), {buttonMidi[1], CHANNEL_1}},
  {{bank[2], BankType::CHANGE_CHANNEL}, mux.pin(4), {buttonMidi[2], CHANNEL_1}},
  {{bank[3], BankType::CHANGE_CHANNEL}, mux.pin(5), {buttonMidi[3], CHANNEL_1}},
  {{bank[4], BankType::CHANGE_CHANNEL}, mux.pin(7), {buttonMidi[4], CHANNEL_1}},
  {{bank[5], BankType::CHANGE_CHANNEL}, mux.pin(9), {buttonMidi[5], CHANNEL_1}}, 
};

Encoder enc0(23,22), 
        enc1(25,24),
        enc2(27,26),
        enc3(29,28),
        enc4(31,30),
        enc5(33,32),
        enc6(35,34),
        enc7(37,36),
        enc8(39,38),
        enc9(41,40);

Bankable::CCButton encbEQ[] = // {20,24,28,52,56,60,85,102,106,110}
{
       {{bank[0], BankType::CHANGE_CHANNEL}, bEnc[0], {1 ,CHANNEL_10}},
       {{bank[1], BankType::CHANGE_CHANNEL}, bEnc[1], {2 ,CHANNEL_10}},
       {{bank[2], BankType::CHANGE_CHANNEL}, bEnc[2], {3 ,CHANNEL_10}},
       {{bank[3], BankType::CHANGE_CHANNEL}, bEnc[3], {4 ,CHANNEL_10}},
       {{bank[4], BankType::CHANGE_CHANNEL}, bEnc[4], {5 ,CHANNEL_10}},
       {{bank[5], BankType::CHANGE_CHANNEL}, bEnc[5], {6 ,CHANNEL_10}},
       {{bank[6], BankType::CHANGE_CHANNEL}, bEnc[6], {7 ,CHANNEL_10}},
       {{bank[7], BankType::CHANGE_CHANNEL}, bEnc[7], {8 ,CHANNEL_10}},
       {{bank[8], BankType::CHANGE_CHANNEL}, bEnc[8], {9 ,CHANNEL_10}}, 
       {{bank[9], BankType::CHANGE_CHANNEL}, bEnc[9], {10,CHANNEL_10}},
};        
        
Bankable::BorrowedCCRotaryEncoder encEQ[10] = 
{
  {{bank[0], BankType::CHANGE_CHANNEL},enc0, {1, CHANNEL_7}, 2, 4,}, // Low Cut    (Freq,Gain,Q,OnOff)
  {{bank[1], BankType::CHANGE_CHANNEL},enc1, {2, CHANNEL_7}, 2, 4,}, // Low Shelf  (Freq,Gain,Q,OnOff)
  {{bank[2], BankType::CHANGE_CHANNEL},enc2, {3, CHANNEL_7}, 2, 4,}, // Peak 1     (Freq,Gain,Q,OnOff)
  {{bank[3], BankType::CHANGE_CHANNEL},enc3, {4, CHANNEL_7}, 2, 4,}, // Peak 2     (Freq,Gain,Q,OnOff)
  {{bank[4], BankType::CHANGE_CHANNEL},enc4, {5, CHANNEL_7}, 2, 4,}, // Peak 3     (Freq,Gain,Q,OnOff)
  {{bank[5], BankType::CHANGE_CHANNEL},enc5, {6, CHANNEL_7}, 2, 4,}, // Peak 4     (Freq,Gain,Q,OnOff)
  {{bank[6], BankType::CHANGE_CHANNEL},enc6, {7, CHANNEL_7}, 2, 4,}, // High Shelf (Freq,Gain,Q,OnOff)
  {{bank[7], BankType::CHANGE_CHANNEL},enc7, {8, CHANNEL_7}, 2, 4,}, // High Cut   (Freq,Gain,Q,OnOff)
  {{bank[8], BankType::CHANGE_CHANNEL},enc8, {9, CHANNEL_7}, 2, 4,}, 
  {{bank[9], BankType::CHANGE_CHANNEL},enc9, {10,CHANNEL_7}, 2, 4,},   
}; 

Bankable::CCButton encbGENERAL[] = 
{
       {{bank[0], BankType::CHANGE_CHANNEL}, bEnc[0], {1 ,CHANNEL_4}},
       {{bank[1], BankType::CHANGE_CHANNEL}, bEnc[1], {2 ,CHANNEL_4}},
       {{bank[2], BankType::CHANGE_CHANNEL}, bEnc[2], {3 ,CHANNEL_4}},
       {{bank[3], BankType::CHANGE_CHANNEL}, bEnc[3], {4 ,CHANNEL_4}},
       {{bank[4], BankType::CHANGE_CHANNEL}, bEnc[4], {5 ,CHANNEL_4}},
       {{bank[5], BankType::CHANGE_CHANNEL}, bEnc[5], {6 ,CHANNEL_4}},
       {{bank[6], BankType::CHANGE_CHANNEL}, bEnc[6], {7 ,CHANNEL_4}},
       {{bank[7], BankType::CHANGE_CHANNEL}, bEnc[7], {8 ,CHANNEL_4}},
       {{bank[8], BankType::CHANGE_CHANNEL}, bEnc[8], {9 ,CHANNEL_4}}, 
       {{bank[9], BankType::CHANGE_CHANNEL}, bEnc[9], {10,CHANNEL_4}},
}; 

Bankable::BorrowedCCRotaryEncoder encGENERAL[10] = 
{
  {{bank[0], BankType::CHANGE_CHANNEL}, enc0, {1,  CHANNEL_1}, 2, 4,},
  {{bank[1], BankType::CHANGE_CHANNEL}, enc1, {2,  CHANNEL_1}, 2, 4,},
  {{bank[2], BankType::CHANGE_CHANNEL}, enc2, {3,  CHANNEL_1}, 2, 4,},
  {{bank[3], BankType::CHANGE_CHANNEL}, enc3, {4,  CHANNEL_1}, 2, 4,},
  {{bank[4], BankType::CHANGE_CHANNEL}, enc4, {5,  CHANNEL_1}, 2, 4,},
  {{bank[5], BankType::CHANGE_CHANNEL}, enc5, {6,  CHANNEL_1}, 2, 4,},
  {{bank[6], BankType::CHANGE_CHANNEL}, enc6, {7,  CHANNEL_1}, 2, 4,},
  {{bank[7], BankType::CHANGE_CHANNEL}, enc7, {8,  CHANNEL_1}, 2, 4,},
  {{bank[8], BankType::CHANGE_CHANNEL}, enc8, {9,  CHANNEL_1}, 2, 4,},
  {{bank[9], BankType::CHANGE_CHANNEL}, enc9, {10, CHANNEL_1}, 2, 4,},   
};

Bankable::CCButton encbSMART[] = 
{
       {{bank[0], BankType::CHANGE_CHANNEL}, bEnc[0], {MCU::V_POT_SELECT_1,CHANNEL_1}},
       {{bank[1], BankType::CHANGE_CHANNEL}, bEnc[1], {MCU::V_POT_SELECT_2,CHANNEL_1}},
       {{bank[2], BankType::CHANGE_CHANNEL}, bEnc[2], {MCU::V_POT_SELECT_3,CHANNEL_1}},
       {{bank[3], BankType::CHANGE_CHANNEL}, bEnc[3], {MCU::V_POT_SELECT_4,CHANNEL_1}},
       {{bank[4], BankType::CHANGE_CHANNEL}, bEnc[4], {MCU::V_POT_SELECT_5,CHANNEL_1}},
       {{bank[5], BankType::CHANGE_CHANNEL}, bEnc[5], {MCU::V_POT_SELECT_6,CHANNEL_1}},
       {{bank[6], BankType::CHANGE_CHANNEL}, bEnc[6], {MCU::V_POT_SELECT_7,CHANNEL_1}},
       {{bank[7], BankType::CHANGE_CHANNEL}, bEnc[7], {MCU::V_POT_SELECT_8,CHANNEL_1}},
       {{bank[8], BankType::CHANGE_CHANNEL}, bEnc[8], {                 40,CHANNEL_1}}, 
       {{bank[9], BankType::CHANGE_CHANNEL}, bEnc[9], {                 41,CHANNEL_1}},
};

Bankable::BorrowedCCRotaryEncoder encSMART[10] = 
{
  {{bank[0], BankType::CHANGE_CHANNEL}, enc0, {MCU::V_POT_1,  CHANNEL_1}, 2, 4,},
  {{bank[1], BankType::CHANGE_CHANNEL}, enc1, {MCU::V_POT_2,  CHANNEL_1}, 2, 4,},
  {{bank[2], BankType::CHANGE_CHANNEL}, enc2, {MCU::V_POT_3,  CHANNEL_1}, 2, 4,},
  {{bank[3], BankType::CHANGE_CHANNEL}, enc3, {MCU::V_POT_4,  CHANNEL_1}, 2, 4,},
  {{bank[4], BankType::CHANGE_CHANNEL}, enc4, {MCU::V_POT_5,  CHANNEL_1}, 2, 4,},
  {{bank[5], BankType::CHANGE_CHANNEL}, enc5, {MCU::V_POT_6,  CHANNEL_1}, 2, 4,},
  {{bank[6], BankType::CHANGE_CHANNEL}, enc6, {MCU::V_POT_7,  CHANNEL_1}, 2, 4,},
  {{bank[7], BankType::CHANGE_CHANNEL}, enc7, {MCU::V_POT_8,  CHANNEL_1}, 2, 4,},
  {{bank[8], BankType::CHANGE_CHANNEL}, enc8, {          24,  CHANNEL_1}, 2, 4,},
  {{bank[9], BankType::CHANGE_CHANNEL}, enc9, {          25,  CHANNEL_1}, 2, 4,},   
};



Bankable::CCValue<4> cc[10] = // POT EQ
{
  {{bank[0], BankType::CHANGE_CHANNEL}, {1 , CHANNEL_7}},
  {{bank[1], BankType::CHANGE_CHANNEL}, {2 , CHANNEL_7}},
  {{bank[2], BankType::CHANGE_CHANNEL}, {3 , CHANNEL_7}},
  {{bank[3], BankType::CHANGE_CHANNEL}, {4 , CHANNEL_7}},
  {{bank[4], BankType::CHANGE_CHANNEL}, {5 , CHANNEL_7}},
  {{bank[5], BankType::CHANGE_CHANNEL}, {6 , CHANNEL_7}},
  {{bank[6], BankType::CHANGE_CHANNEL}, {7 , CHANNEL_7}},
  {{bank[7], BankType::CHANGE_CHANNEL}, {8 , CHANNEL_7}},
  {{bank[8], BankType::CHANGE_CHANNEL}, {9 , CHANNEL_7}},
  {{bank[9], BankType::CHANGE_CHANNEL}, {10, CHANNEL_7}},
};

Bankable::CCValue<4> ccc[10] = // EQ BUTTON
{
  {{bank[0], BankType::CHANGE_CHANNEL}, {1,  CHANNEL_10}},
  {{bank[1], BankType::CHANGE_CHANNEL}, {2,  CHANNEL_10}},
  {{bank[2], BankType::CHANGE_CHANNEL}, {3,  CHANNEL_10}},
  {{bank[3], BankType::CHANGE_CHANNEL}, {4,  CHANNEL_10}},
  {{bank[4], BankType::CHANGE_CHANNEL}, {5,  CHANNEL_10}},
  {{bank[5], BankType::CHANGE_CHANNEL}, {6,  CHANNEL_10}},
  {{bank[6], BankType::CHANGE_CHANNEL}, {7,  CHANNEL_10}},
  {{bank[7], BankType::CHANGE_CHANNEL}, {8,  CHANNEL_10}},
  {{bank[8], BankType::CHANGE_CHANNEL}, {9,  CHANNEL_10}},
  {{bank[9], BankType::CHANGE_CHANNEL}, {10, CHANNEL_10}},   
};

 MCU::VPotRing cccc[8] = // POT SMART
 {
   { 1, CHANNEL_1},
   { 2, CHANNEL_1},
   { 3, CHANNEL_1},
   { 4, CHANNEL_1},
   { 5, CHANNEL_1},
   { 6, CHANNEL_1},
   { 7, CHANNEL_1},
   { 8, CHANNEL_1}
};  

MCU::TimeDisplay timedisplay = {};

MCU::VU vu[] = 
{
  {1, MCU::VUDecay::Default},
  {2, MCU::VUDecay::Default},
  {3, MCU::VUDecay::Default},
  {4, MCU::VUDecay::Default},
  {5, MCU::VUDecay::Default},
  {6, MCU::VUDecay::Default},
};

// MCU::LCD<> trackName = {};
MCU::LCD<> parametersNameValue = {};

Bankable::CCValue<1> ccmuxTransport[6] = 
{
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[0], CHANNEL_1}},
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[1], CHANNEL_1}},
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[2], CHANNEL_1}},
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[3], CHANNEL_1}},
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[4], CHANNEL_1}},
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[5], CHANNEL_1}},
};

Bankable::CCValue<1> ccmuxChannel[6] = 
{
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[0], CHANNEL_2}},
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[1], CHANNEL_2}},
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[2], CHANNEL_2}},
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[3], CHANNEL_2}},
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[4], CHANNEL_2}},
  {{sixButBank2, BankType::CHANGE_ADDRESS}, {buttonMidi[5], CHANNEL_2}},
};

