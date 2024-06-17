# JDS8000 Command Guide

## WAVE FUNCTIONS

### Channel On/Off

##### [0=Off|1=On]

    :w10=1,0,           CH1 On, CH2 Off

### Waveform

##### Only 0-21 are predefined, though 22-39 are supported

    :w11=0,             CH1 Wave 0=Sine
    :w12=5,             CH2 Wave 5=CMOS * NOTE: when reading this value, a second param is returned indicating an internal waveform index for the user definable internal presets eg: :r12=039,0802.

### Frequency

##### Freq * 1000, [0=Hz|1=kHz|2=MHz|3=MHz|4=uHz]

    :w13=4149126,0,     CH1 Freq 4,149.126, Hz
    :w14=246500,1,      CH2 Freq 246.500, kHz (anything other than Hz causes strange display)

### Amplitude

##### Ampl * 1000

    :w15=2520,          CH1 Ampl 2.520 Vpp
    :w16=3333,          CH2 Ampl 3.333 Vpp

### Offset

##### Offset * 100 + 1000 (1000 = 0)

    :w17=1360,          CH1 Offset  3.60 V (1000 + 360)
    :w18=960,           CH2 Offset -0.40 V (1000 - 040)

### Duty Cycle

##### Duty * 100 (default is 50% on waveforms that support it)

    :w19=578,           CH1 Duty 5.78%
    :w20=8865,          CH2 Duty 88.65%

### Phase

##### Phase * 100 (send as pair, CH2 first)

    :w22=754,           CH2 Phase 7.54 deg
    :w21=754,           CH1 Phase 7.54 deg

### Channel Sync

##### Wave, Freqeuncy, Amplitude, Offset, Duty, External

    :w25=0,1,0,0,1,0,   Sync Frequency, Duty

## CONTROL FUNCTIONS

#### === Work in Progress ===

    :w24=w,x,y,z,       w: 0 for WAV/MOD, 1=SYS, 2=SWEEP, 3=VCO, 4=MEAS, 5=Controls graphic header ---\______/----
                        x: 0=Language Setup, 1=CH1 Focus, 2=CH2 Focus, 3=SYstem Setup, 4=Measurement, 5=Counter, 6=Sweep Freq, 7=VCO Freq
                        y: 0-6=WAV for CH, 7=MOD for CH2 (combinations with z != 0 selected individual parameters)
                        z: 0-6=WAV for CH, 7=MOD for CH1 (combinations with y != 6 selected individual parameters)

### Set sweep freq

    :w24=0,6,0,1,       CH1 Sweep Frequency
    :w24=1,6,0,1,       CH2 Sweep Frequency

### Set sweep ampl

    :w24=0,6,1,1,       CH1 Sweep Amplitude
    :w24=1,6,1,1,       CH2 Sweep Amplitude

### Set sweep duty

    :w24=0,6,2,1,       CH1 Sweep Duty
    :w24=1,6,2,1,       CH2 Sweep Duty

#### === Work in Progress ===

    :w26=0,333,         Clear (what?)

## MOD FUNCTIONS

### Modulation Type

##### [0=AM|1=FM|2=PM|3=ASK|4=FSK|5=PSK|6=PWM|7=Burst]

    :w40=1,2,           CH1 FM, CH2 PM 

### Mod Shape

##### [0=Sine|1=Square|2=Triangle|3=UpRamp|4=DownRamp|5=Arb101|6=Arb102|7=Arb103|8=Arb104|9=Arb105]

    :w41=2,4,           CH1 Shape Triangle, CH2 Shape DownRamp

### Mod Source

##### [0=Internal|1=External]

    :w42=1,0,           CH1 Ext., CH2 Int.

### Mod Freq/Rate

##### Hz * 1000

    :w43=514320,        CH1 Mod.Freq 514.320 Hz
    :w44=666666,        CH2 Rate 666.666 Hz

### Mod AM Depth

##### Percent * 10

    :w45=800,           CH1 Depth 80.0%
    :w46=1156,          CH2 Depth 115.2%

### Mod FM Deviation

##### Hz * 10

    :w47=220,           CH1 F.Dev 22.0 Hz
    :w48=741,           CH2 F.Dev 74.1 Hz

### Mod FSK Hop Frequency

##### Hz * 10

    :w49=432,           CH1 Hop.F 43.2 Hz
    :w50=417,           CH2 Hop.F 41.7 Hz

### Mod PM Deviation/PSK Phase

##### Degrees * 10

    :w51=2349,          CH1 P.Dev 234.9 deg
    :w52=127,           CH2 Phase 12.7 deg

### Mod PWM Width

##### 0-11.000 us * 1000

    :w53=4923,          CH1 Width 4.923 uss
    :w54=271,           CH2 Width 0.271 us

### Mod PWM Period

##### 0-40,000,000.00 us * 100

    :w55=1701,          CH1 Period 17.01 us
    :w56=1200002,       CH2 Period 12,000.02 us

### Mod PWM Waveform Inverse

##### [0=Normal|1=Inverse]

    :w57=1,0,           CH1 Inv., CH2 Normal

### Mod Burst Idle

##### [0=Zero|1=PositiveMax|1=NegativeMax]

    :w58=1,2,           CH1 P.MAX, CH2 N.MAX

### Mod Polarity

##### [0=Positive|1=Negative]

    :w59=0,1,           CH1 Pos., CH2 Neg.

### Mod Burst Trigger Source

##### [0=Method:Single/Trig.Src:Trig.(Button)|1=Method:Auto/Internal(Auto)|2=Trig.Src:External(AC)|3=Trig.Src:External(DC)]

    :w60=1,3,           CH1 Method:Auto/Internal(Auto), CH2 3=Trig.Src:External(DC)

### Mod Burst Pulse Number

##### 1-1,000,000,000

    :w61=5,2,           CH1 Number 5, CH2 Number 2

#### === Work in Progress ===

##### 0=Method:Single/Trig.Src:Trig.(Button) Trigger

    :w74=1,0,0,0,       CH1
    :w74=0,1,0,0,       CH2
    :w73=45789,         :r73=45789.
    :w72=554,           :r72=00554.

#### === Work in Progress ===

## SWEEP FRERQUENCY FUNCTIONS

### Choose Sweep Frequency Mode

    :w24=0,6,0,1,       Requires exact values

### Sweep Config

##### Channel [0|1], Time (0-640 * 100s), Direction [0=Increase|1=Decrease|2=Round], Mode [0=Line|1=Logarithm]

    :w64=0,1000,0,0,    CH 1, Time 30.00s, Direction Increasing, Mode Line
    :w64=1,64000,2,1,   CH 2, Time 640.00s, Direction Round, Mode Logarithm

### Sweep

##### [0=Off|1=On]

    :w65=1,0,           CH1 Sweep On, CH2 Sweep Off

### Start Frequency

##### Hz * 10

    :w66=225,           22.5 Hz

### Stop Frequency

##### Hz * 10

    :w67=241000,        24,100.0 Hz

## SWEEP AMPLITUDE FUNCTIONS

### Choose Sweep Amplitude Mode

    :w24=0,6,1,1,       Requires exact values

### Sweep Config

##### Channel [0|1], Time (0.01-640.00 * 100s), Direction [0=Increase|1=Decrease|2=Round], Mode [0=Line|1=Logarithm]

    :w64=0,1000,0,0,    CH 1, Time 30.00s, Direction Increasing, Mode Line
    :w64=1,64000,2,1,   CH 2, Time 640.00s, Direction Round, Mode Logarithm

### Start Amplitude

##### 0-25.00 Vpp * 1000

    :w68=4200,          4.200 Vpp

### Stop Amplitude

##### 0-25.00 Vpp * 1000

    :w69=25000,         25.000 Vpp

## SWEEP DUTY FUNCTIONS

### Choose Sweep Duty Mode

    :w24=0,6,2,1,       Requires exact values

### Sweep Config

##### Channel [0|1], Time (0-640 * 100s), Direction [0=Increase|1=Decrease|2=Round], Mode [0=Line|1=Logarithm]

    :w64=0,1000,0,0,    CH 1, Time 30.00s, Direction Increasing, Mode Line
    :w64=1,64000,2,1,   CH 2, Time 640.00s, Direction Round, Mode Logarithm

### Start Duty

##### 0.00-99.99% * 100

    :w70=1743,          17.54%

### Stop Duty

##### 0.00-99.99% * 100

    :w71=9289,          92.89%

## VCO FUNCTIONS

    TBD

## READ LOCATIONS

    :r01=0,             Serial Number [:r01=2280600098.]

## MEASUREMENT MODE

### Start Frequency

##### Hz * 10

    :w66=225,           22.5 Hz

### Stop Frequency

##### Hz * 10

    :w67=241000,        24,100.0 Hz

* Reads 81-86 repeatedly
